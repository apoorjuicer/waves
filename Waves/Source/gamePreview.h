/* Start Header ************************************************************************/
/*!
\file		gamePreview.h
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
#include "AEEngine.h"

class gamePreview : public BaseState {
private:
    bool goGame = false;   // Flag for returning to the main menu
    s8 previewFont;
    AEGfxTexture* maze1 = nullptr;
    AEGfxVertexList* maze1_mesh = nullptr;
    AEGfxVertexList* okButtonMesh = nullptr;
public:
    gamePreview();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
