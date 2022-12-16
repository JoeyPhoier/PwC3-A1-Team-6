#include "Inventory.h"
#include <vector>
#include <iostream>
//#include "player.h"						

enum Direction { Down, Up, Left, Right };

void Inventory::GetMoney(int& amount) {
	currMoney += amount;
}

void Inventory::GetPreMoney(int& amount) {
	preMoney += amount;
}

void Inventory::LoseMoney(int& amount) {
	currMoney -= amount;
	if (currMoney < 0) currMoney = 0;
}

void Inventory::LosePreMoney(int& amount) {
	preMoney -= amount;
	if (preMoney < 0) preMoney = 0;
}

Inventory::Inventory() {
	for (int i = 0; i < invLim; i++) {
		slot[i] = nullptr;
	}

	Image tempImage = LoadImage("assets/items/items free.png");
	ImageResize(&tempImage, 200, 120);
	vegetableSpriteSheet = LoadTextureFromImage(tempImage);
	UnloadImage(tempImage);
	//vegetableSpriteSheet = LoadTexture("assets/items/items free.png");
	hoeSprite = LoadTexture("assets/items/tools/Hoe.png");
	wateringCanSprite = LoadTexture("assets/items/tools/Watering Can.png");
}

Inventory::~Inventory() {
	for (int i = 0; i < invLim; i++) {
		delete slot[i];
		slot[i] = nullptr;
	}

	UnloadTexture(vegetableSpriteSheet);
	UnloadTexture(hoeSprite);
	UnloadTexture(wateringCanSprite);
}

bool Inventory::AddItem(int id) {
	int firstEmptyIndex = -1;
	for (int i = 0; i < invLim; i++) {
		if (slot[i] != nullptr) {
			//Check if id is the same, then if this slot has less than stack limit
			if (slot[i]->id == id && slot[i]->isStackable){
				slot[i]->currStack++;
				return true;
			}
		}
		else if (firstEmptyIndex == -1) {
			firstEmptyIndex = i;
		}
	}
	if (firstEmptyIndex != -1) {

		if (itemDictionary[id]->type == Hoe) {							
			slot[firstEmptyIndex] = new HoeClass{ *itemDictionary[id] }; //Wonder if copy constructors would make a difference here
		}
		else if (itemDictionary[id]->type == WateringCan) {
			slot[firstEmptyIndex] = new WateringCanClass{ *itemDictionary[id] };
		}
		else if (itemDictionary[id]->type == Vegetable) {
			slot[firstEmptyIndex] = new VegetableClass( *itemDictionary[id] );
		}
		else if (itemDictionary[id]->type == Seed) {
			slot[firstEmptyIndex] = new SeedClass( *itemDictionary[id]);
		}

		return true;
	}
	return false;
}

bool Inventory::AddItem(int id, int amount) {
	bool didLastFuncPass = true;
	for (int i = 0; i < amount; i++) {
		didLastFuncPass = AddItem(id);
	}
	return didLastFuncPass;
}

bool Inventory::RemoveItem(int id) {
	//int firstEmptyIndex = -1;
	for (int i = 0; i < invLim; i++) {
		if (slot[i] != nullptr) {
			if (slot[i]->id == id && slot[i]->isStackable) {
				if (slot[i]->currStack > 1) slot[i]->currStack--;
				else {
					delete slot[i];
					slot[i] = nullptr;
				}
				return true;
			}
		}
	}
	return false;
}

bool Inventory::RemoveItem(int id, int amount) {
	bool didLastFuncPass = true;
	for (int i = 0; i < amount; i++) {
		didLastFuncPass = RemoveItem(id);
	}
	return didLastFuncPass;
}

int TileIndex(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir, bool canTargetCurrTile = 0) {
	//Vector2 currTile{ int((playerPos.x) / (map.tileSize)),int((playerPos.y) / (map.tileSize)) };
	int currTileX = static_cast<int>(playerPos.x / map.tileSize);
	int currTileY = static_cast<int>(playerPos.y / map.tileSize);
	Vector2 mouseCoor = GetScreenToWorld2D(GetMousePosition(), camera);

	int mouseX = static_cast<int>(mouseCoor.x) / map.tileSize;
	int mouseY = static_cast<int>(mouseCoor.y) / map.tileSize;
	std::cout << mouseX << std::endl;

	int deltax = mouseX - currTileX;
	int deltay = mouseY - currTileY;
	//int mouseCoorX = int((GetMouseX() - camera.offset.x + camera.target.x) / (map.tileSize * camera.zoom)) - tileCoorX;
	//int mouseCoorY = int((GetMouseY() - camera.offset.y + camera.target.y) / (map.tileSize * camera.zoom)) - tileCoorY;


	//This copies the way the tools in Stardew valley control;
	//Uses the mouse if it is close to player, and otherwise uses the direction the player is facing.
	if ((deltax <= 1 && deltax >= -1 && deltay <= 1 && deltay >= -1) && !(deltax == 0 && deltay == 0 && !canTargetCurrTile)) {
		switch (deltax) {
		case 1:
			*facingDir = Right;
			currTileX++;
			break;
		case -1:
			*facingDir = Left;
			currTileX--;
		}
		switch (deltay) {
		case 1:
			*facingDir = Down;
			currTileY++;
			break;
		case -1:
			*facingDir = Up;
			currTileY--;
		}
	}
	else {
		switch (*facingDir) {
		case(2):
			currTileX--;
			break;
		case(1):
			currTileY--;
			break;
		case(3):
			currTileX++;
			break;
		case(0):
			currTileY++;
			break;
		}
	}

	if (currTileX >= map.tilesX || currTileX < 0 || currTileY >= map.tilesY || currTileY < 0) return -1;
	else return (currTileX + currTileY * map.tilesX);
}

bool SeedClass::UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir) {
	int tileIndex = TileIndex(camera, playerPos, map, facingDir, true);
	if (tileIndex == -1 || !map.tiles[tileIndex].isTilled || map.tiles[tileIndex].entity != nullptr) return false;

	currStack--;
	map.PlantSeed(tileIndex, id);

	return true;
	
}

bool HoeClass::UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir) {		//Hoes,Picks and axes cannot target the player tile.
	int tileIndex = TileIndex(camera, playerPos, map, facingDir);
	if (tileIndex == -1 || !map.tiles[tileIndex].canBeTilled) return false;

	map.tiles[tileIndex].isTilled = true;
	map.tiles[tileIndex].groundSource = Rectangle(static_cast<float>(6 * map.tileSize), static_cast<float>(map.tileSize), static_cast<float>(map.tileSize), static_cast<float>(map.tileSize));

	return true;
}

bool WateringCanClass::UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir) {
	int tileIndex = TileIndex(camera, playerPos, map, facingDir, true);
	if (tileIndex == -1 || !map.tiles[tileIndex].isTilled) return false;

	map.tiles[tileIndex].isWet = true;
	map.tiles[tileIndex].groundTint = GRAY;

	return true;
};

