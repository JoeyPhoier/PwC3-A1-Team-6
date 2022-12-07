#pragma once

#include "raylib.h"

#include <vector>


#define GRASS Rectangle(tileSize, tileSize, tileSize, tileSize)
#define WATER Rectangle(11 * tileSize, tileSize, tileSize, tileSize)
#define DIRT  Rectangle(6 * tileSize, tileSize, tileSize, tileSize)
#define TREE  Rectangle(0, 10 * tileSize, tileSize, 2 * tileSize)



class Tile {
public:
    Rectangle groundSource{0};
    Color groundTint = WHITE;

    bool hasTree = false;
    bool canBeTilled = false;
    bool isTilled = false;
    bool isWet = false;

};

class Map {
public:
    const int tileSize = 64;
    int tilesX = 32;
    int tilesY = 32;
    std::vector<Tile> tiles = {};
    Texture2D spriteSheet;

    
    Map();

    void render(Camera2D* camera);

};