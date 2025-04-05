#pragma once

#include "pch.h"
#include "Audio.h"

namespace SettingsOverlay {
    void Init();
    void Update();
    void Draw();
    void Free();
    void Toggle();         // Show/hide the overlay
    bool IsVisible();      // Check if currently visible
}

