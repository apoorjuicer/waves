/* Start Header ************************************************************************/
/*!
\file		Enemy.h
\author		Benjamin Ban
\email		k.ban@digipen.edu
\co-author  Jordain
\email
\co-author  Henry
\email

\brief	This header file declares functions that are necessary for dealing with
		enemy movement and ai path finding

		Functionality includes pathfinding,enemystates, rendering enemy and
		moving enemy

		Benjamin:
		-typedef alias and constexpr
		-Structs and enums

		Jordain:
		-chase rock state
		-RenderEnemy_Mesh

		Henry:
		-createEnemyMesh()

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include "Wave.h"
//#include "game.h"

#include <utility> // pair
#include <vector> //vector
#include <iostream> //debug check
#include <cmath> //sqrt
#include <limits> //infinity()
#include <algorithm> //sort



//#define TILE_SIZE_X 100
//#define TILE_SIZE_Y -100
constexpr float ENEMY_SPEED = 16.0f;  // Pixels per frame (adjust as needed)
constexpr double VAL_INF = std::numeric_limits<double>::infinity(); //infinity for init

// create shortcut for <int, int> pair
typedef std::pair<int, int> Pair; //contains i and j
// create shortcut for <double, pair<int, int>> pair
typedef std::pair<double, std::pair<int, int> > pPair; //contains fcost,i,j



enum class Path {
	NO_PATH = 0,
	PATH_FOUND = 1
};

enum Enemy_State { // enemy states
	IDLE,
	PATROL,
	PURSUIT_PLAYER, //CHASE, when player in vicinity
	PURSUIT_ROCK //ALERT, when rock in vicinitity
};

struct Node {
	// Row and Column index of its parent
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	int parent_i, parent_j; // Parent Node coordinates
	double f, g, h; // Costs: f = g + h
};


struct Enemy {
	f32 x, y; //x-y pos
	int gridX, gridY;
	f32 w = 50.f, h = 50.f; //scale
	Pair startPatrol;
	Pair endPatrol;
	//float startPatrolx, startPatroly;
	//float endPatrolX, endPatrolY;
	enum Enemy_State state = PATROL;
	enum Enemy_State previousState;  // Track previous state to detect state changes
	//vector<pair<int, int>> path; // Stores A* path
	std::vector<Pair> path = {};
	std::vector<Pair> patrolPath = {};
	Pair prevPlayerPos;
	int pathIndex = 0; // Current target in path
	bool reachPath = false;
	//Wave enemy_waves[MAX_NUM_WAVES];
	float stateTimer = 0;
	float idle_timer = 3.f;
	float patrol_timer =  10.f;
	float pursuit_timer = 3.f;
	// Constructor initializes all members
	Enemy() : x{ 100.f }, y{ -200.f }, gridX{ 1 }, gridY{ 1 }, w{ 50.f }, h{ 50.f }, state{ IDLE }
		, previousState{IDLE} {}

	void enemy_wave(Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		static f32 time{};
		if (state != IDLE) {
			time += static_cast<f32>(AEFrameRateControllerGetFrameTime());
			if (time > 1.0) {
				spawn_wave(MainWaves, x, y, mesh, 50.f);
				time = 0;
			}

		}
	}
	// Moves enemy along the computed path
	void moveEnemy(Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		(void)MainWaves;
		(void)mesh;

		//std::cout << "moving Enemy\n";
		if (path.empty()) {
			std::cout << "path is empty\n";
			return;
		}

		// Skip the first step if it's the same as the current position
		if (pathIndex == 0 && path[0] == Pair(gridX, gridY)) {
			//std::cout << "skip start pos: (" << path[0].first << ", " << path[0].second << ")\n";
			pathIndex++; // Move to the next valid step
		}

		if (pathIndex < path.size()) {
			Pair nextPos = path[pathIndex];  // Get next position
			//grid to world
			//f32 targetX = nextPos.second * TILE_SIZE;  // Update enemy's row
			////f32 targetY = nextPos.first * -TILE_SIZE; // Update enemy's col
			
			f32 targetX = nextPos.first * TILE_SIZE;  // Update enemy's row
			f32 targetY = nextPos.second * -TILE_SIZE; // Update enemy's col
			// 
			////movement dir vectpr
			f32 dirX = targetX - x;
			f32 dirY = targetY - y;
			f32 distance = sqrt(dirX * dirX + dirY * dirY);
			//normalize dir vector and apply vel
			if (distance > ENEMY_SPEED) {
				x += (dirX / distance) * ENEMY_SPEED;
				y += (dirY / distance) * ENEMY_SPEED;
				//pathIndex++;  // Move to the next step in path
			}
			else {
				// Snap to the exact target when close enough
				//void spawn_wave(Wave(&arr)[MAX_NUM_WAVES], f32 pos_x, f32 pos_y, AEGfxVertexList** mesh_in, f32 max_size = WAVES_SIZE);
				
				x = targetX;
				y = targetY;
				pathIndex++;  // Move to the next step in path
			}

			std::cout << "Moving to: (" << nextPos.first << ", " << nextPos.second << ")\n";
		}
		
		updateGridPosition();
		//printPosition();
	}
	void updateGridPosition() {
		/*gridX = static_cast<int>(round(x / TILE_SIZE));
		gridY = static_cast<int>(round(y / -TILE_SIZE));*/
		//manual offset superior
		gridX = static_cast<int>((x + TILE_SIZE / 2) / TILE_SIZE);
		gridY = static_cast<int>((y - TILE_SIZE / 2) / -TILE_SIZE);
	}
	// Display enemy position, for debug
	void printPosition() {
		std::cout << "Enemy Position in World: (" << x << ", " << y << ")\n";
		std::cout << "Enemy Position in Grid: (" << gridX << ", " << gridY << ")\n";
	}
};


//void Enemy_Pathfinding_aStar(int startX, int startY, int goalX, int goalY);
//void enemy_pathfinding_astar2();
//void aStarSearch(int grid[][COL], Pair src, Pair dest);
void RenderEnemy_Mesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* createEnemyMesh(u32 enemy_color);
void EnemyState(Enemy& e, float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
void UpdateEnemystate(Enemy& e, float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
Path astarsearch(int grid[MAX_ROW][MAX_COL], Pair src, Pair dest, std::vector<Pair>& path);

std::vector<Pair> findPatrolPoints(int grid[MAX_ROW][MAX_COL]);
void setEnemyPatrol(Enemy& enemy, int grid[MAX_ROW][MAX_COL]);
std::string GetEnemyStateName(Enemy_State state);
void printEnemyPatrolpoints(Enemy& e, int grid[MAX_ROW][MAX_COL]);
//class Enemy {
//public:
//	f32 x, y; //x y pos
//	f32 w, h;//might need w/h bounding rect?
//	Enemy_State state; //state of enemy
//
//	//void EnemyState(Enemy& e);
//	//void UpdateEnemystate(Enemy& e);
//
//	
//
//	void FindPath(int grid[][COL], Pair src, Pair dest);
//private:
//	std::vector<Pair> path;  // Stores A* computed path
//	int pathIndex = 0;  // Tracks which step the enemy is at(how many boxes away)
//
//
//};