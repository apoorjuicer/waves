/* Start Header ************************************************************************/
/*!
\file		Rock.h
\author		Guan Shao Jun
\email		shaojun.g@digipen.edu

\brief
Handles the rock mechanics in the game, including rendering, movement, collision detection,
and interaction with the maze.

This module allows the player to throw a rock, checks for collisions with the maze,
applies velocity damping, and updates the rock’s position. It also tracks the rock’s
final resting position and modifies the maze accordingly.

\details
- `drawRock()`: Manages the rock's lifecycle, from launch to collision and final placement.
- `BounceCollision()`: Checks if the rock collides with walls and handles bounce behavior.
- `Rock_Mesh()`: Generates the graphical representation of the rock.
- The rock's last resting position is stored in the maze (`maze[ROW][COL] = 4`).

The rock is only launched if the player has one in their inventory, and upon collision,
it bounces off walls until stopping. The last position is recorded in the maze grid.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "Player.h"
#include "Wave.h"
#include "Maze.h"
#include "Audio.h"
#include <cmath>      // for std::floor
#include <iostream>   // for std::cout
struct Rock {
    float x, y, dx, dy;  // Position in world space
    int currBounce, maxBounce;
};

namespace rock {
	void drawRock(Player& player, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY, Wave (&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, Player& lastRock);
    AEGfxVertexList* Rock_Mesh(int NumOfSlices, u32 colour, f32 radius);
}