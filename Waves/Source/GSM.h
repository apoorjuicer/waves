/* Start Header ************************************************************************/
/*!
\file		GSM.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the GameStateManager class, 
            which handles transitions between different game states. 
            It manages the current state, updates the game loop, and ensures 
            proper rendering and cleanup of states, maintaining smooth state transitions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "baseStates.h"

class GameStateManager {
private:
    BaseState* currentState = nullptr;
    int* pGameRunning = nullptr;
public:
    GameStateManager();

    void SetGameRunningPtr(int* ptr) { pGameRunning = ptr; }

    void SetState(BaseState* newState);
    void Update(f32 deltaTime);
    void Draw();
    ~GameStateManager();
};

