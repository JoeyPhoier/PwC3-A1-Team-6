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

	virtual void UpdateNewDay() {};
	virtual void Interact(Inventory* inventory) {};
	virtual void Render() {};

};

class TileEntity : public Entity {
public:
	Tile* parent = nullptr;
	Vector2 renderPos;
	Texture2D* spriteSheet;
	Rectangle textureSource;
	
	
	void Render();

};

class Plant : public TileEntity {
public:
	int id;
	int lootId;
	int growthStage = 0;
	int maxGrowthStage;

	Plant(int idi, Texture2D* spriteSheeti, Vector2 positioni, Tile* parenti);

	void UpdateNewDay();
	virtual void Interact(Inventory* inventory);


};

class Bed : public TileEntity {
public:
	Map* map = nullptr;

	Bed();
	~Bed();
	void UpdateNewDay();
	void Interact(Inventory* inventory) ;

};

class MarketStall : public TileEntity {
public:
	MarketStall(Texture2D* spriteSheeti, Vector2 renderPosi);
	~MarketStall();
	void Interact(Inventory* inventory);
	void UpdateNewDay() {};
};