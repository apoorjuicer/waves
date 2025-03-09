/* Start Header ************************************************************************/
/*!
\file		GSM.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief

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

