#include "Color.h"

/*
====================
Color::Color
	Returns a black color object
====================
*/
Color::Color() {
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

/*
====================
Color::Color
	Returns a color with the provided rgb values
====================
*/
Color::Color(double r, double g, double b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

/*
====================
Color::ColorBlack
	Returns a black color object
====================
*/ 
Color Color::ColorBlack() {
	Color black;
	black.r = 0;
	black.g = 0;
	black.b = 0;
	return black;
}

/*
====================
Color::ColorWhite
	Returns a white color object
====================
*/ 
Color Color::ColorWhite() {
	Color white;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	return white;
}


/*
====================
Color::ColorGrayscale
	Returns an object with the specified grayscale (unsigned char)
====================
*/
Color Color::ColorGrayscale(unsigned char whiteness) {
	Color col;
	col.r = whiteness;
	col.g = whiteness;
	col.b = whiteness;
	return col;
}


/*
====================
Color::adjustColorForDiagnosticIsLit
	Adjusts the color values for the IS_LIT diagnostic to be either white 
	(if there is any color on the pixel) or black (if there isn't)
====================
*/
void Color::adjustColorForDiagnosticIsLit() {
	if(r > 0 || g > 0 || b > 0) {
		r = 255;
		g = 255;
		b = 255;
	}
}

/*
====================
Color::averageValues
	Averages the provided colors and returns the result
====================
*/
Color Color::averageValues(Color* colors, int numValues) {
	Color result = ColorBlack();
	for(int i = 0; i < numValues; i++) {
		result = result + colors[i];
	}
	double divNum = 1.f/numValues;
	result = result * divNum;
	return result;
}

/*
====================
Color::colorFromRGBAPixel
	Converts RGBApixels (used for EasyBMP) to a Color object
====================
*/
Color Color::colorFromRGBAPixel(RGBApixel *pix) {
	Color col;
	col.r = pix->Red;
	col.g = pix->Green;
	col.b = pix->Blue;
	return col;
}

/*
====================
Color::RGBAPixel()
	This converts the color object into an RGBApixel (used for EasyBMP)
====================
*/
RGBApixel Color::RGBAPixel() {
	RGBApixel pix;
	pix.Alpha = 255;
	pix.Red =	(unsigned char)this->r;
	pix.Green = (unsigned char)this->g;
	pix.Blue =	(unsigned char)this->b;

	return pix;
}

/*
====================
Color::grayscaleValue()
	Returns the grayscale value of the current color as an unsigned char (0-255)
====================
*/
unsigned char Color::grayscaleValue() {
	return this->r * 0.3 + this->g * 0.59 + this->b * 0.11;
}

//Color with color addition
Color Color::operator+(Color right)
{
    Color result;
    result.r = this->r + right.r;
	result.g = this->g + right.g;
	result.b = this->b + right.b;
    return result;
}

//Color with color subtraction
Color Color::operator-(Color right)
{
    Color result;
    result.r = this->r - right.r;
	result.g = this->g - right.g;
	result.b = this->b - right.b;
    return result;
}

//Color with double multiplication
Color Color::operator *(double k) {
	Color result;
	result.r = this->r * k;
	result.g = this->g * k;
	result.b = this->b * k;
	return result;
}

//Color with color multiplication
Color Color::operator *(Color right) {
	Color result;
	result.r = r * right.r;
	result.g = g * right.g;
	result.b = b * right.b;
	return result;
}

//Double with color multiplication
Color operator * (double k, Color v) {
	return v*k;
}
