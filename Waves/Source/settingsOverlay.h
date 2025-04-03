#pragma once

#include "AEEngine.h"
#include "Audio.h"
#include "utility.h"      // For CreateButtonMesh(), etc.
#include <cstdio>         // For sprintf

namespace SettingsOverlay {
    void Init();
    void Update();
    void Draw();
    void Free();
    void Toggle();         // Show/hide the overlay
    bool IsVisible();      // Check if currently visible
}

