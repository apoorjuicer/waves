/* Start Header ************************************************************************/
/*!
\file		splash.cpp
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the SplashState class, which manages the splash screen of the game.
			It handles loading, rendering, and transitioning to the main menu after a specified duration.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "splash.h"

SplashState::SplashState()
    : mTimeElapsed(0.0f), mSplashDuration(2.0f), digipen(nullptr), digipen_mesh(nullptr) {}

SplashState::~SplashState() 
{
}

void SplashState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // Load assets here (e.g., splash image)
    // mSplashTexture = AEGfxTextureLoad("digipen_logo.png");
    digipen = AEGfxTextureLoad("Assets/DigiPen_Singapore_WEB_RED.png");

    if (digipen == nullptr) {
        printf("ERROR: Failed to load texture 'Assets/Maze1.png'\n");
    }

    digipen_mesh = TextureMesh();
}

void SplashState::Initialize() {
    mTimeElapsed = 0.0f;
}

void SplashState::Update(f32 deltaTime) {
    mTimeElapsed += deltaTime;
}

void SplashState::Draw() {
    // 1) Prepare rendering with texture & blending
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // 2) Bind the DigiPen logo texture
    AEGfxTextureSet(digipen, 0.0f, 0.0f);

    // 3) Query window dimensions
    float winW = (float)AEGfxGetWindowWidth();
    float winH = (float)AEGfxGetWindowHeight();

    // 4) Set how much of the window you want to fill (e.g., 80%)
    float fillPercent = 0.8f;

    // 5) Known or measured aspect ratio of your logo
    //    e.g., if the logo is ~2.5 times as wide as tall:
    float logoAspect = 2.5f;

    // Start by fitting the smaller dimension:
    float scaleY = fillPercent * ((winW < winH) ? winW : winH);
    // scaleX = scaleY * aspectRatio
    float scaleX = scaleY * logoAspect;

    // 6) If scaleX or scaleY exceed the window, clamp them
    //    so the logo won’t go offscreen.
    if (scaleX > winW * fillPercent) {
        scaleX = winW * fillPercent;
        scaleY = scaleX / logoAspect;
    }
    if (scaleY > winH * fillPercent) {
        scaleY = winH * fillPercent;
        scaleX = scaleY * logoAspect;
    }

    // 7) Build a transformation matrix
    AEMtx33 transform;
    AEMtx33Identity(&transform);

    // Because the mesh is -0.5..+0.5, scaling by scaleX, scaleY
    // yields a final bounding box of (scaleX × 1) in width
    // and (scaleY × 1) in height, preserving aspect ratio.
    AEMtx33Scale(&transform, scaleX, scaleY);

    // Center on (0,0).  (If your camera is at (0,0), this is screen center.)
    AEMtx33TransApply(&transform, &transform, 0.0f, 0.0f);

    // 8) Apply and draw
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(digipen_mesh, AE_GFX_MDM_TRIANGLES);
}

void SplashState::Free() {
    AEGfxMeshFree(digipen_mesh);
}

void SplashState::Unload() {
    AEGfxTextureUnload(digipen);
}

BaseState* SplashState::GetNextState() {
    if (mTimeElapsed >= mSplashDuration) {
        return new MainMenuState();
    }
    return nullptr;
}
