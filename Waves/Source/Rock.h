#pragma once
#include "AEEngine.h"
#include "Player.h"
#include "Wave.hpp"

struct Rock {
    float x, y, dx, dy;  // Position in world space
    int currBounce, maxBounce;
};

namespace rock {
	void drawRock(Player& player, int maze[ROW][COL], float tileSize, float mazeStartX, float mazeStartY, Wave (&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, Player& lastRock);
    AEGfxVertexList* Rock_Mesh(int NumOfSlices, u32 colour, f32 radius);
}