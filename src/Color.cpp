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