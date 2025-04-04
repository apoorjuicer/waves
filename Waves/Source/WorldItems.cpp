
#include "WorldItems.h"

std::vector<WorldItem> createWorldItems(f32 tileSize) {
     float startY = tileSize; // Y starts at the top (10 tiles * tile size)

     std::vector<WorldItem> worldItems;
     //std::cout << "Rendering Maze and World Items:" << std::endl;
     for (int y{};y < MAX_ROW;++y) {
         for (int x{}; x < MAX_COL; ++x) {
             int tileValue = maze[y][x];
             f32 worldX = x * tileSize;
             f32 worldY = startY - ((y + 1) * tileSize);

             if (tileValue == 3) {
                 worldItems.emplace_back(WorldItem(KEY, worldX, worldY));
             }
             else if (tileValue == 4) {
                 worldItems.emplace_back(WorldItem(ROCK, worldX, worldY));
             }
         }
     }
     return worldItems;
}

void RenderItems(AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh, AEGfxTexture* keyTexture, AEGfxTexture* rockTexture, std::vector<WorldItem> worldItems) {

    f32 time = static_cast<f32>(AEGetTime(nullptr));

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    for (const auto& item : worldItems) {
        //std::cout << "." << item.type << ", x:" << item.x << ", y:" << item.y << ", collected?" << item.collected << std::endl;
        if (!item.collected) {
            f32 bobbingOffset = (item.type == KEY) ? sin(time * 2.0f) * 5.0f : 0.0f;

            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, item.x, item.y + bobbingOffset);
            AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
            AEGfxSetTransform(transform.m);

            if (item.type == KEY) {
                AEGfxTextureSet(keyTexture, 0, 0);
                AEGfxMeshDraw(keyMesh, AE_GFX_MDM_TRIANGLES);
            }
            else if (item.type == ROCK) {
                AEGfxTextureSet(rockTexture, 0, 0);
                AEGfxMeshDraw(rockMesh, AE_GFX_MDM_TRIANGLES);
            }
            // Reset state to prevent carry-over
            AEMtx33Identity(&transform);
            AEGfxSetTransform(transform.m);
            AEGfxTextureSet(nullptr, 0, 0);
        }
    }
}