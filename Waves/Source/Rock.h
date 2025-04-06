/* Start Header ************************************************************************/
/*!
\file		Rock.h
\project    Waves

\author		Guan Shao Jun
\email		shaojun.g@digipen.edu

\brief      Handles the rock mechanics in the game, including rendering, movement, collision detection,
            and interaction with the maze.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "Player.h"
#include "Wave.h"
#include "Maze.h"
#include "Audio.h"

struct Rock {
    float x, y, dx, dy;  // Position in world space
    int currBounce, maxBounce;
    //std::vector<Rock> activeRocks;


};

namespace rock {
	void drawRock(Player& player, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX,
        f32 mazeStartY, Wave (&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, AEGfxVertexList* rockMesh, AEGfxTexture* rockTexture);
    AEGfxVertexList* Rock_Mesh(int NumOfSlices, u32 colour, f32 radius);
}