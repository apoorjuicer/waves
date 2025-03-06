#include "AEEngine.h"
#include "Rock.h"
#include "Player.h"
#include <algorithm>
#include <cmath>      // for std::floor
#include <iostream>   // for std::cout
#include "sonar.hpp"

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

    bool BounceCollision(float x, float y, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY, float rockRadius) {
        // Convert world-space coordinates to grid indices with half-tile offsets for centering
        int gridX = static_cast<int>(std::floor((x - mazeStartX + (tileSize / 2)) / tileSize));
        int gridY = static_cast<int>(std::floor((mazeStartY - y + (tileSize / 2)) / tileSize));

        // Check for out-of-bounds
        if (gridX < 0 || gridX >= 10 || gridY < 0 || gridY >= 10) {
            return false;
        }

        // If the cell is walkable, no collision
        if (maze[gridY][gridX] == 0 || maze[gridY][gridX] == 2 || maze[gridY][gridX] == 3) {
            return true;
        }

        // Calculate the closest point on the grid cell to the rock's center
        float cellCenterX = mazeStartX + gridX * tileSize;
        float cellCenterY = mazeStartY - gridY * tileSize;

        float closestX = std::max<float>(cellCenterX - tileSize / 2.0f, std::min<float>(x, cellCenterX + tileSize / 2.0f));
        float closestY = std::max<float>(cellCenterY - tileSize / 2.0f, std::min<float>(y, cellCenterY + tileSize / 2.0f));

        // Calculate the distance between the rock's center and the closest point
        float dx = x - closestX;
        float dy = y - closestY;
        float distanceSquared = dx * dx + dy * dy;

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
    bool rockActive = false;

    void drawRock(Player& rplayer, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY, wave_s(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList* mesh, AEGfxVertexList* RockMesh) {
        s32 mouseX = 0, mouseY = 0;
        AEVec2 normalisedVec{ 0.0f, 0.0f };
        AEVec2 mouseWorld{ 0.0f, 0.0f };
        //static AEGfxVertexList* RockMesh = Rock_Mesh(48, 0xFFFFFFFF, 10.0f);

        // Launch the rock when the left mouse button is clicked (and if not already active)
        if (AEInputCheckCurr(AEVK_LBUTTON) && !rockActive) {
            AEInputGetCursorPosition(&mouseX, &mouseY);

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
                rock.dx = normalisedVec.x * 15.0f;
                rock.dy = normalisedVec.y * 15.0f;
                rock.currBounce = 0;
                rockActive = true;
            }
        }

        if (rockActive) {
            // Assume dt is the time elapsed this frame in seconds
            float dt = (float)AEFrameRateControllerGetFrameTime();

            // Base damping factor: 2% reduction per second.
            const float baseDamping = 0.40f;

            // Calculate the frame damping factor.
            float dampingFactor = std::pow(baseDamping, dt);

            // Apply damping to the rock's velocity.
            rock.dx *= dampingFactor;
            rock.dy *= dampingFactor;

            // Optional: Stop the rock completely if the speed is very low.
            const float epsilon = 1.0f;
            if (std::fabs(rock.dx) < epsilon && std::fabs(rock.dy) < epsilon) {
                rock.dx = 0.0f;
                rock.dy = 0.0f;
            }
            //if (std::fabs(rock.dx) < epsilon) {
            //    rock.dx = 0.0f;
            //    rock.dy = 0.0f;
            //}
            //if (std::fabs(rock.dy) < epsilon) {
            //    rock.dy = 0.0f;
            //    rock.dx = 0.0f;
            //}
            float prevX = rock.x;
            float prevY = rock.y;

            // --- Process X Movement ---
            rock.x += rock.dx;
            if (!BounceCollision(rock.x, rock.y, maze, tileSize, mazeStartX, mazeStartY,10.0f)) {
                // If collision on X axis, revert X and reverse the X velocity
                spawn_wave(MainWaves, rock.x, rock.y, mesh);
                rock.x = prevX;
                rock.dx *= static_cast<float>( - 0.4);
                rock.currBounce++;
            }

            // --- Process Y Movement ---
            rock.y += rock.dy;
            if (!BounceCollision(rock.x, rock.y, maze, tileSize, mazeStartX, mazeStartY,10.0f)) {
                // If collision on Y axis, revert Y and reverse the Y velocity
                spawn_wave(MainWaves, rock.x, rock.y, mesh);
                rock.y = prevY;
                rock.dy *= static_cast<float>( - 0.4);
                rock.currBounce++;
            }

            if ((rock.currBounce >= rock.maxBounce) || (rock.dx == 0 && rock.dy == 0)) {
                spawn_wave(MainWaves, rock.x, rock.y, mesh);
                rockActive = false;
            }

            // Draw the rock at its current position
            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, rock.x, rock.y);
            AEGfxSetTransform(transform.m);
            AEGfxMeshDraw(RockMesh, AE_GFX_MDM_TRIANGLES);
        }
    }

}
