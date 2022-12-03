#include "Inventory.h"
#include <vector>
#include <iostream>
#include "raylib.h"

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
		slot[firstEmptyIndex] = new Item{ itemDictionary[id] };
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


