/* Start Header ************************************************************************/
/*!
\file		instructions.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the instructionState class, which manages the display of game instructions. 
            It handles loading, rendering, and user interactions, 
            allowing players to view gameplay guidelines before returning to the main menu.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "baseStates.h"
#include "AEEngine.h"

class instructionState : public BaseState {
private:
    bool goMenu;   // Flag for returning to the main menu
public:
    instructionState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
