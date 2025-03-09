/* Start Header ************************************************************************/
/*!
\file		baseStates.h
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

class BaseState {
public:
    virtual ~BaseState() {}

    virtual void Load() = 0;        // Load assets
    virtual void Initialize() = 0;  // Setup initial state data
    virtual void Update(float deltaTime) = 0; // Handle logic updates
    virtual void Draw() = 0;        // Render game objects
    virtual void Free() = 0;        // Free runtime resources
    virtual void Unload() = 0;      // Unload assets
    virtual BaseState* GetNextState() = 0; // Return the next state
};

