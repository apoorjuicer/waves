/* Start Header ************************************************************************/
/*!
\file		game2.h
\author		Henry Hu
\co-author  Jordain Ng, Benjamin Ban, Chewn Thing Kwan, Guan Shao Jun
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the GameState class, which manages the core gameplay logic and rendering.
            It handles game state transitions, updates,
            and rendering of various game elements such as the player, enemies, maze, and interactive objects.
            The class integrates multiple components to create a cohesive gameplay experience.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include "Rock.h"
#include "Wave.h"
#include "Enemy.h"
#include "mainMenu.h"
#include "winState.h"
#include "loseState.h"
#include "pauseOverlay.h"
#include "Audio.h"

class GameState2 : public BaseState {
private:

public:
    GameState2();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

