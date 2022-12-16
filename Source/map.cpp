#include "map.h"

void Tile::UpdateNewDay() {
	if (entity != nullptr) { entity->UpdateNewDay(); }
	isWet = false;
	groundTint = WHITE;
}

void Tile::Interact(Inventory* inventory, Map* map) {
	if (entity == nullptr) return;
	entity->Interact(inventory);

}

Map::Map(Entity* player) {
	
	spriteSheet = LoadTexture("assets/RPGpack_sheet.png");
	marketStallTexture = LoadTexture("assets/marketplace_stall.png");
	//marketStallTexture = LoadTexture("assets/items/carrot.png");

	plantSpriteSheet = LoadTexture("assets/plants_free.png");

	furnitureSpriteSheet = LoadTexture("assets/Basic Furniture.png");

	for (int i = 0; i < tilesX * tilesY; ++i) {
		Tile newTile;
		if (preMap[i] == 0) {
			newTile.groundSource = GRASS;
			newTile.canBeTilled = true;
		}
		else if (preMap[i] == 1) {
			newTile.groundSource = WATER;
			collisions.push_back(Rectangle(static_cast<float>((i % tilesX) * tileSize), static_cast<float>((i / tilesY) * tileSize), static_cast<float>(tileSize), static_cast<float>(tileSize)));
			std::cout << collisions.back().x << std::endl;
		}
		else if (preMap[i] == 2) {
			collisions.push_back(Rectangle(static_cast<float>((i % tilesX) * tileSize), static_cast<float>((i / tilesY) * tileSize), static_cast<float>(tileSize), static_cast<float>(tileSize)));

			Bed newBed;
			newBed.position = Vector2(static_cast<float>((i % tilesX) * tileSize), static_cast<float>((i / tilesY) * tileSize));
			newBed.renderPos = Vector2(static_cast<float>((i % tilesX) * tileSize), static_cast<float>(((i / tilesY) - 1) * tileSize));
			newBed.spriteSheet = &furnitureSpriteSheet;
			newBed.map = this;
			newBed.collisionBox = &collisions.back();
			beds.push_back(newBed);
			entities.push_back(&beds.back());

			newTile.groundSource = GRASS;
			newTile.canBeTilled = false;
			newTile.entity = &beds.back();
		}
		else if (preMap[i] == 3) {
			MarketStall sellStall(&marketStallTexture, Vector2(static_cast<float>((i % tilesX) * tileSize), static_cast<float>((i / tilesY) * tileSize)));
			marketStalls.push_back(sellStall);
			entities.push_back(&marketStalls.back());

			newTile.groundSource = GRASS;
			newTile.canBeTilled = false;
			newTile.entity = &marketStalls.back();;
		}

		tiles.push_back(newTile);
	}

	entities.push_back(player);

}

Map::~Map() {
	UnloadTexture(spriteSheet);
	UnloadTexture(plantSpriteSheet);
	UnloadTexture(furnitureSpriteSheet);
}

void Map::SortEntities() {
	size_t numberOfEntities = entities.size();
	for (size_t i = 1; i < numberOfEntities; ++i) {
		for (size_t j = i; j != 0; --j) {
			if (entities[j]->position.y < entities[j - 1]->position.y) {
				Entity* temp = entities[j];
				entities[j] = entities[j - 1];
				entities[j - 1] = temp;
			}
			else {
				break;
			}
		}
	}
}

void Map::RemoveDeadEntities() {
	auto newEnd = std::remove_if(entities.begin(), entities.end(), [](Entity* entity) { return entity->isDead; });
	entities.erase(newEnd, entities.end());

	for (int i = 0; i < tilesX * tilesY; ++i) {
		if (tiles[i].entity != nullptr) {
			if (tiles[i].entity->isDead) { tiles[i].entity = nullptr; }
		}
	}

	plants.remove_if([](Plant& plant) { return plant.isDead; });
}

void Map::PlantSeed(int tileIndex, int id) {
	Plant newPlant{id, &plantSpriteSheet, Vector2(static_cast<float>((tileIndex % tilesY) * tileSize), static_cast<float>((tileIndex / tilesY) * tileSize)), &tiles[tileIndex]};

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
			DrawTextureRec(spriteSheet, tiles[y * tilesX + x].groundSource, Vector2(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize)), tiles[y * tilesX + x].groundTint);
		}
	}
}

void Map::RenderEntities(Camera2D* camera) {
	SortEntities();
	size_t numOfEntities = entities.size();
	for (size_t i = 0; i < numOfEntities; ++i) {
		// Should check if it's onscreen
		entities[i]->Render();
	}
}