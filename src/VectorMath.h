#ifndef _VECTOR_MATH_
#define _VECTOR_MATH_

#include "Vector.h"

double dot3(double x1,double x2,double y1,double y2,double z1,double z2); //Dot product for 3D
double dot2(double x1,double x2,double y1,double y2); //Dot product for 2D
double dist3(double x1,double x2,double y1,double y2,double z1,double z2); //3D distance
double discrim(double a, double b, double c); //Discriminant
double quadratic(double a, double b, double c); //Returns the smallest value that satisfies the quadratic equation
double quadratic2(double a, double b, double c); 
Vector cross3(double x1, double x2, double y1, double y2, double z1, double z2); //Returns the vector of the cross product of 2 3D vectors
void norm(double &x, double &y, double &z); //Returns the normalized vector by reference

#endif