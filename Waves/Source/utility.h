#pragma once
#include "pch.h"
#include <string>

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

// ==========================================================
// Font Management
// ==========================================================

namespace FontManager {
    // Initializes and loads all fonts needed in the game.
    void InitializeFonts();

    // Retrieves a previously loaded font by a unique key (e.g., "NunitoBold70").
    // Returns the font handle, or -1 if not found.
    s8 GetFont(const std::string& key);

    // Unloads all fonts and clears the cache. Call this at game shutdown.
    void ShutdownFonts();
}
