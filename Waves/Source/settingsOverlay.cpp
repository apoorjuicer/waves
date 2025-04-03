#include "settingsOverlay.h"

namespace {
    // File-scope internal variables
    bool sVisible = false;
    float currentVolume = 1.0f;
    bool isDraggingHandle = false;

    s8 volumeFont = -1;
    AEGfxVertexList* panelMesh = nullptr;
    AEGfxVertexList* sliderTrackMesh = nullptr;
    AEGfxVertexList* sliderHandleMesh = nullptr;
	AEGfxVertexList* okButtonMesh = nullptr;

    const f32 panelWidth = 600.0f;
    const f32 panelHeight = 400.0f;
    const f32 sliderWidth = 350.0f;
    const f32 sliderHeight = 20.0f;
    const f32 panelPosX = 0.0f;
    const f32 panelPosY = 0.0f;
    const f32 handleWidth = 30.0f;
    const f32 handleHeight = 30.0f;
    const f32 sliderY = panelPosY - 30.0f;

    const f32 kButtonW = 100.0f;
    const f32 kButtonH = 50.0f;
    const f32 kokY = -150.0f;
    const f32 kokX = 0.0f;

    f32 handleX = -sliderWidth / 2 + currentVolume * sliderWidth;
}

namespace SettingsOverlay {
    void Init() {
		AEGfxSetCamPosition(0.0f, 0.0f);
        sVisible = false;
        currentVolume = AudioManager::GetMasterVolume();
        volumeFont = FontManager::GetFont("NunitoBold50");
        panelMesh = CreateButtonMesh(0xFF606060);
        sliderTrackMesh = CreateButtonMesh(0xFFFFFFFF); 
        sliderHandleMesh = CreateButtonMesh(0xFF000000); 
		okButtonMesh = CreateButtonMesh(0xFFFFFFFF); 
    }

    void Update() {
        if (!sVisible) return;

        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (IsMouseInsideButton(kokX, kokY, kButtonW, kButtonH)) {
                sVisible = false;
            }
        }

        // Get mouse screen position
        s32 mouseX, mouseY;
        AEInputGetCursorPosition(&mouseX, &mouseY);

        // Convert screen to world coordinates
        f32 worldMouseX = (f32)mouseX - AEGfxGetWindowWidth() * 0.5f;
        f32 worldMouseY = (f32)-mouseY + AEGfxGetWindowHeight() * 0.5f;

        f32 sliderLeft = panelPosX - sliderWidth * 0.5f;
        f32 sliderRight = panelPosX + sliderWidth * 0.5f;

        f32 handleLeft = handleX - handleWidth * 0.5f;
        f32 handleRight = handleX + handleWidth * 0.5f;
        f32 handleTop = sliderY + handleHeight * 0.5f;
        f32 handleBottom = sliderY - handleHeight * 0.5f;

        // Start dragging
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (worldMouseX >= handleLeft && worldMouseX <= handleRight &&
                worldMouseY >= handleBottom && worldMouseY <= handleTop) {
                isDraggingHandle = true;
            }
        }

        // While dragging
        if (AEInputCheckCurr(AEVK_LBUTTON) && isDraggingHandle) {
            handleX = worldMouseX;

            // Clamp handle within slider bounds
            if (handleX < sliderLeft) handleX = sliderLeft;
            if (handleX > sliderRight) handleX = sliderRight;

            // Update volume based on clamped handleX
            currentVolume = (handleX - sliderLeft) / sliderWidth;
            AudioManager::SetMasterVolume(currentVolume);
        }

        // Stop dragging
        if (AEInputCheckReleased(AEVK_LBUTTON)) {
            isDraggingHandle = false;
        }
    }


    void Draw() {
        if (!sVisible) return;

        AEMtx33 t, s, m;
        AEMtx33Trans(&t, panelPosX, panelPosY);
        AEMtx33Scale(&s, panelWidth, panelHeight);
        AEMtx33Concat(&m, &t, &s);
        AEGfxSetTransform(m.m);
        AEGfxMeshDraw(panelMesh, AE_GFX_MDM_TRIANGLES);

        // ===== Draw "SETTINGS" Title =====
        // Get size of the text in normalized device coordinates
        float textW, textH;
        AEGfxGetPrintSize(volumeFont, "SETTINGS", 1.0f, &textW, &textH);

        // Compute centered position
        float centerX = -textW * 0.5f; // Horizontal center
        float centerY = 0.3f;          // Move slightly above 0.0f

        AEGfxPrint(volumeFont, "SETTINGS", centerX, centerY, 1.0f, 1, 1, 1, 1);

        // ===== Draw "VOLUME" Title =====
        // Get size of the text in normalized device coordinates
        float vtextW, vtextH;
        AEGfxGetPrintSize(volumeFont, "VOLUME", 1.0f, &vtextW, &vtextH);

        // Compute centered position
        float vcenterX = -vtextW * 0.5f; // Horizontal center
        float vcenterY = 0.0f;          // Move slightly above 0.0f

        AEGfxPrint(volumeFont, "VOLUME", vcenterX, vcenterY, 1.0f, 1, 1, 1, 1);

        // ===== Draw Slider Track =====
        AEMtx33 sliderTrans, sliderScale, sliderMtx;

        AEMtx33Trans(&sliderTrans, 0.0f, sliderY);
        AEMtx33Scale(&sliderScale, sliderWidth, sliderHeight);
        AEMtx33Concat(&sliderMtx, &sliderTrans, &sliderScale);
        AEGfxSetTransform(sliderMtx.m);
        AEGfxMeshDraw(sliderTrackMesh, AE_GFX_MDM_TRIANGLES);

        // ===== Draw Slider Handle =====
        AEMtx33Trans(&sliderTrans, handleX, sliderY);
        AEMtx33Scale(&sliderScale, handleWidth, handleHeight);
        AEMtx33Concat(&sliderMtx, &sliderTrans, &sliderScale);
        AEGfxSetTransform(sliderMtx.m);
        AEGfxMeshDraw(sliderHandleMesh, AE_GFX_MDM_TRIANGLES);

        DrawButton(okButtonMesh, kokX, kokY, "OK", volumeFont, kButtonW, kButtonH, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    void Toggle() {
        sVisible = !sVisible;
    }

    bool IsVisible() {
        return sVisible;
    }

    void Free() {
        if (panelMesh) {
            AEGfxMeshFree(panelMesh);
            panelMesh = nullptr;
        }
        if (sliderTrackMesh) { AEGfxMeshFree(sliderTrackMesh); sliderTrackMesh = nullptr; }
        if (sliderHandleMesh) { AEGfxMeshFree(sliderHandleMesh); sliderHandleMesh = nullptr; }
    }
}
