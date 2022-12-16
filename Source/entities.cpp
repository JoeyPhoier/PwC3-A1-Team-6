#include "entities.h"




Plant::Plant(int idi, Texture2D* spriteSheeti, Vector2 positioni, Tile* parenti) {
	id = idi;
	lootId = idi - 100;
	position = positioni;
	renderPos = positioni;
	spriteSheet = spriteSheeti;
	parent = parenti;
	switch (idi) {
	case 101:
		textureSource = Rectangle(0, 0, 64, 64);
		maxGrowthStage = 16;
		break;
	case 102:
		textureSource = Rectangle(0, 64, 64, 64);
		maxGrowthStage = 12;
		break;
	case 103:
		textureSource = Rectangle(0, 128, 64, 64);
		maxGrowthStage = 9;
		break;
	case 104:
		textureSource = Rectangle(0, 192, 64, 64);
		maxGrowthStage = 10;
		break;
	default:

		break;
	}
}

void Plant::UpdateNewDay() {
	if (parent->isWet && growthStage < maxGrowthStage) {
		growthStage++;
		if (growthStage == maxGrowthStage) {
			textureSource.x = 4 * 64;
		}
		else if (growthStage >= (3 * maxGrowthStage) / 4) {
			textureSource.x = 3 * 64;
		}
		else if (growthStage >= maxGrowthStage / 2) {
			textureSource.x = 2 * 64;
		}
		else if (growthStage == 1) {
			textureSource.x = 64;
			position.y += 48;
		}
		
		
	}
	
}

void Plant::Interact(Inventory* inventory) {
	if (growthStage < maxGrowthStage) return;
	inventory->AddItem(lootId);
	isDead = true;
}

void Plant::Render() {
	DrawTextureRec(*spriteSheet, textureSource, renderPos, WHITE);
}


void Bed::UpdateNewDay() {
	return;
}

void Bed::Interact(Inventory* inventory) {
	map->UpdateNewDay();
}

void Bed::Render() {
	DrawTextureRec(*spriteSheet, textureSource, renderPos, WHITE);
}