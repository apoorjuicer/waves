#pragma once
struct player {
	//float x, y;
	AEVec2 position;  // Current position
	f32 radius;      // Collision radius
};
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour);
AEGfxVertexList* testing_only(); //to delete ltr

