#include "entities.h"

void TileEntity::Render() {
	DrawTextureRec(*spriteSheet, textureSource, renderPos, WHITE);
};

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
		textureSource = Rectangle(0, 0, 0, 0);
		maxGrowthStage = 0;
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


Bed::Bed() {
	textureSource = { 0, 64, 64, 128 };
}

Bed::~Bed() {
}

void Bed::UpdateNewDay() {
	return;
}

void Bed::Interact(Inventory* inventory) {
	map->UpdateNewDay();
}

MarketStall::MarketStall(Texture2D* spriteSheeti, Vector2 renderPosi) {
	spriteSheet = spriteSheeti;
	textureSource = Rectangle{ 0, 0, float(spriteSheet->width), float(spriteSheet->height)};
	renderPos = Vector2(renderPosi.x - spriteSheet->width * 0.4f, renderPosi.y - spriteSheet->height * 0.55f);
	position = renderPosi;
}

MarketStall::~MarketStall(){
}

void MarketStall::Interact(Inventory* inventory) {
	std::cout << "GOOD SELL ITS WORKING" << std::endl;
	for(int i = 0; i < inventory->invLim; i++)
	{
		if (inventory->slot[i] != nullptr && inventory->slot[i]->isSellable) {
			inventory->currMoney += inventory->slot[i]->currStack * inventory->slot[i]->value;
			delete inventory->slot[i];
			inventory->slot[i] = nullptr;
			

		}
	}
}

