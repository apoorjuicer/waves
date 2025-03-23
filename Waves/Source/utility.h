#pragma once
#include "AEEngine.h"

// ==========================================================
// For main menu
// ==========================================================
bool IsMouseInsideButton(f32 buttonX, f32 buttonY, f32 buttonWidth, f32 buttonHeight);
bool IsMouseInsideButtonNDC(f32 centerX_NDC, f32 centerY_NDC, f32 width_NDC, f32 height_NDC);
AEGfxVertexList* CreateButtonMesh(u32 color);
void DrawButton(AEGfxVertexList* buttonMesh, f32 x, f32 y, const char* text, s8 font, f32 scaleX, f32 scaleY,
    f32 textR, f32 textG, f32 textB, f32 textA);
void DrawTextRightAligned(s8 font, const char* text, f32 xWorld, f32 yWorld, f32 r, f32 g, f32 b, f32 a);

// ==========================================================
// For select
// ==========================================================
void DrawTexturedButton(AEGfxVertexList* pMesh,
    f32 leftX, f32 topY,
    AEGfxTexture* pTexture,
    f32 width, f32 height);

// ==========================================================
// For game preview
// ==========================================================
AEGfxVertexList* TextureMesh();