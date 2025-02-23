#pragma once
//enemy.h
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"

#include <utility> // pair
#include <vector> //vector
#include <iostream>
#include <cmath> //sqrt
#include <stack> 
#include <set> //set
#include <cfloat> //flt_max
#include <cstring>
#include <algorithm> 

#define ROW 10
#define COL 10
#define TILE_SIZE_X 100;
#define TILE_SIZE_Y -100;
#define ENEMY_SPEED 4.0f  // Pixels per frame (adjust as needed)

// Creating a shortcut for int, int pair type
typedef std::pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef std::pair<double, std::pair<int, int> > pPair;

enum Enemy_State { // enemy states
	IDLE,
	PATROL,
	PURSUIT_PLAYER,
	PURSUIT_ROCK
};

struct Point {
	int row, col;
	// Constructor
	Point(int r, int c) : row(r), col(c) {} // so can ini Point p(3, 5); like this
};
//node struct
//struct Node {
//	int x, y; // Coordinates
//	int parentX, parentY; // Parent Node coordinates
//	int f, g, h;          // Costs: f = g + h
//};
struct Node {
	// Row and Column index of its parent
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	int parent_i, parent_j;
	// f = g + h
	double f, g, h;
};

// Structure to store a Node's details
//struct Node {
//	int parentX, parentY; // Parent Node coordinates
//	int f, g, h;          // Costs: f = g + h
//};


struct Enemy {
	f32 x, y;
	f32 w, h;
	enum Enemy_State state;
	//vector<pair<int, int>> path; // Stores A* path
	std::vector<Pair> path = {};
	int pathIndex = 0; // Current target in path

	//// Function to call A* and store the path
	//void findPath(int grid[][COL], Pair src, Pair dest) {
	//	path.clear(); // reset prev path
	//	AstarSearch2(grid, src, dest, path); // Calls A* and fills the path
	//	pathIndex = 0; // Reset path progress
	//}

	// Moves enemy along the computed path
	void moveToNextStep() {
		printf("moveToNextStep called\n");
		if (path.empty()) {
			printf("path is empty\n");
			return;
		}
		if (pathIndex < path.size()) {
			Pair nextPos = path[pathIndex];  // Get next position
			//grid to world
			f32 targetX = nextPos.second *TILE_SIZE_X;  // Update enemy's row
			f32 targetY = nextPos.first * TILE_SIZE_Y; // Update enemy's col
			//movement dir vectpr
			f32 deltaX = targetX - x;
			f32 deltaY = targetY - y;
			f32 distance = sqrt(deltaX * deltaX + deltaY * deltaY);
			//move gradually
			// Move towards the target gradually
			if (distance > ENEMY_SPEED) {
				x += (deltaX / distance) * ENEMY_SPEED;  // Move step-by-step
				y += (deltaY / distance) * ENEMY_SPEED;
			}
			else {
				// Snap to the exact target when close enough
				x = targetX;
				y = targetY;
				pathIndex++;  // Move to the next step in path
			}



			//pathIndex++; // Move to next step
			printf("Moving to: (%d, %d)\n", nextPos.second, nextPos.first);
		}
		else {
			printf("Path finished, enemy has reached destination.\n");
		}
	}

	//// Display enemy position
	void printPosition() {
		std::cout << "Enemy Position: (" << x << ", " << y << ")\n";
	}
};


//void Enemy_Pathfinding_aStar(int startX, int startY, int goalX, int goalY);
//void enemy_pathfinding_astar2();
//void aStarSearch(int grid[][COL], Pair src, Pair dest);
void RenderEnemy_Mesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
void drawEnemy(Enemy& enemy, u32 colour);
AEGfxVertexList* createEnemy_mesh(u32 color);
void EnemyState(Enemy& e);
void UpdateEnemystate(Enemy& e);
void AstarSearch2(int grid[][COL], Pair src, Pair dest, std::vector<Pair>& path);

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