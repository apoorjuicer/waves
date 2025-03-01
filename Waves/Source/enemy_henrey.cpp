#include "AEEngine.h"
#include "Maze.h"
#include <vector>
#include <iostream>
#include <queue>
#include <utility>
#include <cmath>
#include <tuple>


#define MAZE_SIZE 20
#define INF 1000000  // Large number to represent infinity

// Function to create the enemy mesh
AEGfxVertexList* createEnemyMesh(u32 enemy_color) {
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, enemy_color, 0.0f, 1.0f,
		0.5f, -0.5f, enemy_color, 1.0f, 1.0f,
		-0.5f, 0.5f, enemy_color, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, enemy_color, 1.0f, 1.0f,
		0.5f, 0.5f, enemy_color, 1.0f, 0.0f,
		-0.5f, 0.5f, enemy_color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

// Directions: RIGHT, LEFT, DOWN, UP
int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };

// Cost arrays
int G[MAZE_SIZE][MAZE_SIZE];  // Movement cost from start
int H[MAZE_SIZE][MAZE_SIZE];  // Heuristic cost to goal
int F[MAZE_SIZE][MAZE_SIZE];  // Total cost F = G + H
std::pair<int, int> parent[MAZE_SIZE][MAZE_SIZE];  // Parent for path reconstruction

// Priority queue comparator (sorts by F-cost, breaks ties using G-cost)
struct Compare {
    bool operator()(std::pair<int, int> a, std::pair<int, int> b) {
        if (F[a.second][a.first] == F[b.second][b.first])
            return G[a.second][a.first] > G[b.second][b.first];  // Lower G-cost wins tie
        return F[a.second][a.first] > F[b.second][b.first];  // Default: Sort by F-cost
    }
};

// Manhattan Distance Heuristic
int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

// A* Pathfinding using 2D array
std::vector<std::pair<int, int>> AStarPathfinding(int startX, int startY, int goalX, int goalY) {
    // Initialize cost arrays and parent tracking
    for (int y = 0; y < MAZE_SIZE; ++y) {
        for (int x = 0; x < MAZE_SIZE; ++x) {
            G[y][x] = INF;
            H[y][x] = Heuristic(x, y, goalX, goalY);
            F[y][x] = INF;
            parent[y][x] = { -1, -1 };
        }
    }

    // Track processed tiles (closed list)
    bool closedList[MAZE_SIZE][MAZE_SIZE] = { false };

    // Priority queue for open set (tiles to be processed)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> openSet;

    // Start position setup
    G[startY][startX] = 0;
    F[startY][startX] = H[startY][startX];
    openSet.push({ startX, startY });

    while (!openSet.empty()) {
        // Get the tile with the lowest F-cost
        std::pair<int, int> current = openSet.top();
        openSet.pop();

        int curX = current.first;
        int curY = current.second;

        // Mark current tile as processed
        closedList[curY][curX] = true;

        // If goal is reached, reconstruct path
        if (curX == goalX && curY == goalY) {
            std::vector<std::pair<int, int>> path;
            while (curX != startX || curY != startY) {
                path.push_back({ curX, curY });
                std::tie(curX, curY) = parent[curY][curX];
            }
            std::reverse(path.begin(), path.end());
            return path;  // Return the final path
        }

        // Explore all 4 directions
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];

            // Check if it's inside the maze, walkable, and NOT already processed
            if (newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE && maze[newY][newX] == 0 && !closedList[newY][newX]) {
                int newG = G[curY][curX] + 1;  // Each step costs 1

                // If new path is better, update costs and parent
                if (newG < G[newY][newX]) {
                    G[newY][newX] = newG;
                    F[newY][newX] = G[newY][newX] + H[newY][newX];
                    parent[newY][newX] = { curX, curY };
                    openSet.push({ newX, newY });
                }
            }
        }
    }

    return {};  // Return empty path if no solution
}
