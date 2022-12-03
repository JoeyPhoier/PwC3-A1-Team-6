#include "raylib.h"
#include "JCC_tools.h"
#include <iostream>

void jccDrawTexture(Texture2D texture, int width, int y) {

}

float jccTruncate(float number, int digitsAfterZero) {
	int scaler = 1;
	for (int i = 0; i < digitsAfterZero; i++) {
		scaler *= 10;
	}
	number = (trunc(number * scaler)) / scaler;
	return number;
}

Vector2 NormalVect(Vector2 vector) {
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	vector.x = jccTruncate(vector.x / length, 2);				//Truncation fixes the jitteryness.
	vector.y = jccTruncate(vector.y / length, 2);
	return vector;
}

Vector2 NormalVect(float x, float y) {
	float length = sqrt((x * x) + (y * y));
	x = x / length;
	y = y / length;
	return Vector2(x, y);
}