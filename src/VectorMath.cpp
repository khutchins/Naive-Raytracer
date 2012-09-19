#include "VectorMath.h"
#include "Point.h"
#include <math.h>

/*
====================
discrim
	calculates the discriminant of a, b, and c
====================
*/
double discrim(double a, double b, double c) //Discriminant
{
	return b*b - 4*a*c;
}

/*
====================
quadratic
	computes the smallest positive value that satisfies the quadratic equation
====================
*/
double quadratic(double a, double b, double c) //Returns the smallest value (greater than zero) that satisfies the quadratic equation
{
	double t1 = (-1*b - sqrt(b*b - 4*a*c))/2/a;
	if (t1 > 0) return t1;
	else return (-1*b + sqrt(b*b - 4*a*c))/2/a;
}

/*
====================
cosAngle
	returns the cosine of the angle between two vectors
====================
*/
double cosAngle(Vector v1, Vector v2) { //Returns cos(angle) between two vectors
	return v1.dot(v2)/v1.magnitude()/v2.magnitude();
}

/*
====================
angle
	returns the angle between two vectors
====================
*/
double angle(Vector v1, Vector v2) { //Returns the angle between two vectors
	return acos(cosAngle(v1,v2));
}

/*
====================
pointsOnSameSide
	Checks if two points are on the same side of a line segment.  Returns true if 
	same side, false if opposite sides
====================
*/
bool pointsOnSameSideOfLine(Point point1, Point point2, Point vertex1, Point vertex2) {
	Vector side = vertex2 - vertex1;

	Vector crossProduct1 = side.cross(point1 - vertex1);
	Vector crossProduct2 = side.cross(point2 - vertex1);

	//if the cross products point in the same direction, on same side (positive dot product)
	//if they point in opposite directions, on opposite sides (negative dot product)
	return (crossProduct1.dot(crossProduct2) >= 0);
}