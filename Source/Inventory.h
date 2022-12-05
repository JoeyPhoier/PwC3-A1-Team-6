#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include "raylib.h"

enum ItemType { Fruit, Vegetable, Seed, Tool };

class Item {										//Might need to make some subclasses for added info, such as a seed subclass
public:												//that holds both the in-inventory sprite and the overworld sprite.
	int id;											//Would also hold all the tool specific info if an energy system were to be implemented.
	std::string name;
	ItemType type;
	bool isStackable = true;
	//int stackLim;									//In the game, maxStack is 999. We could try lower numbers similar to minecraft.
	int currStack = 1;								//REMEMBER TO CHANGE "AddItem()" IF THE ABOVE GETS IMPLEMENTED. 
	int value = 0;
	Texture2D sprite;
};

class Inventory {
	
public:
	std::array<Item*, 10> slot;						//Using Item** might make inventory expansion easier
	int invLim = slot.size();
	int selectedIndex = 0;
	Inventory();
	~Inventory();

	bool AddItem(int id);
	bool AddItem(int id, int amount);

	bool RemoveItem(int id);
	bool RemoveItem(int id, int amount);

	std::unordered_map<int, Item> itemDictionary = {
		//These down here dont need to be in any particular order, but it makes our job easier if they're organized by type.
		//I was thinking of reserving the indexes in 100's, so plants got the first 100, then tools got the next 100, and so on.
		//This would help in case we'd like to have new items have a similar id to others of their type, without having
		//to reorder every following item in the list.
		{1,Item{
			.id = 1,
			.name = "Parsnip",
			.type = Vegetable,
			//.stackLim = 64,
			.value = 35,
			//.sprite = LoadTexture("assets/items/parsnip.png")
			}
		},
		{2,Item{
			.id = 2,
			.name = "Carrot",
			.type = Vegetable,
			//.stackLim = 64,
			.value = 50,
			.sprite = LoadTexture("assets/items/carrot.png")
			}
		},
		{101,Item{
			.id = 101,
			.name = "Parsnip Seeds",
			.type = Seed,
			.isStackable = false,
			.value = 10,
			//.sprite = LoadTexture("assets/items/parsnip.png")
			}

		},
		{201,Item{
			.id = 201,
			.name = "Copper Hoe",
			.type = Tool,
			//.sprite = LoadTexture("assets/items/parsnip.png")
			}
		}
		
	};
	


};


