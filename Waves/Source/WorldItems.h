#pragma once

#include "pch.h"

enum InventoryItemType {
    NOTHING = 0,
    KEY,
    ROCK
};

struct WorldItem {
    InventoryItemType type;
    f32 x, y;
    bool collected;

    WorldItem(InventoryItemType t, f32 posX, f32 posY) : type(t), x(posX), y(posY), collected(false) {}
};

std::vector<WorldItem> createWorldItems(f32 tileSize);
void RenderItems(AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh, AEGfxTexture* keyTexture, AEGfxTexture* rockTexture, std::vector<WorldItem> worldItems);