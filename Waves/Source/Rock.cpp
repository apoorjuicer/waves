/* Start Header ************************************************************************/
/*!
\file		Rock.cpp
\project    Waves

\author		Guan Shao Jun
\email		shaojun.g@digipen.edu

\brief      Handles the rock mechanics in the game, including rendering, movement, collision detection,
            and interaction with the maze.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Rock.h"

// #include "sonar.hpp"

namespace rock {

    AEGfxVertexList* Rock_Mesh(int NumOfSlices, u32 colour, f32 radius) {
        int slices = NumOfSlices;
        f32 anglestep = 2.0f * PI / slices;

        for (int i = 0; i < slices; ++i) {
            f32 angle1 = i * anglestep;
            f32 angle2 = (i + 1) * anglestep;
            AEGfxTriAdd(
                0.0f, 0.0f, colour, 0.5f, 0.5f,
                radius * cos(angle1), radius * sin(angle1), colour, 0.5f + 0.5f * cos(angle1), 0.5f - 0.5f * sin(angle1),
                radius * cos(angle2), radius * sin(angle2), colour, 0.5f + 0.5f * cos(angle2), 0.5f - 0.5f * sin(angle2)
            );
        }
        return AEGfxMeshEnd();
    }

    bool BounceCollision(f32 x, f32 y, int maze1[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY, f32 rockRadius) {
        // Convert world-space coordinates to grid indices with half-tile offsets for centering
        int gridX = static_cast<int>(std::floor((x - mazeStartX + (tileSize / 2)) / tileSize));
        int gridY = static_cast<int>(std::floor((mazeStartY - y + (tileSize / 2)) / tileSize));

        // Check for out-of-bounds
        if (gridX < 0 || gridX >= MAX_ROW || gridY < 0 || gridY >= MAX_COL) {
            return false;
        }

        // If the cell is walkable, no collision
        if (maze1[gridY][gridX] != 1) {
            return true;
        }


        // Calculate the closest point on the grid cell to the rock's center
        f32 cellCenterX = mazeStartX + gridX * tileSize;
        f32 cellCenterY = mazeStartY - gridY * tileSize;

        f32 closestX = std::max<f32>(cellCenterX - tileSize / 2.0f, std::min<f32>(x, cellCenterX + tileSize / 2.0f));
        f32 closestY = std::max<f32>(cellCenterY - tileSize / 2.0f, std::min<f32>(y, cellCenterY + tileSize / 2.0f));

        // Calculate the distance between the rock's center and the closest point
        f32 dx = x - closestX;
        f32 dy = y - closestY;
        f32 distanceSquared = dx * dx + dy * dy;

        // Check if the distance is less than or equal to the rock's radius
        if (distanceSquared <= rockRadius * rockRadius) {
            // Collision detected
            return false;
        }

        // No collision
        return true;
    }

    // Global rock object
    Rock rock = { 0.0f, 0.0f, 0.0f, 0.0f, 0, 3 };
    //bool rockActive = false;
    f32 rockThrowSoundTimer = 0.0f; // Timer for the rock throw sound
    const f32 rockThrowSoundDuration = 0.3f; // Duration to play the sound (in seconds)

    void drawRock(Player& rplayer, int maze1[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, AEGfxVertexList* rockMesh, AEGfxTexture* rockTexture) {
        f32 rockRad = 10.0f;
        s32 mouseX = 0, mouseY = 0;
        AEVec2 normalisedVec{ 0.0f, 0.0f };
        AEVec2 mouseWorld{ 0.0f, 0.0f };
        //AEGfxVertexList* RockMesh = Rock_Mesh(48, 0xFFFFFFFF, 10.0f);

        // Check if the player has a rock in their inventory
        bool hasRock = false;
        int rockIndex = -1;

        for (int i = 0; i < INVENTORY_SIZE; ++i) {
            if (rplayer.inventory[i] == ROCK) {
                hasRock = true;
                rockIndex = i;  // Store the index of the first found rock
                break;
            }
        }

        // Launch the rock when the left mouse button is clicked (and if not already active)
        if (AEInputCheckCurr(AEVK_RBUTTON) && !rplayer.rockActive && hasRock) {

          
            f32 speed = 15.0f;
            AEInputGetCursorPosition(&mouseX, &mouseY);

            // Play the rock throw sound
            AudioManager::PlayAudio("Rock Throw", 0.3f, 1.0f, 0);
            rockThrowSoundTimer = 0.0f; // Reset the timer

            /*rplayer.inventory[rockIndex] = NOTHING;*/

            // Get screen dimensions
            f32 screenWidth = static_cast<f32>(AEGfxGetWindowWidth());
            f32 screenHeight = static_cast<f32>(AEGfxGetWindowHeight());

            // Convert to world coordinates (taking camera/player position into account)
            mouseWorld.x = (static_cast<f32>(mouseX) - screenWidth / 2) + rplayer.x;
            mouseWorld.y = (screenHeight / 2 - static_cast<f32>(mouseY)) + rplayer.y;

            AEVec2 direction = { mouseWorld.x - rplayer.x, mouseWorld.y - rplayer.y };

            if (AEVec2Length(&direction) > 0.1f) {
                AEVec2Normalize(&normalisedVec, &direction);

                rock.x = rplayer.x;
                rock.y = rplayer.y;

                // Set the rock velocity (adjust the multiplier as needed)
                rock.dx = normalisedVec.x * speed;
                rock.dy = normalisedVec.y * speed;
                rock.currBounce = 0;
                rplayer.rockActive = true;
            }
        }

        if (rplayer.rockActive) {
            // Assume dt is the time elapsed this frame in seconds
            f32 dt = (f32)AEFrameRateControllerGetFrameTime();

            // Update the timer
            rockThrowSoundTimer += dt;

            // Stop the sound if the timer exceeds the duration
            if (rockThrowSoundTimer >= rockThrowSoundDuration) {
                AudioManager::StopAudio("Rock Throw");
            }

            
            const f32 baseDamping = 0.40f;

            // Calculate the frame damping factor.
            f32 dampingFactor = std::pow(baseDamping, dt);

            // Apply damping to the rock's velocity.
            rock.dx *= dampingFactor;
            rock.dy *= dampingFactor;

            // Optional: Stop the rock completely if the speed is very low.
            const f32 epsilon = 1.0f;
            if (std::fabs(rock.dx) < epsilon && std::fabs(rock.dy) < epsilon) {
                rock.dx = 0.0f;
                rock.dy = 0.0f;
            }
            f32 prevX = rock.x;
            f32 prevY = rock.y;

            // --- Process X Movement ---
            rock.x += rock.dx;
            if (!BounceCollision(rock.x, rock.y, maze1, tileSize, mazeStartX, mazeStartY, rockRad)) {
                // If collision on X axis, revert X and reverse the X velocity
                spawn_wave(MainWaves, prevX, prevY, mesh);
                rock.x = prevX;
                rock.dx *= ( - 0.4f);
                rock.currBounce++;

                // Play the collision sound
                AudioManager::PlayAudio("Collide", 0.4f, 1.0f, 0);
            }

            // --- Process Y Movement ---
            rock.y += rock.dy;
            if (!BounceCollision(rock.x, rock.y, maze1, tileSize, mazeStartX, mazeStartY, rockRad)) {
                // If collision on Y axis, revert Y and reverse the Y velocity
                spawn_wave(MainWaves, prevX, prevY, mesh);
                rock.y = prevY;
                rock.dy *= ( - 0.4f);
                rock.currBounce++;

                // Play the collision sound
                AudioManager::PlayAudio("Collide", 0.4f, 1.0f, 0);
            }

            if ((rock.currBounce >= rock.maxBounce) || (rock.dx == 0 && rock.dy == 0)) {
                spawn_wave(MainWaves, prevX, prevY, mesh);
                rplayer.rockActive = false;
                rplayer.lastRockPos.x = rock.x;
                rplayer.lastRockPos.y = rock.y;
                rplayer.rockLanded = true;
  
                rplayer.DropItem(rock.x, rock.y);
                
            }

            // Draw the rock at its current position
            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, rock.x, rock.y);
            AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
            AEGfxSetTransform(transform.m);
            AEGfxTextureSet(rockTexture, 0, 0);
            AEGfxMeshDraw(rockMesh, AE_GFX_MDM_TRIANGLES);
            AEGfxTextureSet(nullptr, 0, 0);
        }
    }

}
