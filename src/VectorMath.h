#ifndef _VECTOR_MATH_
#define _VECTOR_MATH_

#include "Vector.h"
#include "Point.h"

double dot3(double x1,double x2,double y1,double y2,double z1,double z2); //Dot product for 3D
double dot3(Vector v1, Vector v2);
double dot2(double x1,double x2,double y1,double y2); //Dot product for 2D
double dist3(double x1,double x2,double y1,double y2,double z1,double z2); //3D distance
double dist3(Point p1, Point p2);
double dist3Compare(Point p1, Point p2); //Distance between two objects squared (faster than dist3)
double discrim(double a, double b, double c); //Discriminant
double quadratic(double a, double b, double c); //Returns the smallest value that satisfies the quadratic equation
Vector cross3(double x1, double x2, double y1, double y2, double z1, double z2); //Returns the vector of the cross product of 2 3D vectors
Vector cross3(Vector v1, Vector v2);
Vector vectorPerpendicularTo(Vector original);
void norm(double &x, double &y, double &z); //Returns the normalized doubles by reference
void norm(Vector &v); //Returns the normalized vector by reference
double magnitude(Vector v); //Returns the magnitude of a vector
double magnitude2(Vector v); //Returns the magnitude of a vector squared
double oldMag(Vector v); //Returns the incorrect vector (actually dot product of v with itself)
double cosAngle(Vector v1, Vector v2); //Returns cosine of the angle between two vectors
double angle(Vector v1, Vector v2); //Returns angle between two vectors

#endif
