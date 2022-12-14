#pragma once

#include "raylib.h"
#include "map.h"

class Tile;
class Map;

class Entity {
public:
	Vector2 pos; // Used for rendering order, could maybe use collisionBox instead
	Vector2 renderPos;

	Rectangle collisionBox;


	virtual void Render() = 0;

};

class TileEntity : public Entity {
public:
	Tile* parent = nullptr;
	
	virtual void UpdateNewDay() = 0;
	virtual void Interact() = 0;

};

class Plant : public TileEntity {
public:
	int id;
	Texture2D* spriteSheet;
	Rectangle textureSource;
	int growthStage = 0;
	int maxGrowthStage;

	Plant(int idi, Texture2D* spriteSheeti);

	void UpdateNewDay();
	void Interact();
	void Render();

};

class Bed : public TileEntity {
public:
	Texture2D* spriteSheet;
	Rectangle textureSource = {0, 64, 64, 128};
	Map* map = nullptr;

	void UpdateNewDay();
	void Interact();
	void Render();

};