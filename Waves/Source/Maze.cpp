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

#include "Maze.h"
#include "WorldItems.h"

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
        //std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line, label;

    // Read row count from first line (e.g., "ROW: 10")
    if (!std::getline(file, line)) {
        //std::cerr << "Error: Failed to read row info from file " << filename << "\n";
        return false;
    }
    std::istringstream issRow(line);
    if (!(issRow >> label >> activeRow)) {
        //std::cerr << "Error: Failed to parse row info from file " << filename << "\n";
        return false;
    }

    // Read column count from second line (e.g., "COL: 10")
    if (!std::getline(file, line)) {
        //std::cerr << "Error: Failed to read column info from file " << filename << "\n";
        return false;
    }
    std::istringstream issCol(line);
    if (!(issCol >> label >> activeCol)) {
        //std::cerr << "Error: Failed to parse column info from file " << filename << "\n";
        return false;
    }

    // Validate dimensions against MAX_ROW and MAX_COL.
    if (activeRow <= 0 || activeRow > MAX_ROW || activeCol <= 0 || activeCol > MAX_COL) {
        //std::cerr << "Error: Maze dimensions (" << activeRow << "x" << activeCol
        //    << ") out of allowed range (" << MAX_ROW << "x" << MAX_COL << ")\n";
        return false;
    }

    // Read maze data row by row.
    for (int i = 0; i < activeRow; ++i) {
        if (!std::getline(file, line)) {
        //    std::cerr << "Error: Not enough rows in maze file " << filename << "\n";
            return false;
        }
        std::istringstream iss(line);
        for (int j = 0; j < activeCol; ++j) {
            if (!(iss >> maze[i][j])) {
             //   std::cerr << "Error: Failed to read maze data at row " << i << ", col " << j << "\n";
                return false;
            }
        }
    }
    return true;
}

void ClearMazeData()
{
    for (int i = 0; i < MAX_ROW; ++i)
    {
        for (int j = 0; j < MAX_COL; ++j)
        {
            maze[i][j] = 0; // Or your default tile ID
        }
    }

    // Optionally reset active dimensions too
    activeRow = 0;
    activeCol = 0;
}

void RenderBackground(f32 tilesize, AEGfxVertexList* pathMesh, AEGfxVertexList* goalMesh, AEGfxTexture* pathTexture/*, AEGfxTexture* goalTexture*/) {
    // Calculate the top-left corner of the maze
    float startX = 0.0f; // X starts at 0
    float startY = tilesize; // Y starts at the top (10 tiles * tile size)

    for (int y = 0; y < MAX_ROW; ++y) {
        for (int x = 0; x < MAX_COL; ++x) {
            // Calculate the position of each tile
            f32 posX = startX + (x * tilesize);
            f32 posY = startY - ((y + 1) * tilesize); // Subtract to flip the direction

            AEMtx33 transform = { 0 }; AEMtx33 scale{}; AEMtx33 trans{};
            AEMtx33Scale(&scale, TILE_SIZE, TILE_SIZE);
            AEMtx33Trans(&trans, posX, posY);
            AEMtx33Concat(&transform, &trans, &scale);

            AEGfxSetTransform(transform.m);
            // Draw walls or paths
            if (maze[y][x] != 1) {

                AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.0f);
                AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);

                AEGfxTextureSet(pathTexture, 0.f, 0.f);
                AEGfxMeshDraw(pathMesh, AE_GFX_MDM_TRIANGLES);
            }
            if (maze[y][x] == 2) {
                AEGfxSetRenderMode(AE_GFX_RM_COLOR);
                AEGfxMeshDraw(goalMesh, AE_GFX_MDM_TRIANGLES);
            }
        }
    }
}

void RenderWalls(f32 tilesize, AEGfxVertexList* wallMesh) {
    // Calculate the top-left corner of the maze
    float startX = 0.0f; // X starts at 0
    float startY = tilesize; // Y starts at the top (10 tiles * tile size)

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 1.0f);

    for (int y = 0; y < MAX_ROW; ++y) {
        for (int x = 0; x < MAX_COL; ++x) {
            // Calculate the position of each tile
            f32 posX = startX + (x * tilesize);
            f32 posY = startY - ((y + 1) * tilesize); // Subtract to flip the direction

            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, posX, posY);
            AEGfxSetRenderMode(AE_GFX_RM_COLOR);
            AEGfxSetTransform(transform.m);
            // Draw walls or paths
            if (maze[y][x] == 1) {
                AEGfxMeshDraw(wallMesh, AE_GFX_MDM_TRIANGLES);
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

void PrintMapData() {
    std::cout << "ROW: " << activeRow << '\n'
        << "COL: " << activeCol << '\n';

    for (int i{}; i < activeRow; ++i) {
        for (int j{}; j < activeCol; ++j) {
            std::cout << maze[i][j] << ' ';

            if (j == (activeCol - 1)) {
                std::cout << '\n';
            }
        }
    }
}


