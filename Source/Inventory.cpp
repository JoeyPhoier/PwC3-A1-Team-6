#include "Inventory.h"
#include <vector>
#include <iostream>
#include "raylib.h"
#include "map.h"
#include "player.h"						

Inventory::Inventory() {
	for (int i = 0; i < invLim; i++) {
		slot[i] = nullptr;
	}
}

Inventory::~Inventory() {
	for (int i = 0; i < invLim; i++) {
		delete slot[i];
		slot[i] = nullptr;
	}
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

int TileIndex(Camera2D& camera, Map& map, int facingDir, bool canTargetCurrTile = 0) {
	int tileCoorX = (camera.target.x) / (map.tileSize * camera.zoom);
	int tileCoorY = (camera.target.y) / (map.tileSize * camera.zoom);

	int mouseCoorX = int((GetMouseX() - camera.offset.x + camera.target.x) / (map.tileSize * camera.zoom)) - tileCoorX;
	int mouseCoorY = int((GetMouseY() - camera.offset.y + camera.target.y) / (map.tileSize * camera.zoom)) - tileCoorY;



	//This copies the way the tools in Stardew valley control;
	//Uses the mouse if it is close to player, and otherwise uses the direction the player is facing.
	if ((mouseCoorX <= 1 && mouseCoorX >= -1 && mouseCoorY <= 1 && mouseCoorY >= -1) && !(mouseCoorX == 0 && mouseCoorY == 0 && !canTargetCurrTile)) {
		switch (mouseCoorX) {
		case 1:
			tileCoorX++;
			break;
		case -1:
			tileCoorX--;
		}
		switch (mouseCoorY) {
		case 1:
			tileCoorY++;
			break;
		case -1:
			tileCoorY--;
		}
	}
	else {
		switch (facingDir) {
		case(0):
			tileCoorX--;
			break;
		case(1):
			tileCoorY--;
			break;
		case(2):
			tileCoorX++;
			break;
		case(3):
			tileCoorY++;
			break;
		}
	}

	if (tileCoorX > map.tilesX || tileCoorX < 0 || tileCoorY > map.tilesY || tileCoorY < 0) return -1;
	else return (tileCoorX + tileCoorY * map.tilesX);
}

bool SeedClass::UseItem(Camera2D& camera, Map& map, int facingDir) {
	int tileIndex = TileIndex(camera, map, facingDir, true);
	if (tileIndex == -1 || !map.tiles[tileIndex].isTilled) return false;

	currStack--;
	map.PlantSeed(tileIndex, id);

	return true;
	
}

bool HoeClass::UseItem(Camera2D& camera, Map& map, int facingDir) {		//Hoes,Picks and axes cannot target the player tile.
	int tileIndex = TileIndex(camera, map, facingDir);
	if (tileIndex == -1 || !map.tiles[tileIndex].canBeTilled) return false;

	map.tiles[tileIndex].isTilled = true;
	map.tiles[tileIndex].groundSource = Rectangle(6 * map.tileSize, map.tileSize, map.tileSize, map.tileSize);

	return true;
}

bool WateringCanClass::UseItem(Camera2D& camera, Map& map, int facingDir) {
	int tileIndex = TileIndex(camera, map, facingDir, true);
	if (tileIndex == -1 || !map.tiles[tileIndex].isTilled) return false;

	map.tiles[tileIndex].isWet = true;
	map.tiles[tileIndex].groundTint = GRAY;

	return true;
};

