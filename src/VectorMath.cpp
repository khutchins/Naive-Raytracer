#include "VectorMath.h"
#include "Point.h"
#include <math.h>

double dot3(double x1,double x2,double y1,double y2,double z1,double z2) //Dot product for 3D
{
	return x1*x2 + y1*y2 + z1*z2;
}
double dot3(Vector v1, Vector v2) {
	return dot3(v1.x,v2.x,v1.y,v2.y,v1.z,v2.z);
}

double dot2(double x1,double x2,double y1,double y2) //Dot product for 2D
{
	return dot3(x1,x2,y1,y2,0,0);
}

double dist3(double x1,double x2,double y1,double y2,double z1,double z2) //3D distance
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

double dist3(Point x1, Point x2) {
	return sqrt((x2.x-x1.x)*(x2.x-x1.x) + (x2.y-x1.y)*(x2.y-x1.y) + (x2.z-x1.z)*(x2.z-x1.z));
}

double dist3Compare(Point x1, Point x2) { //Gives the square of the distance.  Just used for comparisons because it's faster
	return (x2.x-x1.x)*(x2.x-x1.x) + (x2.y-x1.y)*(x2.y-x1.y) + (x2.z-x1.z)*(x2.z-x1.z);
}

double discrim(double a, double b, double c) //Discriminant
{
	return b*b - 4*a*c;
}

double quadratic(double a, double b, double c) //Returns the smallest value (greater than zero) that satisfies the quadratic equation
{
	double t1 = (-1*b - sqrt(b*b - 4*a*c))/2/a;
	if (t1 > 0) return t1;
	else return (-1*b + sqrt(b*b - 4*a*c))/2/a;
}

Vector cross3(double x1, double x2, double y1, double y2, double z1, double z2) //Returns the vector of the cross product of 2 3D vectors
{
	Vector v;
	v.x = y1*z2 - z1*y2;
	v.y = z1*x2 - x1*z2;
	v.z = x1*y2 - y1*x2;
	return v;
}

Vector cross3(Vector v1, Vector v2) {
	return cross3(v1.x,v2.x,v1.y,v2.y,v1.z,v2.z);
}

Vector vectorPerpendicularTo(Vector original) {
	if(original.x == 0 && original.y == 0 && original.z == 0) {
		Vector zeroVector;
		zeroVector.x = 0;
		zeroVector.y = 0;
		zeroVector.z = 0;
		return zeroVector;
	}
	if(original.x <= original.y && original.x <= original.z) {
		return cross3(original.x,1,original.y,0,original.z,0);
	}
	else if(original.y <= original.x && original.y <= original.z) {
		return cross3(original.x,0,original.y,1,original.z,0);
	}
	else return cross3(original.x,0,original.y,0,original.z,1);
}

void norm(double &x, double &y, double &z) //Returns the normalized vector by reference
{
	if(x != 0 || y != 0 || z != 0)
	{
		double n = sqrt(x*x+y*y+z*z);
		x /= n;
		y /= n;
		z /= n;
	}
}

void norm(Vector &v) //Returns the normalized vector by reference
{
	norm(v.x,v.y,v.z);
}

double magnitude(Vector v) { //Returns the magnitude of a vector
	return sqrt(dot3(v,v));
}

double magnitude2(Vector v) { //Returns the magnitude of a vector squared {
	return dot3(v,v);
}

double cosAngle(Vector v1, Vector v2) { //Returns cos(angle) between two vectors
	return dot3(v1,v2)/magnitude(v1)/magnitude(v2);
}

double angle(Vector v1, Vector v2) { //Returns the angle between two vectors
	return acos(cosAngle(v1,v2));
}
