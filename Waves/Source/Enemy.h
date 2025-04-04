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
	constexpr float sightRangeRock = 3.5f * TILE_SIZE;  // Enemy detection range for rock
	constexpr float sightRangePlayer = 2.5f * TILE_SIZE;  // Enemy detection range for player

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

	void RenderEnemy_Mesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
	AEGfxVertexList* createEnemyMesh(u32 enemy_color);
	Path astarsearch(int grid[MAX_ROW][MAX_COL], Pair src, Pair dest, std::vector<Pair>& path);
	std::vector<Pair> findPatrolPoints(int grid[MAX_ROW][MAX_COL]);


	class Enemy {
	public:
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
		float patrol_timer = 10.f;
		float pursuit_timer = 4.f;
		//bool hasPursuedRock = false;

		Enemy() : x{ 100.f }, y{ -200.f }, gridX{ 1 }, gridY{ 1 }, w{ 50.f }, h{ 50.f }, state{ IDLE }
			, previousState{ IDLE } {}

		//member function
		void updateGridPosition();
		void printPosition()const;
		void enemy_wave(Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) const;
		void moveEnemy(Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
		void EnemyState(float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
		void UpdateEnemystate(float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, DirectionalWave dir_wave);
		void setEnemyPatrol(int grid[MAX_ROW][MAX_COL]);
		void printEnemyPatrolpoints()const;
		void print_debug() const;
		void EnterPatrolPath();
		void Update_EnemyPos(float& AT, float UT, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
		std::string GetEnemyStateName() const;
		void chasePlayer(float& AT, float UT, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
		bool seePlayer(float sightRange) const;
		bool seeRock(float sightRange) const;
		bool hitbywave(DirectionalWave dir_wave, AEVec2& out) const;
		void Update_Patrol(bool in_PatrolPath, Pair currIndexpos, float& AT, float UT, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
		
		void SetIdleState();
		void SetPatrolState();
		void SetPursuitPlayerState();
		void SetPursuitRockState();

	private:



	};




	


