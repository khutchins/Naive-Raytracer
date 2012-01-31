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
		result = result + colors[i];
	}
	double divNum = 1.f/numValues;
	result = result * divNum;
	return result;
}

Color Color::colorFromRGBAPixel(RGBApixel *pix) {
	Color col;
	col.r = pix->Red;
	col.g = pix->Green;
	col.b = pix->Blue;
	return col;
}

Color Color::operator+(Color right)
{
    Color result;
    result.r = this->r + right.r;
	result.g = this->g + right.g;
	result.b = this->b + right.b;
    return result;
}

Color Color::operator-(Color right)
{
    Color result;
    result.r = this->r - right.r;
	result.g = this->g - right.g;
	result.b = this->b - right.b;
    return result;
}

Color Color::operator *(double k) {
	Color result;
	result.r = this->r * k;
	result.g = this->g * k;
	result.b = this->b * k;
	return result;
}

Color Color::operator *(Color right) {
	Color result;
	result.r = r * right.r;
	result.g = g * right.g;
	result.b = b * right.b;
	return result;
}

Color operator * (double k, Color v) {
	return v*k;
}
