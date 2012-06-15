#ifndef _RAY_H_
#define _RAY_H_

#include "Point.h"
#include "Vector.h"

struct Ray {
	Point start;
	Vector dir;

	Ray();
	Ray(Point start, Vector direction);
};

#endif