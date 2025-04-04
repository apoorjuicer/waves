/* Start Header ************************************************************************/
/*!
\file		pauseOverlay.h
\author		Henry Hu
\email		zhihenghenrey.hu@digipen.edu
\brief

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"

namespace PauseOverlay {
    void Init();
    void Update();
    void Draw();
    void Free();
    bool IsPaused();
    void SetPaused(bool pause);
    bool ShouldExitToMenu();
}

extern AEVec2 GlobalCenter;

