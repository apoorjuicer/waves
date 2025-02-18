#pragma once
//enemy.h
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"

#include <utility> // pair
#include <vector>
#include <iostream>
#include <cmath>
#include <stack>
#include <set>
#include <cfloat>
#include <cstring>
#include <algorithm>

#define ROW 10
#define COL 10


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
struct Node {
	int x, y; // Coordinates
	int parentX, parentY; // Parent Node coordinates
	int f, g, h;          // Costs: f = g + h
};
struct cell {
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
	std::vector<std::pair<int, int>> path;
	int pathIndex = 0; // Current target in path
};

AEGfxVertexList* createEnemy_mesh(u32 color);
void RenderEnemy_Mesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
void drawEnemy(Enemy& enemy, u32 colour);
void EnemyState(Enemy& e);
void UpdateEnemystate(Enemy& e);
//void Enemy_Pathfinding_aStar(int startX, int startY, int goalX, int goalY);
//void enemy_pathfinding_astar2();
void aStarSearch(int grid[][COL], Pair src, Pair dest);



class Enemy {
public:
	f32 x, y; //x y pos
	f32 w, h;//might need w/h bounding rect?
	Enemy_State state; //state of enemy

	//void EnemyState(Enemy& e);
	//void UpdateEnemystate(Enemy& e);
private:
	std::vector<Pair> path;  // Stores A* computed path
	int pathIndex;  // Tracks which step the enemy is at(how many boxes away)
	void AstarSearch2(int grid[][COL], Pair src, Pair dest);

};