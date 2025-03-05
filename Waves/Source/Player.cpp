#include "AEEngine.h"
#include "Player.h"
#include "maze.h"
#include<iostream>

extern Player player = { 1.0f, -1.0f }; //starting point

void InitializePlayer(Player& player, f32 tileSize) {
    // Get the current window dimensions
    f32 screenWidth = static_cast<f32>(AEGfxGetWindowWidth());
    f32 screenHeight = static_cast<f32>(AEGfxGetWindowHeight());

    // Calculate the starting position to center the maze
    f32 mazeWidth = 10.0f * tileSize;  // Total width of the maze
    f32 mazeHeight = 10.0f * tileSize; // Total height of the maze
    f32 startX = (screenWidth - mazeWidth) / 2.0f; // Center horizontally
    f32 startY = (screenHeight - mazeHeight) / 2.0f; // Center vertically (top-down)

    // Convert player's grid coordinates to screen coordinates
    player.x = startX + (player.x * tileSize);
    player.y = startY + (player.y * tileSize); // Y increases as you move down
}

// Function to create a donut mesh with an empty center
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color)
{
    // Start creating the mesh
    AEGfxMeshStart();

    // Loop through the segments to define vertices
    for (int i = 0; i < numSegments; ++i)
    {
        // Calculate angles for the current and next segments

        f32 theta1 = (2.0f * PI * i) / numSegments;
        f32 theta2 = (2.0f * PI * (i + 1)) / numSegments;

        // Outer ring vertices
        f32 x1_outer = outerRadius * cosf(theta1);
        f32 y1_outer = outerRadius * sinf(theta1);
        f32 x2_outer = outerRadius * cosf(theta2);
        f32 y2_outer = outerRadius * sinf(theta2);

        // Inner ring vertices
        f32 x1_inner = innerRadius * cosf(theta1);
        f32 y1_inner = innerRadius * sinf(theta1);
        f32 x2_inner = innerRadius * cosf(theta2);
        f32 y2_inner = innerRadius * sinf(theta2);

        // Add two triangles to form a quad between the inner and outer rings
        AEGfxTriAdd(
            x1_inner, y1_inner, color, 0.0f, 1.0f,  // Inner vertex 1
            x2_outer, y2_outer, color, 1.0f, 0.0f,  // Outer vertex 2
            x1_outer, y1_outer, color, 0.0f, 0.0f   // Outer vertex 1
        );

        AEGfxTriAdd(
            x1_inner, y1_inner, color, 0.0f, 1.0f,  // Inner vertex 1
            x2_inner, y2_inner, color, 1.0f, 1.0f,  // Inner vertex 2
            x2_outer, y2_outer, color, 1.0f, 0.0f   // Outer vertex 2
        );
    }

    // End the mesh and return it
    return AEGfxMeshEnd();
}

void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation)
{
    // Create transform matrices
    AEMtx33 scaleDonut = { 0 };
    AEMtx33 rotateDonut = { 0 };
    AEMtx33 translateDonut = { 0 };

    // Apply scaling, rotation, and translation
    AEMtx33Scale(&scaleDonut, scaleX, scaleY);           // Scaling
    AEMtx33Rot(&rotateDonut, rotation);                  // Rotation
    AEMtx33Trans(&translateDonut, posX, posY);           // Translation

    // Concatenate the matrices in the correct order (scale, then rotate, then translate)
    AEMtx33 transformDonut = { 0 };
    AEMtx33Concat(&transformDonut, &rotateDonut, &scaleDonut);
    AEMtx33Concat(&transformDonut, &translateDonut, &transformDonut);

    // Apply the transform and draw the mesh
    AEGfxSetTransform(transformDonut.m);
    AEGfxMeshDraw(NameOfMesh, AE_GFX_MDM_TRIANGLES);
}

AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour, f32 radius)
{
    int slices = NumOfSlices;
    f32 anglestep = 2.0f * PI / slices;
    float centerX = 0.0f;
    float centerY = 0.0f;

    for (int i = 0; i < slices; ++i)
    {
        f32 angle1 = i * anglestep;
        f32 angle2 = (i + 1) * anglestep;
        AEGfxTriAdd(
            centerX, centerY, colour, 0.5f, 0.5f,
            centerX + radius * cos(angle1), centerY + radius * sin(angle1), colour, 0.5f + 0.5f * cos(angle1), 0.5f - 0.5f * sin(angle1),
            centerX + radius * cos(angle2), centerY + radius * sin(angle2), colour, 0.5f + 0.5f * cos(angle2), 0.5f - 0.5f * sin(angle2)
        );
    }
    return AEGfxMeshEnd();
}

bool IsWalkable(float x, float y, int maze[maze_size][maze_size], float tileSize, float mazeStartX, float mazeStartY) {
    // Convert world-space coordinates to grid indices
    int gridX = static_cast<int>((x - mazeStartX + (tileSize/2)) / tileSize);
    int gridY = static_cast<int>(((tileSize/2) - mazeStartY - y) / tileSize); // Flip Y-axis for top-down rendering

    std::cout << "World Position: (" << x << ", " << y << ")\n";
    std::cout << "Grid Coordinates: (" << gridX << ", " << gridY << ")\n";

    // Check for out-of-bounds
    if (gridX < 0 || gridX >= 10 || gridY < 0 || gridY >= 10) {
        std::cout << "Out of Bounds\n";
        return false;
    }

    // Check if the grid cell is walkable
    if (maze[gridY][gridX] == 0) {
        std::cout << "Walkable\n";
        return true;
    }
    else {
        std::cout << "Blocked\n";
        return false;
    }
}


void MovePlayer(Player& player, float dx, float dy, float speed, float dt, int maze[maze_size][maze_size], float tileSize) {
    float mazeStartY = 10 * tileSize; // Ensure consistent mazeStartY calculation

    // Calculate the potential new position
    float newX = player.x + dx * speed * dt;
    float newY = player.y + dy * speed * dt; // Ensure y decreases in world space leading increasing index

    // Check if the new position is walkable
    if (IsWalkable(newX, player.y, maze, tileSize, 0.0f, 0)) {
        player.x = newX;
    }
    if (IsWalkable(player.x, newY, maze, tileSize, 0.0f, 0)) {
        player.y = newY;
    }
}


void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh) {
    AEMtx33 transform = { 0 };
    AEMtx33Trans(&transform, player.x, player.y); // Adjust for screen space
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);
}