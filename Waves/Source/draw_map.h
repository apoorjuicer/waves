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