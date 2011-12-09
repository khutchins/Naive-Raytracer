#ifndef _POINT_H_
#define _POINT_H_

#include "Vector.h"

struct Vector;

typedef struct Point {
    double x,y,z;
	Vector operator+(Point &right);
	Vector operator-(Point &right);
	Vector operator+(Vector &right);
	Vector operator-(Vector &right);

	Point operator * (double k);
	friend Point operator * (double k, Point& v);
} Point;

#endif