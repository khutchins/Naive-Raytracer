#ifndef _POINT_H_
#define _POINT_H_

#include "Vector.h"

typedef struct _Point {
    double x,y,z;
	Vector operator+(_Point right);
	Vector operator-(_Point right);
} Point;

#endif