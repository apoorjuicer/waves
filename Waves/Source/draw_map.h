/* Start Header ************************************************************************/
/*!
\file		draw_map.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "AEEngine.h"

#pragma once

struct boundaryBlock {
	f32 x;
	f32 y;
	f32 scale_x;
	f32 scale_y;
};

struct wallBlock {
	f32 x;
	f32 y;
	f32 scale_x;
	f32 scale_y;
};

extern wallBlock* wallList;
extern boundaryBlock* boundaryList;
extern int wallCount;
extern int boundaryCount;

AEGfxVertexList* createBoundaryMesh(u32 boundary_color);
AEGfxVertexList* createWallMesh(u32 wall_color);

void addWall(f32 x, f32 y, f32 scale_x, f32 scale_y);
void addBoundary(f32 x, f32 y, f32 scale_x, f32 scale_y);

void drawWalls(AEGfxVertexList* wallMesh);
void drawBoundaries(AEGfxVertexList* boundaryMesh);