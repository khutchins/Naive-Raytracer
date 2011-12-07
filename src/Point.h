#ifndef _POINT_H_
#define _POINT_H_

#include "Vector.h"

typedef struct Point {
    double x,y,z;
	Vector operator+(Point right);
	Vector operator-(Point right);
} Point;

#endif