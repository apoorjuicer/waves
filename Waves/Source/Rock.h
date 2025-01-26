#pragma once
#include "AEEngine.h"
#include "Player.h"

struct Rock {
    float x, y, dx, dy;  // Position in world space
    int currBounce, maxBounce;
};

namespace rock {
	void drawRock(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY);
}