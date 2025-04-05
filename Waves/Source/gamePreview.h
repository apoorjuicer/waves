/* Start Header ************************************************************************/
/*!
\file		gamePreview.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the gamePreview class, which represents a preview state before entering the main game. 
            It manages the loading, rendering, and interactions within the preview, 
            including displaying a maze preview and an "OK" button to proceed.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "utility.h"
#include "mainMenu.h"
#include "game1.h"
#include "Player.h"

class gamePreview : public BaseState {
private:
    bool goGame = false;   // Flag for returning to the main menu
    s8 previewFont;
    AEGfxVertexList* maze1_mesh = nullptr;
    AEGfxVertexList* okButtonMesh = nullptr;

    int level_num;
public:
    gamePreview(int i);
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
