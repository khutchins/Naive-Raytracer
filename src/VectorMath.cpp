#include "VectorMath.h"
#include "Point.h"
#include <math.h>

/*
====================
dot3
	computes a 3D dot product from individual variables
====================
*/
double dot3(double x1,double x2,double y1,double y2,double z1,double z2)
{
	return x1*x2 + y1*y2 + z1*z2;
}

/*
====================
dot3
	computes a 3D dot product from two vectors
====================
*/
double dot3(Vector v1, Vector v2) {
	return dot3(v1.x,v2.x,v1.y,v2.y,v1.z,v2.z);
}

/*
====================
dot2
	computes a 2D dot product from individual variables
====================
*/
double dot2(double x1,double x2,double y1,double y2)
{
	return dot3(x1,x2,y1,y2,0,0);
}

/*
====================
dist3
	computes the 3D distance between individual variables
====================
*/
double dist3(double x1,double x2,double y1,double y2,double z1,double z2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

/*
====================
dist3
	computes the 3D distance between two points
====================
*/
double dist3(Point x1, Point x2) {
	return sqrt((x2.x-x1.x)*(x2.x-x1.x) + (x2.y-x1.y)*(x2.y-x1.y) + (x2.z-x1.z)*(x2.z-x1.z));
}

/*
====================
dist3Compare
	the square of the 3D distance (faster than normal dist3, used for 
	comparisons)
====================
*/
double dist3Compare(Point x1, Point x2) {
	return (x2.x-x1.x)*(x2.x-x1.x) + (x2.y-x1.y)*(x2.y-x1.y) + (x2.z-x1.z)*(x2.z-x1.z);
}

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
magnitude
	returns the magnitude of a vector (pop pop!)
====================
*/
double magnitude(Vector v) { //Returns the magnitude of a vector
	return sqrt(dot3(v,v));
}

/*
====================
magnitude2
	returns the square of the magnitude (dot product of it with itself)
====================
*/
double magnitude2(Vector v) { //Returns the magnitude of a vector squared
	return dot3(v,v);
}

/*
====================
cosAngle
	returns the cosine of the angle between two vectors
====================
*/
double cosAngle(Vector v1, Vector v2) { //Returns cos(angle) between two vectors
	return dot3(v1,v2)/magnitude(v1)/magnitude(v2);
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
	return (dot3(crossProduct1,crossProduct2) >= 0);
}