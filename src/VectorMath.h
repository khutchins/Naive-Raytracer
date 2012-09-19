#ifndef _VECTOR_MATH_
#define _VECTOR_MATH_

#include "Vector.h"
#include "Point.h"

double discrim(double a, double b, double c); //Discriminant
double quadratic(double a, double b, double c); //Returns the smallest value that satisfies the quadratic equation

bool pointsOnSameSideOfLine(Point point1, Point point2, Point vertex1, Point vertex2); //Checks if both points are on the same side of a line determined by vertex1 and vertex2

#endif
