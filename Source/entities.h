#pragma once

#include "raylib.h"
#include "map.h"
#include "Inventory.h"

class Tile;
class Map;

class Inventory;

class Entity {
public:
	Vector2 position = Vector2(0, 0); // Used for rendering order, could maybe use collisionBox instead
	Rectangle* collisionBox;
	bool isDead = false;


	virtual void Render() {};

};

class TileEntity : public Entity {
public:
	Tile* parent = nullptr;
	Vector2 renderPos;
	
	virtual void UpdateNewDay() = 0;
	virtual void Interact(Inventory* inventory) = 0;

};

class Plant : public TileEntity {
public:
	int id;
	int lootId;
	Texture2D* spriteSheet;
	Rectangle textureSource;
	int growthStage = 0;
	int maxGrowthStage;

	Plant(int idi, Texture2D* spriteSheeti, Vector2 positioni, Tile* parenti);

	void UpdateNewDay();
	void Interact(Inventory* inventory);
	void Render();

};

class Bed : public TileEntity {
public:
	Texture2D* spriteSheet;
	Rectangle textureSource = {0, 64, 64, 128};
	Map* map = nullptr;

	void UpdateNewDay();
	void Interact(Inventory* inventory);
	void Render();

};