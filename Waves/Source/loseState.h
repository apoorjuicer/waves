/* Start Header ************************************************************************/
/*!
\file		loseState.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the LoseState class, which manages the game-over screen. 
            It handles rendering the loss message, providing options to return to the main menu, 
            and managing transitions to other game states.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "mainMenu.h"
#include "Audio.h"

class LoseState : public BaseState {
private:
    bool goMenu;   // Flag for returning to the main menu
    s8 loseFont;
    //bool retryGame; Button to go back to the game
public:
    LoseState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
