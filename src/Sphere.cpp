#include "Sphere.h"

Vector Sphere::calculateNormalForPoint(Point p) {
	Vector normal;
	normal.x = p.x - this->center.x;
	normal.y = p.y - this->center.y;
	normal.z = p.z - this->center.z;
	return normal;
}