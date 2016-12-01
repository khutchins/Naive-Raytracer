#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "Point.h"
#include <string>

struct Point;

using namespace std;

struct Vector {
	double x,y,z;

	Vector();
	Vector(double x, double y, double z);
	static Vector VectorZero();

	void normalize();
	Vector perpendicularVector();
	Vector cross(const double x, const double y, const double z);
	Vector cross(const Vector other);
	double dot(const double x, const double y, const double z);
	double dot(const Vector other);
	double magnitude();
	double magnitude2(); //magnitude squared
	double distanceFrom(const Vector other);
	double comparativeDistanceFrom(const Vector other); //distance for comparison (not actual distance)
	double cosAngleBetween(const Vector other);
	double angleBetween(const Vector other);

	void print();

	Vector operator+(Vector right);
	Vector operator-(Vector right);
	Point operator+(Point right);
	Point operator-(Point right);
	Vector operator * (double k);
	friend Vector operator * (double k, Vector v);

};

#endif
