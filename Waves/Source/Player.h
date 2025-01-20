#pragma once

AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
