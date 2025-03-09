/* Start Header ************************************************************************/
/*!
\file		enemy_henrey.cpp
\author		Henry Hu
\email		zhihenghenrey.hu@digipen.edu

\brief		This file is intended for defining enemy-related functionality, including enemy struct definitions and mesh creation.
			It provides the foundation for rendering and managing enemies within the game using Alpha Engine's graphics system.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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
