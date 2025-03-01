#include "AEEngine.h"
#include <vector>

#pragma once

struct enemy {
	f32 x;
	f32 y;
	f32 scalex;
	f32 scaley;
};
AEGfxVertexList* createEnemyMesh(u32 enemy_color);
int Heuristic(int x1, int y1, int x2, int y2);
std::vector<std::pair<int, int>> AStarPathfinding(int startX, int startY, int goalX, int goalY);
