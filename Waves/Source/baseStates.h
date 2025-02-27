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

