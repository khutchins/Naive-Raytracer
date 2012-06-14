#ifndef _COLOR_H_
#define _COLOR_H_

#include "EasyBMP/EasyBMP.h"

typedef class Color {
public:
    double r,g,b;        // Valid range 0-1

	Color();
	Color(double r, double g, double b);
	static Color ColorBlack();
	static Color ColorWhite();
	static Color ColorGrayscale(unsigned char whiteness);

	void adjustColorForDiagnosticIsLit();
	static Color averageValues(Color* colors, int numValues);
	static Color colorFromRGBAPixel(RGBApixel *pix);
	RGBApixel RGBAPixel();
	unsigned char grayscaleValue();

	Color operator+(Color right);
	Color operator-(Color right);
	Color operator * (double k);
	Color operator * (Color right);
	friend Color operator * (double k, Color v);
} Color;

#endif
