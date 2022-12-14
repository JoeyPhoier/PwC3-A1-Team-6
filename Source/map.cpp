#include "map.h"




void Tile::UpdateNewDay() {
	if (entity != nullptr) { entity->UpdateNewDay(); }
	isWet = false;
	groundTint = WHITE;
}

void Tile::Interact() {
	if (entity == nullptr) return;
	entity->Interact();

}




Map::Map() {
	
	spriteSheet = LoadTexture("assets/RPGpack_sheet.png");

	Image tempImage = LoadImage("assets/plants_free.png");
	ImageResize(&tempImage, 320, 384);
	plantSpriteSheet = LoadTextureFromImage(tempImage);
	//plantSpriteSheet = LoadTexture("assets/plants_free.png");

	tempImage = LoadImage("assets/Basic Furniture.png");
	ImageResize(&tempImage, 576, 384);
	furnitureSpriteSheet = LoadTextureFromImage(tempImage);

	for (int i = 0; i < tilesX * tilesY; ++i) {
		Tile newTile;
		int random = GetRandomValue(0, 10);      // Temporary world generation, should be changed
		if (random > 1) {
			newTile.groundSource = GRASS;
			newTile.canBeTilled = true;
		}
		else {
			newTile.groundSource = WATER;
		}

		tiles.push_back(newTile);
	}
	
	Bed newBed;
	newBed.pos = Vector2(3 * tileSize, 3 * tileSize);
	newBed.renderPos = Vector2(3 * tileSize, 2 * tileSize);
	newBed.spriteSheet = &furnitureSpriteSheet;
	newBed.parent = &tiles[3 * tilesX + 3];
	newBed.map = this;
	beds.push_back(newBed);
	entities.push_back(&beds.back());

	tiles[3 * tilesX + 3].groundSource = GRASS;
	tiles[3 * tilesX + 3].canBeTilled = false;
	tiles[3 * tilesX + 3].entity = &beds.back();

}

void Map::PlantSeed(int tileIndex, int id) {
	Plant newPlant{id, &plantSpriteSheet};
	newPlant.renderPos = Vector2((tileIndex%tilesY) * tileSize, (tileIndex/tilesY) * tileSize);
	newPlant.parent = &tiles[tileIndex];

	plants.push_back(newPlant);
	entities.push_back(&plants.back());
	tiles[tileIndex].entity = &plants.back();

}

void Map::UpdateNewDay() {
	for (int i = 0; i < tilesX * tilesY; ++i) {
		tiles[i].UpdateNewDay();
	}
}

void Map::RenderGround(Camera2D* camera) {
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
		}
	}
}

void Map::RenderEntities(Camera2D* camera) {
	size_t numOfEntities = entities.size();
	for (size_t i = 0; i < numOfEntities; ++i) {
		// Should check if it's onscreen
		entities[i]->Render();
	}
}