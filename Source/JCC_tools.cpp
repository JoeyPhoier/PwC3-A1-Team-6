#include "raylib.h"
#include "JCC_tools.h"
#include <iostream>

void jccDrawTexture(Texture2D texture, int width, int y) {

}


Vector2 NormalVect(Vector2 vector) {
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	vector.x = vector.x / length;
	vector.y = vector.y / length;
	return vector;
}

Vector2 NormalVect(float x, float y) {
	float length = sqrt((x * x) + (y * y));
	x = x / length;
	y = y / length;
	return Vector2(x, y);
}