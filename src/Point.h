#ifndef _POINT_H_
#define _POINT_H_

#include "Vector.h"

struct Vector;

struct Point {
	static Point PointZero();

	Point();
	Point(double x, double y, double z);

    double x,y,z;
	//Vector operator+(Point right);
	Vector operator-(Point right);
	Point operator+(Vector right);
	Point operator-(Vector right);

	Point operator * (double k);
	friend Point operator * (double k, Point v);
};

#endif
