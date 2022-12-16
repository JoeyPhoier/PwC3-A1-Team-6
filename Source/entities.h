#pragma once

#include "raylib.h"
#include "map.h"
#include "Inventory.h"

class Tile;
class Map;

class Inventory;

class Entity {
public:
	Vector2 position = Vector2(0, 0);
	Rectangle* collisionBox = nullptr;
	bool isDead = false;

	virtual void UpdateNewDay() {};
	virtual void Interact(Inventory* inventory) {};
	virtual void Render() {};

};

class TileEntity : public Entity {
public:
	Tile* parent = nullptr;
	Vector2 renderPos = Vector2(0, 0);
	Texture2D* spriteSheet = nullptr;
	Rectangle textureSource = {0, 0, 0, 0};
	
	
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

class SellingStall : public TileEntity {
public:
	SellingStall(Texture2D* spriteSheeti, Vector2 renderPosi);
	~SellingStall();
	void Interact(Inventory* inventory);
	void UpdateNewDay() {};
};

class BuyingStall : public TileEntity {
public:
	int seedId;
	BuyingStall(Texture2D* spriteSheeti, Vector2 renderPosi, int seedIdi);
	~BuyingStall() {};
	void Interact(Inventory* inventory);
	void UpdateNewDay() {};
};