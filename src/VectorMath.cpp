#include "VectorMath.h"
#include "Point.h"
#include <math.h>

double dot3(double x1,double x2,double y1,double y2,double z1,double z2) //Dot product for 3D
{
	return x1*x2 + y1*y2 + z1*z2;
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

double discrim(double a, double b, double c) //Discriminant
{
	return b*b - 4*a*c;
}

double quadratic(double a, double b, double c) //Returns the smallest value that satisfies the quadratic equation
{
	return (-1*b - sqrt(b*b - 4*a*c))/2/a;
}

double quadratic2(double a, double b, double c) 
{
	return (-1 * b / a) - sqrt(b*b - a*c)/a;
}

Vector cross3(double x1, double x2, double y1, double y2, double z1, double z2) //Returns the vector of the cross product of 2 3D vectors
{
	Vector v;
	v.x = y1*z2 - z1*y2;
	v.y = z1*x2 - x1*z2;
	v.z = x1*y2 - y1*x2;
	return v;
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
