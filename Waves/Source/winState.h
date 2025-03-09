/* Start Header ************************************************************************/
/*!
\file		winState.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the WinState class, which manages the game's victory screen. 
            It handles rendering the win message, providing options to 
            proceed to the next phase or return to the main menu, and managing state transitions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "baseStates.h"
#include "AEEngine.h"

class WinState : public BaseState {
private:
    bool goNext;       // Flag to trigger transition
    s8 winFont;
public:
    WinState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
