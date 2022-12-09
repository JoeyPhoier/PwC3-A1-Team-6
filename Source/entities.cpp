#include "entities.h"




Plant::Plant(int idi, Texture2D* texturei) {
	switch (idi) {
	case 101:

		break;
	case 102:
		id = id;
		texture = texturei;
		textureSource = Rectangle(0, 64, 64, 64);
		maxGrowthStage = 12;
		break;
	default:

		break;
	}
}

void Plant::Update() {
	return;
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

void Plant::Render() {
	DrawTextureRec(*texture, textureSource, renderPos, WHITE);
}