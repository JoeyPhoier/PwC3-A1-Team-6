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
		maxGrowthStage = 12;
		break;
	case 102:
		textureSource = Rectangle(0, 64, 64, 64);
		maxGrowthStage = 6;
		break;
	case 103:
		textureSource = Rectangle(0, 128, 64, 64);
		maxGrowthStage = 4;
		break;
	case 104:
		textureSource = Rectangle(0, 192, 64, 64);
		maxGrowthStage = 8;
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

SellingStall::SellingStall(Texture2D* spriteSheeti, Vector2 renderPosi) {
	spriteSheet = spriteSheeti;
	textureSource = Rectangle{ 0, 0, float(spriteSheet->width), float(spriteSheet->height)};
	renderPos = Vector2(renderPosi.x - spriteSheet->width * 0.4f, renderPosi.y - spriteSheet->height * 0.55f);
	position = renderPosi;
}

SellingStall::~SellingStall(){
}

void SellingStall::Interact(Inventory* inventory) {
	for(int i = 0; i < inventory->invLim; i++)
	{
		if (inventory->slot[i] != nullptr && inventory->slot[i]->isSellable) {
			inventory->currMoney += inventory->slot[i]->currStack * inventory->slot[i]->sellValue;
			delete inventory->slot[i];
			inventory->slot[i] = nullptr;
			
		}
	}
}

BuyingStall::BuyingStall(Texture2D* spriteSheeti, Vector2 renderPosi, int seedIdi) {
	spriteSheet = spriteSheeti;
	seedId = seedIdi;
	int spritex = 0;
	switch (seedIdi) {
	case 101:
		spritex = 0;
		break;
	case 102:
		spritex = 128;
		break;
	case 103:
		spritex = 64;
		break;
	case 104:
		spritex = 192;
		break;
	}
	textureSource = Rectangle{ float(spritex), 0, 64, float(spriteSheet->height) };
	renderPos = Vector2(renderPosi.x, renderPosi.y);
	position = renderPosi;
}

void BuyingStall::Interact(Inventory* inventory) {
	if (inventory->currMoney >= inventory->itemDictionary[seedId]->buyValue) {

		std::cout << inventory->itemDictionary[seedId]->buyValue << std::endl;
		inventory->currMoney -= inventory->itemDictionary[seedId]->buyValue;
		inventory->AddItem(seedId);
	}
}

