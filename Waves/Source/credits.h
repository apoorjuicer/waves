/* Start Header ************************************************************************/
/*!
\file		credits.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the creditState class, which manages the display of game credits.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "mainMenu.h"

class creditState : public BaseState {
private:
    bool goMenu;   // Flag for returning to the main menu
    AEGfxTexture* credit = nullptr;
    AEGfxVertexList* credit_mesh = nullptr;
public:
    creditState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

