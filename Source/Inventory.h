#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <iostream>
#include "raylib.h"
#include "map.h"

class Map;

int TileIndex(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir, bool canTargetCurrTile);

enum ItemType { NaN, Fruit, Vegetable, Seed, Tool, Hoe, WateringCan };

class Item {										//Might need to make some subclasses for added info, such as a seed subclass
public:												//that holds both the in-inventory sprite and the overworld sprite.
	int id;											//Would also hold all the tool specific info if an energy system were to be implemented.
	std::string name;
	ItemType type;
	bool isStackable = true;
	//int stackLim;									//In the game, maxStack is 999. We could try lower numbers similar to minecraft.
	int currStack = 1;								//REMEMBER TO CHANGE "AddItem()" IF THE ABOVE GETS IMPLEMENTED.
	bool isSellable = true;
	int value = 0;
	Texture2D* sprite;
	Rectangle spriteSource;

	Item(int idi, std::string namei, int valuei, Texture2D* spritei, Rectangle spriteSourcei) {	//move to .cpp
		id = idi;
		name = namei;
		value = valuei;
		sprite = spritei;
		spriteSource = spriteSourcei;
	};
	Item(Item& item) {
		id = item.id;
		name = item.name;
		value = item.value;
		sprite = item.sprite;
		spriteSource = item.spriteSource;
	}

	virtual bool UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir) {
		currStack--;
		return true;
	};

};

class VegetableClass : public Item {
public:
	VegetableClass(int idi, std::string namei, int valuei, Texture2D* spritei, Rectangle spriteSourcei) :Item(idi, namei, valuei, spritei, spriteSourcei) {
		type = Vegetable;
	}
	VegetableClass(Item& item) : Item(item) { type = Vegetable; }
};

class SeedClass : public Item {
public:
	SeedClass(int idi, std::string namei, int valuei, Texture2D* spritei, Rectangle spriteSourcei) :Item(idi, namei, valuei, spritei, spriteSourcei) {	//move to .cpp
		type = Seed;
	}
	SeedClass(Item& item) : Item(item) { type = Seed; }
	bool UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir);
};

class ToolClass : public Item {
public:
	ToolClass(int idi, std::string namei,Texture2D* spritei, Rectangle spriteSourcei) :Item(idi, namei, 0, spritei, spriteSourcei) {
		type = Tool;
		isStackable = false;
		isSellable = false;
	}
	ToolClass(Item& item) : Item(item) {
		type = Tool;
		isStackable = false;
		isSellable = false;
	}
};

class HoeClass : public ToolClass {
public:
	HoeClass(int idi, std::string namei, Texture2D* spritei, Rectangle spriteSourcei) :ToolClass(idi, namei, spritei, spriteSourcei) { type = Hoe; };
	HoeClass(Item& item) :ToolClass(item) { type = Hoe; };
	bool UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir);
};

class WateringCanClass : public ToolClass {
public:
	WateringCanClass(int idi, std::string namei, Texture2D* spritei, Rectangle spriteSourcei) :ToolClass(idi, namei, spritei, spriteSourcei) { type = WateringCan; };
	WateringCanClass(Item& item) :ToolClass(item) { type = WateringCan; };
	bool UseItem(Camera2D& camera, Vector2& playerPos, Map& map, int* facingDir);
};

class Inventory {
public:
	std::array<Item*, 10> slot;						//Using Item** might make inventory expansion easier
	int invLim = static_cast<int>(slot.size());
	int selectedIndex = 0;
	int currMoney = 0;
	int preMoney = 0;


	void GetMoney(int& amount);
	void GetPreMoney(int& amount);
	void LoseMoney(int& amount);
	void LosePreMoney(int& amount);

	Texture2D vegetableSpriteSheet;
	Texture2D hoeSprite;
	Texture2D wateringCanSprite;
	Inventory();
	~Inventory();

	bool AddItem(int id);
	bool AddItem(int id, int amount);

	bool RemoveItem(int id);
	bool RemoveItem(int id, int amount);

	std::unordered_map<int, Item*> itemDictionary = {
		//These down here dont need to be in any particular order, but it makes our job easier if they're organized by type.
		//I was thinking of reserving the indexes in 100's, so plants got the first 100, then tools got the next 100, and so on.
		//This would help in case we'd like to have new items have a similar id to others of their type, without having
		//to reorder every following item in the list.
		{1,new VegetableClass(								//Kinda dumb how this works, but aggregate initialization
			1,												//doesn't work for subclass, nor for classes with virtual funcs.
			"Pumpkin",										//Cant use designated parameters for constructors either :(
			//.stackLim = 64,
			70,
			&vegetableSpriteSheet,
			Rectangle(0, 0, 40, 40)
			)	
		},
		{2,new VegetableClass(
			2,
			"Carrot",
			//.stackLim = 64,
			50,
			&vegetableSpriteSheet,
			Rectangle(80, 0, 40, 40)
			)
		},
		{3,new VegetableClass(
			3,
			"Potato",
			//.stackLim = 64,
			35,
			&vegetableSpriteSheet,
			Rectangle(40, 0, 40, 40)
			)
		},
		{4,new VegetableClass(
			4,
			"Tomato",
			//.stackLim = 64,
			40,
			&vegetableSpriteSheet,
			Rectangle(120, 0, 40, 40)
			)
		},
		{101,new Item(
			101,
			"Pumpkin Seeds",
			5,
			&vegetableSpriteSheet,
			Rectangle(0, 40, 40, 40)
			)

		},
		{102,new SeedClass(
			102,
			"Carrot Seeds",
			3,
			&vegetableSpriteSheet,
			Rectangle(80, 40, 40, 40)
			)
		},
		{103,new SeedClass(
			103,
			"Potato Seeds",
			3,
			&vegetableSpriteSheet,
			Rectangle(40, 40, 40, 40)
			)
		},
		{104,new SeedClass(
			104,
			"Tomato Seeds",
			3,
			&vegetableSpriteSheet,
			Rectangle(120, 40, 40, 40)
			)
		},
		{201,new HoeClass{									
			201,									
			"Copper Hoe",
			&hoeSprite,
			Rectangle(0, 0, 40, 40)
			}
		},
		{202, new WateringCanClass{
			202,
			"Watering Can",
			&wateringCanSprite,
			Rectangle(0, 0, 32, 32)
			}
		}
		
	};
	


};


