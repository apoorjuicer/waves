/* Start Header ************************************************************************/
/*!
\file		baseStates.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the BaseState class, an abstract base class for managing game states in a state machine. 
            It provides a standardized interface for loading, initializing, updating, rendering, 
            and unloading game states, ensuring a structured transition between different game phases.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "pch.h"

// ========================================
// BaseState class for managing game states
// ========================================
class BaseState {
public:
	virtual ~BaseState() {}                 // Destructor

    virtual void Load() = 0;                // Load assets
    virtual void Initialize() = 0;          // Setup initial state data
    virtual void Update(f32 deltaTime) = 0; // Handle logic updates
    virtual void Draw() = 0;                // Render game objects
    virtual void Free() = 0;                // Free runtime resources
    virtual void Unload() = 0;              // Unload assets
    virtual BaseState* GetNextState() = 0;  // Return the next state
};
