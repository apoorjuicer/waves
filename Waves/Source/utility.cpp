#include "utility.h"

// ==========================================================
// For main menu
// ==========================================================
bool IsMouseInsideButton(f32 buttonX, f32 buttonY, f32 buttonWidth, f32 buttonHeight) {
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Convert screen space to world space
    f32 worldMouseX = (f32)mouseX - (AEGfxGetWindowWidth() / 2);
    f32 worldMouseY = (AEGfxGetWindowHeight() / 2) - (f32)mouseY;

    // Check if mouse is inside the rectangle
    return (worldMouseX >= buttonX - buttonWidth / 2 &&
        worldMouseX <= buttonX + buttonWidth / 2 &&
        worldMouseY >= buttonY - buttonHeight / 2 &&
        worldMouseY <= buttonY + buttonHeight / 2);
}

bool IsMouseInsideButtonNDC(f32 centerX_NDC, f32 centerY_NDC,
    f32 width_NDC, f32 height_NDC)
{
    // 1. Get mouse in screen coords (0..windowWidth, 0..windowHeight).
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // 2. Convert mouse to normalized device coords (–1..+1).
    f32 halfW = AEGfxGetWindowWidth() * 0.5f;
    f32 halfH = AEGfxGetWindowHeight() * 0.5f;
    f32 mouseXNDC = (mouseX - halfW) / halfW;
    f32 mouseYNDC = (halfH - mouseY) / halfH;

    // 3. Compute the bounding box in NDC.
    f32 left = centerX_NDC - (width_NDC * 0.5f);
    f32 right = centerX_NDC + (width_NDC * 0.5f);
    f32 bottom = centerY_NDC - (height_NDC * 0.5f);
    f32 top = centerY_NDC + (height_NDC * 0.5f);

    // 4. Check if mouse is inside that bounding box.
    return (mouseXNDC >= left && mouseXNDC <= right &&
        mouseYNDC >= bottom && mouseYNDC <= top);
}

AEGfxVertexList* CreateButtonMesh(u32 color) {
    AEGfxMeshStart();

    // Create a rectangle using two triangles
    AEGfxTriAdd(-0.5f, -0.5f, color, 0.0f, 1.0f,
        0.5f, -0.5f, color, 1.0f, 1.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, -0.5f, color, 1.0f, 1.0f,
        0.5f, 0.5f, color, 1.0f, 0.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    return AEGfxMeshEnd();
}

// Utility function to draw buttons with scaling
void DrawButton(AEGfxVertexList* buttonMesh, f32 x, f32 y, const char* text, s8 font, f32 scaleX, f32 scaleY,
    f32 textR, f32 textG, f32 textB, f32 textA) {
    if (!buttonMesh || font == -1) return;

    // Create scaling and translation matrices
    AEMtx33 scaleMtx, transMtx, finalMtx;
    AEMtx33Scale(&scaleMtx, scaleX, scaleY);  // Scale the mesh
    AEMtx33Trans(&transMtx, x, y);            // Position the mesh
    AEMtx33Concat(&finalMtx, &transMtx, &scaleMtx);
    AEGfxSetTransform(finalMtx.m);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    // Retrieve camera position and window dimensions for text conversion
    f32 camX, camY;
    AEGfxGetCamPosition(&camX, &camY);
    f32 winW = (f32)AEGfxGetWindowWidth();
    f32 winH = (f32)AEGfxGetWindowHeight();

    // Get the text dimensions (assumes scale of 1.0 for printing)
    f32 textWidth, textHeight;
    AEGfxGetPrintSize(font, text, 1.0f, &textWidth, &textHeight);

    // Convert the world button center (x,y) to normalized screen coordinates,
    // subtracting half the text width/height to center the text.
    f32 normX = (x - camX) / (winW / 2.0f) - (textWidth * 0.5f);
    f32 normY = (y - camY) / (winH / 2.0f) - (textHeight * 0.5f);

    // Draw the text in normalized coordinates with black color
    AEGfxPrint(font, text, normX, normY, 1.0f, textR, textG, textB, textA);
}

void DrawTextRightAligned(s8 font, const char* text, f32 xWorld, f32 yWorld, f32 r, f32 g, f32 b, f32 a)
{
    if (font == -1 || !text) return;

    // 1. Get the current camera position and window size.
    f32 camX, camY;
    AEGfxGetCamPosition(&camX, &camY);
    f32 winW = (f32)AEGfxGetWindowWidth();
    f32 winH = (f32)AEGfxGetWindowHeight();

    // 2. Measure the text width/height (in normalized coordinates).
    f32 textW, textH;
    AEGfxGetPrintSize(font, text, 1.0f, &textW, &textH);

    // 3. Convert from world coords to normalized device coords.
    //    Then subtract the entire text width to right-align.
    f32 normX = (xWorld - camX) / (winW / 2.0f) - textW;
    f32 normY = (yWorld - camY) / (winH / 2.0f);

    // 4. Draw in normalized coordinates.
    AEGfxPrint(font, text, normX, normY, 1.0f, r, g, b, a);
}

// ==========================================================
// For select
// ==========================================================
void DrawTexturedButton(AEGfxVertexList* pMesh,
    f32 leftX, f32 topY,
    AEGfxTexture* pTexture,
    f32 width, f32 height)
{
    f32 centerX = leftX + width * 0.5f;
    f32 centerY = topY - height * 0.5f;

    // 1) Create the final transform for this button
    AEMtx33 scaleMtx, transMtx, finalMtx;
    AEMtx33Scale(&scaleMtx, width, height);   // scale the 1×1 quad
    AEMtx33Trans(&transMtx, centerX, centerY);
    AEMtx33Concat(&finalMtx, &transMtx, &scaleMtx);

    // 2) Use texture rendering mode
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    // 3) Bind the texture
    AEGfxTextureSet(pTexture, 0.0f, 0.0f);

    // 4) Apply the transform
    AEGfxSetTransform(finalMtx.m);

    // 5) Draw the mesh (two triangles forming a quad)
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

// ==========================================================
// For game preview
// ==========================================================
AEGfxVertexList* TextureMesh() {
    AEGfxMeshStart();

    // Create a rectangle using two triangles
    AEGfxTriAdd(-0.5, -0.5, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5, -0.5, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5, 0.5, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(0.5, -0.5, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5, 0.5, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5, 0.5, 0xFFFFFFFF, 0.0f, 0.0f);

    return AEGfxMeshEnd();
}

// ==========================================================
// Font Manager Implementation
// ==========================================================

namespace FontManager {
    // Internal cache for font handles
    static std::unordered_map<std::string, s8> fontCache;

    void InitializeFonts() {
        // Load Nunito-Bold at multiple sizes
        fontCache["NunitoBold50"] = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 50);
        fontCache["NunitoBold70"] = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 70);
        fontCache["NunitoBold100"] = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 100);

        // Load Nunito-SemiBoldItalic at size 70
        fontCache["NunitoSemiBoldItalic70"] = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-SemiBoldItalic.ttf", 70);

        // Load Nunito-MediumItalic at size 100
        fontCache["NunitoMediumItalic100"] = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-MediumItalic.ttf", 100);

        // Load a separate font used in LoseState and WinState
        fontCache["Buggy80"] = AEGfxCreateFont("Assets/buggy-font.ttf", 80);

        // Load Nosifer-Regular for the main menu
        fontCache["NosiferRegular150"] = AEGfxCreateFont("Assets/Fonts/Nosifer-Regular.ttf", 150);
    }

    s8 GetFont(const std::string& key) {
        auto it = fontCache.find(key);
        if (it != fontCache.end()) {
            return it->second;
        }
        return -1; // Not found
    }

    void ShutdownFonts() {
        for (auto& pair : fontCache) {
            if (pair.second != -1) {
                AEGfxDestroyFont(pair.second);
                pair.second = -1;
            }
        }
        fontCache.clear();
    }
}