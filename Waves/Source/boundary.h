#pragma once
#include "AEEngine.h"
#include "Player.h"
struct Rect {
    AEVec2 center; // Center of the rectangle
    f32 width, height;
};

void Boundary_Clamp(player& p, f32 max_x, f32 max_y, f32 min_x, f32 in_y);
void collide_obstacle(player& p, Rect& obstacle);