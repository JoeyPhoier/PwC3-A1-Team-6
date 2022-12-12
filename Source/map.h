#pragma once

#include "raylib.h"
#include "entities.h"
#include <vector>
#include <list>


#define GRASS Rectangle(tileSize, tileSize, tileSize, tileSize)
#define WATER Rectangle(11 * tileSize, tileSize, tileSize, tileSize)
#define DIRT  Rectangle(6 * tileSize, tileSize, tileSize, tileSize)
#define TREE  Rectangle(0, 10 * tileSize, tileSize, 2 * tileSize)


class Entity;
class Plant;

class Tile {
public:
    Rectangle groundSource{0};
    Color groundTint = WHITE;
    
    Entity* entity = nullptr;
    
    bool canBeTilled = false;
    bool isTilled = false;
    bool isWet = false;


    void UpdateNewDay();

};

class Map {
public:
    const int tileSize = 64;
    int tilesX = 32;
    int tilesY = 32;
    std::vector<Tile> tiles = {};
    Texture2D spriteSheet;
    
    std::list<Plant> plants = {};
    Texture2D plantSpriteSheet;
    std::vector<Entity*> entities = {};

    
    Map();

    void PlantSeed(int tileIndex, int id);

    void UpdateNewDay();

    void RenderGround(Camera2D* camera);

    void RenderEntities(Camera2D* camera);

};