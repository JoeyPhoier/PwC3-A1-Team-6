#include "Inventory.h"
#include <vector>
#include <iostream>
//#include "player.h"						

enum Direction { Down, Up, Left, Right };

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
	int firstEmptyIndex = -1;
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
	Vector2 currTile{ int((playerPos.x) / (map.tileSize)),int((playerPos.y) / (map.tileSize)) };
	Vector2 mouseCoor = GetScreenToWorld2D(GetMousePosition(), camera);

	mouseCoor.x = int (mouseCoor.x / map.tileSize);
	mouseCoor.y = int (mouseCoor.y / map.tileSize);
	std::cout << mouseCoor.x << std::endl;

	int deltax = mouseCoor.x - currTile.x;
	int deltay = mouseCoor.y - currTile.y;
	//int mouseCoorX = int((GetMouseX() - camera.offset.x + camera.target.x) / (map.tileSize * camera.zoom)) - tileCoorX;
	//int mouseCoorY = int((GetMouseY() - camera.offset.y + camera.target.y) / (map.tileSize * camera.zoom)) - tileCoorY;


	//This copies the way the tools in Stardew valley control;
	//Uses the mouse if it is close to player, and otherwise uses the direction the player is facing.
	if ((deltax <= 1 && deltax >= -1 && deltay <= 1 && deltay >= -1) && !(deltax == 0 && deltay == 0 && !canTargetCurrTile)) {
		switch (deltax) {
		case 1:
			*facingDir = Right;
			currTile.x++;
			break;
		case -1:
			*facingDir = Left;
			currTile.x--;
		}
		switch (deltay) {
		case 1:
			*facingDir = Down;
			currTile.y++;
			break;
		case -1:
			*facingDir = Up;
			currTile.y--;
		}
	}
	else {
		switch (*facingDir) {
		case(2):
			currTile.x--;
			break;
		case(1):
			currTile.y--;
			break;
		case(3):
			currTile.x++;
			break;
		case(0):
			currTile.y++;
			break;
		}
	}

	if (currTile.x >= map.tilesX || currTile.x < 0 || currTile.y >= map.tilesY || currTile.y < 0) return -1;
	else return (currTile.x + currTile.y * map.tilesX);
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
	map.tiles[tileIndex].groundSource = Rectangle(6 * map.tileSize, map.tileSize, map.tileSize, map.tileSize);

	return true;
}

bool WateringCanClass::UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir) {
	int tileIndex = TileIndex(camera, playerPos, map, facingDir, true);
	if (tileIndex == -1 || !map.tiles[tileIndex].isTilled) return false;

	map.tiles[tileIndex].isWet = true;
	map.tiles[tileIndex].groundTint = GRAY;

	return true;
};

