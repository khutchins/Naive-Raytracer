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

	void print();

	Vector operator+(Vector right);
	Vector operator-(Vector right);
	Point operator+(Point right);
	Point operator-(Point right);
	Vector operator * (double k);
	friend Vector operator * (double k, Vector v);

};

#endif
