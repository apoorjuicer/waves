/* Start Header ************************************************************************/
/*!
\file		mainMenu.h
\author		Jordain Ng
\email		jordainyingzhe.ng@digipen.edu

\brief		This file defines functions and structures for managing and rendering walls and boundaries in the game world. 
			It provides functionality to create meshes, 
			add wall and boundary blocks, and render them using Alpha Engine's graphics system.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "draw_map.h"
#include "Player.h"

wallBlock* wallList = NULL;
boundaryBlock* boundaryList = NULL;
int wallCount = 0;
int boundaryCount = 0;

AEGfxVertexList* createBoundaryMesh(u32 boundary_color) {
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, boundary_color, 0.0f, 1.0f,
		0.5f, -0.5f, boundary_color, 1.0f, 1.0f,
		-0.5f, 0.5f, boundary_color, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, boundary_color, 1.0f, 1.0f,
		0.5f, 0.5f, boundary_color, 1.0f, 0.0f,
		-0.5f, 0.5f, boundary_color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* createWallMesh(u32 wall_color) {
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, wall_color, 0.0f, 1.0f,
		0.5f, -0.5f, wall_color, 1.0f, 1.0f,
		-0.5f, 0.5f, wall_color, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, wall_color, 1.0f, 1.0f,
		0.5f, 0.5f, wall_color, 1.0f, 0.0f,
		-0.5f, 0.5f, wall_color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}


void addWall(f32 x, f32 y, f32 scale_x, f32 scale_y) {
	wallBlock* temp = (wallBlock*)realloc(wallList, sizeof(wallBlock) * (wallCount + 1));
	if (temp == NULL) {
		// Handle memory allocation failure (e.g., log error and exit)
		fprintf(stderr, "Failed to allocate memory for wallList\n");
		return;
	}
	wallList = temp;

	wallList[wallCount].x = x;
	wallList[wallCount].y = y;
	wallList[wallCount].scale_x = scale_x;
	wallList[wallCount].scale_y = scale_y;
	wallCount++;
}

void addBoundary(f32 x, f32 y, f32 scale_x, f32 scale_y) {
	boundaryBlock* temp = (boundaryBlock*)realloc(boundaryList, sizeof(boundaryBlock) * (boundaryCount + 1));
	if (temp == NULL) {
		// Handle memory allocation failure (e.g., log error and exit)
		fprintf(stderr, "Failed to allocate memory for boundaryList\n");
		return;
	}
	boundaryList = temp;

	boundaryList[boundaryCount].x = x;
	boundaryList[boundaryCount].y = y;
	boundaryList[boundaryCount].scale_x = scale_x;
	boundaryList[boundaryCount].scale_y = scale_y;
	boundaryCount++;
}

void drawWalls(AEGfxVertexList* wallMesh) {
	for (int i = 0; i < wallCount; i++) {
		RenderMesh(
			wallMesh,
			wallList[i].x,
			wallList[i].y,
			wallList[i].scale_x,
			wallList[i].scale_y,
			0.0f // Rotation
		);
	}
}

void drawBoundaries(AEGfxVertexList* boundaryMesh) {
	for (int i = 0; i < boundaryCount; i++) {
		RenderMesh(
			boundaryMesh,
			boundaryList[i].x,
			boundaryList[i].y,
			boundaryList[i].scale_x,
			boundaryList[i].scale_y,
			0.0f // Rotation
		);
	}
}
