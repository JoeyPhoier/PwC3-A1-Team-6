#pragma once

#include "raylib.h"
#include "map.h"

class Tile;

class Entity {
public:
	Vector2 pos; // Used for rendering order, could maybe use collisionBox instead
	Vector2 renderPos;

	Texture2D* texture;
	Rectangle collisionBox;


	virtual void Update() = 0;
	virtual void UpdateNewDay() = 0;
	virtual void Render() = 0;

};


class Plant : public Entity {
public:
	int id;
	Rectangle textureSource;
	int growthStage = 0;
	int maxGrowthStage;
	Tile* parent = nullptr;

	Plant(int idi, Texture2D* texturei);

	void Update();
	void UpdateNewDay();
	void Render();

};