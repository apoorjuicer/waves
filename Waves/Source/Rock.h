/* Start Header ************************************************************************/
/*!
\file		Rock.h
\author		Guan Shao Jun
\email		shaojun.g@digipen.edu

\brief

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

struct Rock {
    float x, y, dx, dy;  // Position in world space
    int currBounce, maxBounce;
};

namespace rock {
	void drawRock(Player& player, int maze[ROW][COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY, Wave (&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, Player& lastRock);
    AEGfxVertexList* Rock_Mesh(int NumOfSlices, u32 colour, f32 radius);
}