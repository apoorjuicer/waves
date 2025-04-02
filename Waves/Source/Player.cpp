/* Start Header ************************************************************************/
/*!
\file		Player.cpp
\author		Jordain Ng
\co-author  Guan Shao Jun
\email		jordainyingzhe.ng@digipen.edu

\brief      this file includes functions, to for player movement and collision checking, to 
            prevent the player from going into the wall

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include<iostream>
#include "Audio.h"

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


bool IsWalkable(f32 x, f32 y, int maze1[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY) {
    // Define the radius (world units) that must be walkable
    const f32 radius = 7.0f;

    // Compute the bounding box of the circle centered at (x,y)
    f32 minWorldX = x - radius;
    f32 maxWorldX = x + radius;
    f32 minWorldY = y - radius;
    f32 maxWorldY = y + radius;

    // Lambda functions to convert a world coordinate to a grid index.
    auto convertX = [tileSize, mazeStartX](f32 worldX) -> int {
        return static_cast<int>((worldX - mazeStartX + (tileSize / 2)) / tileSize);
        };
    auto convertY = [tileSize, mazeStartY](f32 worldY) -> int {
        return static_cast<int>(((tileSize / 2) - mazeStartY - worldY) / tileSize);
        };

    // Convert the four edges of the bounding box to grid indices.
    int gridX1 = convertX(minWorldX);
    int gridX2 = convertX(maxWorldX);
    int gridY1 = convertY(minWorldY);
    int gridY2 = convertY(maxWorldY);

    // Determine the min and max grid indices (in case conversion inverts order).
    int minGridX = (gridX1 < gridX2) ? gridX1 : gridX2;
    int maxGridX = (gridX1 > gridX2) ? gridX1 : gridX2;
    int minGridY = (gridY1 < gridY2) ? gridY1 : gridY2;
    int maxGridY = (gridY1 > gridY2) ? gridY1 : gridY2;

    // Check if any part of the bounding box is out-of-bounds.
    if (minGridX < 0 || maxGridX >= MAX_ROW || minGridY < 0 || maxGridY >= MAX_COL) {
        return false;
    }

    // Check every grid cell in the bounding box.
    // (A more refined test might check only cells actually intersecting the circle.)
    for (int gy = minGridY; gy <= maxGridY; ++gy) {
        for (int gx = minGridX; gx <= maxGridX; ++gx) {
            if (maze1[gy][gx] == 1) { // assuming 1 means blocked
                return false;
            }
        }
    }
    return true;
}


void CheckItem(Player& player1, int maze1[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY) {
    UNREFERENCED_PARAMETER(tileSize);
    UNREFERENCED_PARAMETER(mazeStartX);
    UNREFERENCED_PARAMETER(mazeStartY);
    UNREFERENCED_PARAMETER(maze1);

    if (AEInputCheckTriggered(AEVK_E)) {
        f32 playerMinX = player1.x - player1.rad;
        f32 playerMaxX = player1.x + player1.rad;
        f32 playerMinY = player1.y - player1.rad;
        f32 playerMaxY = player1.y + player1.rad;

        for (auto& item : worldItems) {
            if (!item.collected &&
                item.x + player1.rad >= playerMinX && item.x - player1.rad <= playerMaxX &&
                item.y + player1.rad >= playerMinY && item.y - player1.rad <= playerMaxY) {

                AudioManager::PlayAudio("Collect", 0.2f, 1.0f, 0);

                // Pick up the item (add it to the player's inventory)
                player1.PickUpItem(item.type);

                // Mark the item as collected
                item.collected = true;
                break;
            }
        }
    }
}

void Player::PickUpItem(InventoryItemType itemType) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (inventory[i] == NOTHING) {
            inventory[i] = itemType;
            return;
        }
    }
}

void Player::DropItem(f32 dropX, f32 dropY) {
    for (int i = {INVENTORY_SIZE -1 }; i >=0; --i) {
        if (inventory[i] == ROCK) {
            // Drop the item and add it to worldItems
            worldItems.emplace_back(WorldItem(inventory[i], dropX, dropY));
            inventory[i] = NOTHING;  // Clear the slot
            return;
        }
    }
}




void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze1[MAX_ROW][MAX_COL], f32 tileSize) {
    //f32 mazeStartY = 10 * tileSize; // Ensure consistent mazeStartY calculation

    // Calculate the potential new position
    f32 newX = pplayer.x + dx * speed * dt;
    f32 newY = pplayer.y + dy * speed * dt; // Ensure y decreases in world space leading increasing index

    // Check if the new position is walkable
    if (IsWalkable(newX, pplayer.y, maze1, tileSize, 0.0f, 0)) {
        pplayer.x = newX;
    }
    if (IsWalkable(pplayer.x, newY, maze1, tileSize, 0.0f, 0)) {
        pplayer.y = newY;
    }
}

AEGfxVertexList* CreateBoxMesh(u32 color) {
    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, color, 0.0f, 1.0f,  // bottom-left
        0.5f, -0.5f, color, 1.0f, 1.0f,  // bottom-right
        -0.5f, 0.5f, color, 0.0f, 0.0f   // top-left
    );
    AEGfxTriAdd(
        0.5f, -0.5f, color, 1.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, color, 1.0f, 0.0f,  // top-right
        -0.5f, 0.5f, color, 0.0f, 0.0f   // top-left
    );
    return AEGfxMeshEnd();
}

void DrawInventory(const Player& player1, AEGfxVertexList* boxMesh,
    AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh,
    AEGfxTexture* keyTexture, AEGfxTexture* rockTexture) {

    f32 worldScreenLeft = player1.x - (AEGfxGetWindowWidth() / 2);
    f32 worldScreenBottom = player1.y - (AEGfxGetWindowHeight() / 2);

    f32 offsetX = 100.0f;
    f32 offsetY = 100.0f;
    f32 boxSize = 80.0f;
    f32 spacing = 20.0f;

    for (int i = INVENTORY_SIZE - 1; i >= 0; --i) {
        f32 x = worldScreenLeft + offsetX + i * (boxSize + spacing);
        f32 y = worldScreenBottom + offsetY;

        // Draw inventory slot (box) - No Texture
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEMtx33 boxTransform;
        AEMtx33Identity(&boxTransform);
        AEMtx33Scale(&boxTransform, boxSize, boxSize);
        AEMtx33TransApply(&boxTransform, &boxTransform, x, y);
        AEGfxSetTransform(boxTransform.m);
        AEGfxMeshDraw(boxMesh, AE_GFX_MDM_TRIANGLES);

        //  Draw inventory item (if exists)
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Enable transparency

        AEGfxVertexList* itemMesh = nullptr;
        AEGfxTexture* itemTexture = nullptr;

        if (player1.inventory[i] == KEY) {
            itemMesh = keyMesh;
            itemTexture = keyTexture;
        }
        else if (player1.inventory[i] == ROCK) {
            itemMesh = rockMesh;
            itemTexture = rockTexture;
        }

        if (itemMesh && itemTexture) {
            AEGfxTextureSet(itemTexture, 0, 0);

            AEMtx33 itemTransform;
            AEMtx33Identity(&itemTransform);
            //AEMtx33Scale(&itemTransform, boxSize * 0.8f, boxSize * 0.8f);
            AEMtx33TransApply(&itemTransform, &itemTransform, x, y);
            AEGfxSetTransform(itemTransform.m);
            AEGfxMeshDraw(itemMesh, AE_GFX_MDM_TRIANGLES);
        }
    }
        AEGfxTextureSet(nullptr, 0, 0);
}



 
bool PlayerEnemyCollision(Player& player1, Enemy& enemy, f32 tilesize) {
    // Assuming enemy.x, enemy.y is the center of the enemy rectangle.
    f32 halfWidth = tilesize * 0.5f;
    f32 halfHeight = tilesize * 0.5f;
    /*f32 halfWidth = enemy.w * 0.5f;
    f32 halfHeight = enemy.h * 0.5f;*/

    // Calculate the bounds of the enemy rectangle.
    f32 left = enemy.x - halfWidth;
    f32 right = enemy.x + halfWidth;
    f32 top = enemy.y - halfHeight;
    f32 bottom = enemy.y + halfHeight;

    // Clamp the player's center to the enemy's bounds.
    f32 closestX = (player1.x < left) ? left : (player1.x > right ? right : player1.x);
    f32 closestY = (player1.y < top) ? top : (player1.y > bottom ? bottom : player1.y);

    // Compute the distance between the player's center and the closest point.
    f32 dx = player1.x - closestX;
    f32 dy = player1.y - closestY;
    f32 distanceSquared = dx * dx + dy * dy;

    // A collision occurs if the distance is less than or equal to the player's radius.
    return distanceSquared <= (player1.rad * player1.rad);
};

bool checkGoal(Player& player1, int maze1[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY) {
    // Convert player position to grid indices
    int gridX = static_cast<int>((player1.x - mazeStartX + (tileSize / 2)) / tileSize);
    int gridY = static_cast<int>((mazeStartY + (tileSize / 2) - player1.y) / tileSize);

    // Ensure indices are within bounds before accessing the maze array
    if (gridX < 0 || gridX >= MAX_COL || gridY < 0 || gridY >= MAX_ROW) {
        return false; // Out of bounds = Not on a goal tile
    }

    bool hasKey = false;
    for (int i{}; i < INVENTORY_SIZE; ++i) {
        if (player1.inventory[i] == KEY) {
            hasKey = true;
            break;
        }
    }

    // Check if the player has the key and is standing on the goal tile (maze value == 2)
    return (hasKey && maze1[gridY][gridX] == 2 && AEInputCheckCurr(AEVK_E));
}



void RenderPlayer(const Player& pplayer, AEGfxVertexList* playerMesh) {
    AEMtx33 transform = { 0 };
    AEMtx33Trans(&transform, pplayer.x, pplayer.y); // Adjust for screen space
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);
}