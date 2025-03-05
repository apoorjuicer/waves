#pragma once
//enemy.h
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"

#include <utility> // pair
#include <vector> //vector
#include <iostream>
#include <cmath> //sqrt
#include <limits> //infinity()
#include <algorithm> //sort

#define ROW 10
#define COL 10
//#define TILE_SIZE_X 100
//#define TILE_SIZE_Y -100
#define ENEMY_SPEED 8.0f  // Pixels per frame (adjust as needed)

// create shortcut for <int, int> pair
typedef std::pair<int, int> Pair; //contains i and j
// create shortcut for <double, pair<int, int>> pair
typedef std::pair<double, std::pair<int, int> > pPair; //contains fcost,i,j
constexpr double VAL_INF = std::numeric_limits<double>::infinity(); //infinity for init

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
	f32 w, h; //scale
	enum Enemy_State state;
	//vector<pair<int, int>> path; // Stores A* path
	std::vector<Pair> path = {};
	int pathIndex = 0; // Current target in path
	bool reachPath = false;
	// Constructor initializes all members
	Enemy() : x{ 0 }, y{ 0 }, w{ 50 }, h{ 50 }, state{ IDLE } {}
	// Moves enemy along the computed path
	void moveEnemy() {
		printf("moveToNextStep called\n");
		if (path.empty()) {
			printf("path is empty\n");
			return;
		}
		if (pathIndex < path.size()) {
			Pair nextPos = path[pathIndex];  // Get next position
			//grid to world
			f32 targetX = nextPos.second * TILE_SIZE;  // Update enemy's row
			f32 targetY = nextPos.first *  -TILE_SIZE; // Update enemy's col
			////movement dir vectpr
			f32 dirX = targetX - x;
			f32 dirY = targetY - y;
			f32 distance = sqrt(dirX * dirX + dirY * dirY);
			//normalize dir vector and apply vel
			if (distance > ENEMY_SPEED) {
				x += (dirX / distance) * ENEMY_SPEED;
				y += (dirY / distance) * ENEMY_SPEED;
			}
			else {
				// Snap to the exact target when close enough
				x = targetX;
				y = targetY;
				pathIndex++;  // Move to the next step in path
			}


			printf("Moving to: (%d, %d)\n", nextPos.second, nextPos.first);
		}
		else {
			printf("Path finished, enemy has reached destination.\n");
		}
	}

	// Display enemy position, for debug
	void printPosition() {
		std::cout << "Enemy Position: (" << x << ", " << y << ")\n";
	}
};


//void Enemy_Pathfinding_aStar(int startX, int startY, int goalX, int goalY);
//void enemy_pathfinding_astar2();
//void aStarSearch(int grid[][COL], Pair src, Pair dest);
void RenderEnemy_Mesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* createEnemy_mesh(u32 color);
void EnemyState(Enemy& e);
void UpdateEnemystate(Enemy& e);
Path astarsearch(int grid[][COL], Pair src, Pair dest, std::vector<Pair>& path);

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