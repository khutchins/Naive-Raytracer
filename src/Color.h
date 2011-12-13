#ifndef _COLOR_H_
#define _COLOR_H_

typedef class Color {
public:
    double r,g,b;        // Valid range 0-1

	Color();
	Color(double r, double g, double b);
	static Color ColorBlack();
	static Color ColorWhite();

	void adjustColorForDiagnosticIsLit();
	static Color averageValues(Color* colors, int numValues);
} Color;

#endif