#include "Color.h"

Color::Color() {
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

Color::Color(double r, double g, double b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

void Color::adjustColorForDiagnosticIsLit() {
	if(r > 0 || g > 0 || b > 0) {
		r = 255;
		g = 255;
		b = 255;
	}
}

Color Color::ColorBlack() {
	Color black;
	black.r = 0;
	black.g = 0;
	black.b = 0;
	return black;
}

Color Color::ColorWhite() {
	Color white;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	return white;
}

Color Color::averageValues(Color* colors, int numValues) {
	Color result = ColorBlack();
	for(int i = 0; i < numValues; i++) {
		result.r += colors[i].r;
		result.g += colors[i].g;
		result.b += colors[i].b;
	}
	result.r /= numValues;
	result.g /= numValues;
	result.b /= numValues;
	return result;
}