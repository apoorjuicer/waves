#include "AEEngine.h"
#include <iostream>

// Function to create the enemy mesh
//AEGfxVertexList* createEnemyMesh(u32 enemy_color) {
//	// Informing the library that we're about to start adding triangles
//	AEGfxMeshStart();
//
//	// This shape has 2 triangles that makes up a square
//	// Color parameters represent colours as ARGB
//	// UV coordinates to read from loaded textures
//	AEGfxTriAdd(
//		-0.5f, -0.5f, enemy_color, 0.0f, 1.0f,
//		0.5f, -0.5f, enemy_color, 1.0f, 1.0f,
//		-0.5f, 0.5f, enemy_color, 0.0f, 0.0f);
//
//	AEGfxTriAdd(
//		0.5f, -0.5f, enemy_color, 1.0f, 1.0f,
//		0.5f, 0.5f, enemy_color, 1.0f, 0.0f,
//		-0.5f, 0.5f, enemy_color, 0.0f, 0.0f);
//
//	return AEGfxMeshEnd();
//}
