#pragma once

#include "pch.h"

namespace ExitConfirmOverlay {
    void Init();
    void Update(int* pGameRunning);
    void Draw();
    void Free();

    void Toggle();
    bool IsVisible();
}

