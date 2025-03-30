/* Start Header ************************************************************************/
/*!
\file		Maze.h
\author		Jordain Ng
\co-author  Guan Shao Jun
\email		jordainyingzhe.ng@digipen.edu

\brief      This folder includes functions, to help with drawing of the maze, with a 2d array

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include "Maze.h"
#include "Player.h"

int activeRow = 10; // initial active rows
int activeCol = 10; // initial active columns

int maze[MAX_ROW][MAX_COL] =
{
    /*{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 1, 0, 2, 1 },
    { 1, 0, 1, 1, 1, 0, 1, 0, 1, 1 },
    { 1, 3, 4, 0, 1, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
    { 1, 0, 1, 1, 1, 1, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }*/
};

bool ImportMapDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line, label;

    // Read row count from first line (e.g., "ROW: 10")
    if (!std::getline(file, line)) {
        std::cerr << "Error: Failed to read row info from file " << filename << "\n";
        return false;
    }
    std::istringstream issRow(line);
    if (!(issRow >> label >> activeRow)) {
        std::cerr << "Error: Failed to parse row info from file " << filename << "\n";
        return false;
    }

    // Read column count from second line (e.g., "COL: 10")
    if (!std::getline(file, line)) {
        std::cerr << "Error: Failed to read column info from file " << filename << "\n";
        return false;
    }
    std::istringstream issCol(line);
    if (!(issCol >> label >> activeCol)) {
        std::cerr << "Error: Failed to parse column info from file " << filename << "\n";
        return false;
    }

    // Validate dimensions against MAX_ROW and MAX_COL.
    if (activeRow <= 0 || activeRow > MAX_ROW || activeCol <= 0 || activeCol > MAX_COL) {
        std::cerr << "Error: Maze dimensions (" << activeRow << "x" << activeCol
            << ") out of allowed range (" << MAX_ROW << "x" << MAX_COL << ")\n";
        return false;
    }

    // Read maze data row by row.
    for (int i = 0; i < activeRow; ++i) {
        if (!std::getline(file, line)) {
            std::cerr << "Error: Not enough rows in maze file " << filename << "\n";
            return false;
        }
        std::istringstream iss(line);
        for (int j = 0; j < activeCol; ++j) {
            if (!(iss >> maze[i][j])) {
                std::cerr << "Error: Failed to read maze data at row " << i << ", col " << j << "\n";
                return false;
            }
        }
    }
    return true;
}

//void Import_MapData(void) {
//    // Reset active maze values if desired.
//    activeRow = 0;
//    activeCol = 0;
//
//    std::string filePath;
//    // Example decision based on game state.
//    // (Assume you have an enum or similar defined for your game states;
//    // here we use a sample condition based on your GSM-managed current state.)
//    // For instance, if your current state is of type GameState (platform level):
//    //    if (dynamic_cast<GameState*>(gsm.currentState) != nullptr)
//    // For this sample, we'll choose Maze1.
//    filePath = "Assets/Maze2.txt";
//
//    // Import maze data. If it fails, log an error and update the GSM to quit.
//    if (!ImportMapDataFromFile(filePath)) {
//        std::cerr << "Error: Failed to load map file " << filePath << ". Quitting game.\n";
//        // Assuming your GSM handles transitions, you might set the next state to a quit state:
//        // e.g., gsm.SetState(new QuitState());
//    }
//}

void createWorldItems(f32 tileSize) {
    float startY = tileSize; // Y starts at the top (10 tiles * tile size)


    std::cout << "Rendering Maze and World Items:" << std::endl;
    for (int y{};y < MAX_ROW;++y) {
        for (int x{}; x < MAX_COL; ++x) {
            int tileValue = maze[y][x];
            f32 worldX = x * tileSize;
            f32 worldY = startY - ((y + 1) * tileSize);

            if (tileValue == 3) {
                worldItems.emplace_back(WorldItem(KEY, worldX, worldY));
                std::cout << "WorldItem KEY at (" << worldX << ", " << worldY << ")" << std::endl;
            }
            else if (tileValue == 4) {
                worldItems.emplace_back(WorldItem(ROCK, worldX, worldY));
                std::cout << "WorldItem ROCK at (" << worldX << ", " << worldY << ")" << std::endl;
            }
        }
    }
}

void RenderMaze(int maze1[MAX_ROW][MAX_COL], f32 tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh, AEGfxVertexList* goalMesh, AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh) {
    (void)pathMesh;
    // Calculate the top-left corner of the maze
    float startX = 0.0f; // X starts at 0
    float startY = tileSize; // Y starts at the top (10 tiles * tile size)

    //for (int i = 0; i < INVENTORY_SIZE; ++i) {
    //    std::cout << player.inventory[i] << " ";  // Output each item type in the inventory
    //}
    std::cout << std::endl;

    for (int y = 0; y < MAX_ROW; ++y) {
        for (int x = 0; x < MAX_COL; ++x) {
            // Calculate the position of each tile
            f32 posX = startX + (x * tileSize);
            f32 posY = startY - ((y + 1) * tileSize); // Subtract to flip the direction

            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, posX, posY);
            AEGfxSetTransform(transform.m);
            // Draw walls or paths
            if (maze1[y][x] == 1) {
                AEGfxMeshDraw(wallMesh, AE_GFX_MDM_TRIANGLES);
            }
            if (maze1[y][x] == 2) {
                AEGfxMeshDraw(goalMesh, AE_GFX_MDM_TRIANGLES);
            }
        }
    }

    //std::cout << "World items info: ";
    //    int i{ 1 };
    for (const auto& item : worldItems) {
        //std::cout << i++ << "." << item.type << ", x:" << item.x << ", y:" << item.y<<", collected?"<< item.collected << std::endl;
        if (!item.collected) {
            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, item.x, item.y);
            AEGfxSetTransform(transform.m);

            if (item.type == KEY) {
                AEGfxMeshDraw(keyMesh, AE_GFX_MDM_TRIANGLES);
            }
            else if (item.type == ROCK) {
                AEGfxMeshDraw(rockMesh, AE_GFX_MDM_TRIANGLES);
            }
        }
    }
}

AEGfxVertexList* CreateSquareMesh(float size, u32 color)
{
    AEGfxMeshStart();

    // Add two triangles to form a square
    AEGfxTriAdd(-size / 2, -size / 2, color, 0.0f, 1.0f,
        size / 2, -size / 2, color, 1.0f, 1.0f,
        -size / 2, size / 2, color, 0.0f, 0.0f);

    AEGfxTriAdd(size / 2, -size / 2, color, 1.0f, 1.0f,
        size / 2, size / 2, color, 1.0f, 0.0f,
        -size / 2, size / 2, color, 0.0f, 0.0f);

    return AEGfxMeshEnd();
}


