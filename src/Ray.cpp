#include "Ray.h"

/*
====================
Ray::Ray
	Initializes a ray with a point zero and direction zero
====================
*/
Ray::Ray() {
	this->start = Point::PointZero();
	this->dir	= Vector::VectorZero();
}

/*
====================
Ray::Ray
	Initializes a ray with a point start and vector direction
====================
*/
Ray::Ray(Point start, Vector direction) {
	this->start = start;
	this->dir = direction;
}