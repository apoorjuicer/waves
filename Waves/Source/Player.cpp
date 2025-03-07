#include "AEEngine.h"
#include "Player.h"
#include<iostream>

extern Player player = { 100.0f, -100.0f };

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


bool IsWalkable(float x, float y, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY) {
    // Convert world-space coordinates to grid indices
    int gridX = static_cast<int>((x - mazeStartX + (tileSize/2)) / tileSize);
    int gridY = static_cast<int>(((tileSize/2) - mazeStartY - y) / tileSize); // Flip Y-axis for top-down rendering

    //std::cout << "World Position: (" << x << ", " << y << ")\n";
    //std::cout << "Grid Coordinates: (" << gridX << ", " << gridY << ")\n";

    // Check for out-of-bounds
    if (gridX < 0 || gridX >= 10 || gridY < 0 || gridY >= 10) {
        //std::cout << "Out of Bounds\n";
        return false;
    }

    // Check if the grid cell is walkable
    if (maze[gridY][gridX] == 0 || maze[gridY][gridX] == 3 || maze[gridY][gridX] == 2) {
        //std::cout << "Walkable\n";
        //if (AEInputCheckCurr(AEVK_E) && maze[gridY][gridX] == 3) {
        //    std::cout << maze[gridY][gridX];
        //    maze[gridY][gridX] = 0;
        //    key = 1;
        //}
        return true;
    }
    else {
        //std::cout << "Blocked\n";
        return false;
    }

}

void CheckKey(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY) {
    int gridX = static_cast<int>((player.x - mazeStartX + (tileSize / 2)) / tileSize);
    int gridY = static_cast<int>(((tileSize / 2) - mazeStartY - player.y) / tileSize);

    if (AEInputCheckCurr(AEVK_E) && maze[gridY][gridX] == 3) {
        std::cout << maze[gridY][gridX];
        maze[gridY][gridX] = 0;
        player.key = 1;
    }
}


void MovePlayer(Player& pplayer, float dx, float dy, float speed, float dt, int maze[10][10], float tileSize) {
    //float mazeStartY = 10 * tileSize; // Ensure consistent mazeStartY calculation

    // Calculate the potential new position
    float newX = pplayer.x + dx * speed * dt;
    float newY = pplayer.y + dy * speed * dt; // Ensure y decreases in world space leading increasing index

    // Check if the new position is walkable
    if (IsWalkable(newX, pplayer.y, maze, tileSize, 0.0f, 0)) {
        pplayer.x = newX;
    }
    if (IsWalkable(pplayer.x, newY, maze, tileSize, 0.0f, 0)) {
        pplayer.y = newY;
    }
    //CheckKey(newX, pplayer.y, maze, tileSize, 0.0f, 0, pplayer.key);
}


void RenderPlayer(const Player& pplayer, AEGfxVertexList* playerMesh) {
    AEMtx33 transform = { 0 };
    AEMtx33Trans(&transform, pplayer.x, pplayer.y); // Adjust for screen space
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);
}

//void DrawLives(int Lives, AEGfxVertexList* heartMesh, AEGfxTexture* heartTexture) {
//    float startX = 50.0f;  // X position for the first heart
//    float startY = 700.0f; // Y position for hearts
//    float heartSize = 50.0f; // Size of hearts
//    float spacing = 60.0f; // Space between hearts
//
//    for (int i = 0; i < Lives; i++) {
//        float x = startX + i * spacing;
//
//        // Set texture before drawing
//        AEGfxTextureSet(heartTexture, 0, 0);
//        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//        //AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
//        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//        AEGfxSetTransparency(1.0f);
//
//        // Transform for positioning
//        AEMtx33 transform;
//        AEMtx33Identity(&transform);
//        AEMtx33Scale(&transform, heartSize, heartSize);
//        AEMtx33TransApply(&transform, &transform, x, startY);
//
//        // Apply the transform and draw
//        AEGfxSetTransform(transform.m);
//        AEGfxMeshDraw(heartMesh, AE_GFX_MDM_TRIANGLES);
//    }
//}