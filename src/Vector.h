#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "Point.h"

struct Point;

struct Vector {
	double x,y,z;

	Vector();
	Vector(double x, double y, double z);

	Vector operator+(Vector &right);
	Vector operator-(Vector &right);
	Vector operator+(Point &right);
	Vector operator-(Point &right);
	Vector operator * (double k);
	friend Vector operator * (double k, Vector& v);

};

#endif