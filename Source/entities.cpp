#include "entities.h"




Plant::Plant(int idi, Texture2D* spriteSheeti) {
	switch (idi) {
	case 101:

		break;
	case 102:
		id = id;
		spriteSheet = spriteSheeti;
		textureSource = Rectangle(0, 64, 64, 64);
		maxGrowthStage = 12;
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
		}
		
		
	}
	
}

void Plant::Interact() {
	if (growthStage < maxGrowthStage) return;
	// Remove plant entity and give the player the item
}

void Plant::Render() {
	DrawTextureRec(*spriteSheet, textureSource, renderPos, WHITE);
}


void Bed::UpdateNewDay() {
	return;
}

void Bed::Interact() {
	map->UpdateNewDay();
}

void Bed::Render() {
	DrawTextureRec(*spriteSheet, textureSource, renderPos, WHITE);
}