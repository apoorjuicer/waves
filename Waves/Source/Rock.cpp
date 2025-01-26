#include "AEEngine.h"
#include "Rock.h"
#include "Player.h"

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

    bool BounceCollision(float x, float y, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY) {
        // Convert world-space coordinates to grid indices
        int gridX = static_cast<int>((x - mazeStartX + (tileSize / 2)) / tileSize);
        //int gridY = static_cast<int>(((tileSize / 2) - mazeStartY - y) / tileSize); // Flip Y-axis for top-down rendering
        int gridY = static_cast<int>((mazeStartY - y) / tileSize);

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

    // Global rock object
    Rock rock = { 0.0f, 0.0f, 0.0f, 0.0f, 0, 3 };
    bool rockActive = false;

    void drawRock(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY) {
        s32 mouseX = 0, mouseY = 0;
        AEVec2 normalisedVec{ 0.0f, 0.0f };
        AEVec2 mouseWorld{ 0.0f, 0.0f };
        static AEGfxVertexList* RockMesh = Rock_Mesh(48, 0xFF000000, 10.0f);

        if (AEInputCheckCurr(AEVK_LBUTTON) && !rockActive) {
            AEInputGetCursorPosition(&mouseX, &mouseY);

            // Get proper screen dimensions
            f32 screenWidth = static_cast<f32>(AEGfxGetWindowWidth());  // Changed from AEGfxGetWindowWidth()
            f32 screenHeight = static_cast<f32>(AEGfxGetWindowHeight()); // Changed from AEGfxGetWindowHeight()

            // Convert to world coordinates with camera offset
            mouseWorld.x = (static_cast<f32>(mouseX) - screenWidth / 2) + player.x;  // Add camera X
            mouseWorld.y = (screenHeight / 2 - static_cast<f32>(mouseY)) + player.y; // Add camera Y

            AEVec2 direction = { mouseWorld.x - player.x, mouseWorld.y - player.y };

            if (AEVec2Length(&direction) > 0.1f) {
                AEVec2Normalize(&normalisedVec, &direction);

                rock.x = player.x;
                rock.y = player.y;

                // Adjust speed as needed
                rock.dx = normalisedVec.x * 15.0f;
                rock.dy = normalisedVec.y * 15.0f;
                rock.currBounce = 0;
                rockActive = true;
            }
        }

        if (rockActive) {
            rock.x += rock.dx;
            rock.y += rock.dy;

            if (!BounceCollision(rock.x, rock.y, maze, tileSize, mazeStartX, mazeStartY)) {
                rock.dx = -rock.dx;
                rock.dy = -rock.dy;
                rock.currBounce++;

                if (rock.currBounce >= rock.maxBounce) {
                    rockActive = false;
                }
            }

            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, rock.x, rock.y); // Adjust for screen space
            AEGfxSetTransform(transform.m);
            AEGfxMeshDraw(RockMesh, AE_GFX_MDM_TRIANGLES);
        }
    }

}