#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include<iostream>

extern Player player = { 100.0f, -100.0f };
extern Player lastRock = { 0.0f, 0.0f };

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
    f32 centerX = 0.0f;
    f32 centerY = 0.0f;

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


bool IsWalkable(f32 x, f32 y, int maze[ROW][COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY) {
    // Convert world-space coordinates to grid indices
    int gridX = static_cast<int>((x - mazeStartX + (tileSize/2)) / tileSize);
    int gridY = static_cast<int>(((tileSize/2) - mazeStartY - y) / tileSize); // Flip Y-axis for top-down rendering

    //std::cout << "World Position: (" << x << ", " << y << ")\n";
    //std::cout << "Grid Coordinates: (" << gridX << ", " << gridY << ")\n";

    // Check for out-of-bounds
    if (gridX < 0 || gridX >= ROW || gridY < 0 || gridY >= COL) {
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


void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze[ROW][COL], f32 tileSize) {
    //f32 mazeStartY = 10 * tileSize; // Ensure consistent mazeStartY calculation

    // Calculate the potential new position
    f32 newX = pplayer.x + dx * speed * dt;
    f32 newY = pplayer.y + dy * speed * dt; // Ensure y decreases in world space leading increasing index

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
//    std::cout << "DrawLives called with Lives = " << Lives << std::endl;
//    // Get camera position to adjust UI rendering
//    float camX, camY;
//    AEGfxGetCamPosition(&camX, &camY);
//
//    // Positions relative to screen (e.g., top-left corner)
//    float startX = 50.0f;
//    float startY = 50.0f; // Top-left instead of 700.0f
//    float heartSize = 50.0f;
//    float spacing = 60.0f;
//
//    for (int i = 0; i < Lives; i++) {
//        std::cout << "Drawing heart " << i << std::endl;
//        // Convert screen position to world space using camera offset
//        float x = startX + i * spacing + camX - AEGfxGetWindowWidth() / 2.0f;
//        float y = startY + camY + AEGfxGetWindowHeight() / 2.0f;
//
//        AEGfxTextureSet(heartTexture, 0, 0);
//        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//        /*AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);*/
//        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//        AEGfxSetTransparency(1.0f);
//
//        AEMtx33 transform;
//        AEMtx33Identity(&transform);
//        AEMtx33Scale(&transform, heartSize, heartSize);
//        AEMtx33TransApply(&transform, &transform, x, y); // Use adjusted x, y
//
//        // Debug output
//        std::cout << "Transform Matrix:\n";
//        std::cout << transform.m[0][0] << " " << transform.m[0][1] << " " << transform.m[0][2] << "\n";
//        std::cout << transform.m[1][0] << " " << transform.m[1][1] << " " << transform.m[1][2] << "\n";
//        std::cout << transform.m[2][0] << " " << transform.m[2][1] << " " << transform.m[2][2] << "\n";
//
//        AEGfxSetTransform(transform.m);
//        AEGfxMeshDraw(heartMesh, AE_GFX_MDM_TRIANGLES);
//    }
//}