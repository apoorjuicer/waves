#pragma once
#include "AEEngine.h"
#include "Player.h"
#include "Wave.hpp"

struct Rock {
    float x, y, dx, dy;  // Position in world space
    int currBounce, maxBounce;
};

namespace rock {
	void drawRock(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY, Wave (&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh);
}