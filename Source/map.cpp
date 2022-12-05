#include "map.h"


Map::Map() {
	
	spriteSheet = LoadTexture("assets/RPGpack_sheet.png");

	for (int i = 0; i < tilesX * tilesY; ++i) {
		Tile newTile;
		int random = GetRandomValue(0, 10);      // Temporary world generation, should be changed
		if (random > 6) {
			newTile.groundSource = DIRT;
			newTile.canBePlantedOn = true;
		}
		else if (random > 1) {
			newTile.groundSource = GRASS;
		}
		else {
			newTile.groundSource = WATER;
		}

		if (random == 2) {
			newTile.hasTree = true;
		}
		else if (random == 10) {
			newTile.groundTint = LIGHTGRAY;
			newTile.isWet = true;
		}

		tiles.push_back(newTile);
	}
}

void Map::render(Camera2D* camera) {
	int minY = (int)((camera->target.y - camera->offset.y / camera->zoom) / tileSize);
	int maxY = (int)((camera->target.y + camera->offset.y / camera->zoom + 2 * tileSize) / tileSize);
	int minX = (int)((camera->target.x - camera->offset.x / camera->zoom) / tileSize);
	int maxX = (int)((camera->target.x + camera->offset.x / camera->zoom + tileSize) / tileSize);
	if (minY < 0) minY = 0;
	if (maxY > tilesY) maxY = tilesY;
	if (minX < 0) minX = 0;
	if (maxX > tilesX) maxX = tilesX;

	for (int y = minY; y < maxY; ++y) {
		for (int x = minX; x < maxX; ++x) {
			DrawTextureRec(spriteSheet, tiles[y * tilesX + x].groundSource, Vector2(x * tileSize, y * tileSize), tiles[y * tilesX + x].groundTint);
			if (tiles[y * tilesX + x].hasTree) {
				DrawTextureRec(spriteSheet, TREE, Vector2(x * tileSize, (y - 1) * tileSize), WHITE);
			}
		}
	}
}