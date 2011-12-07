#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Point.h"
#include "Material.h"
#include "Color.h"
#include "Vector.h"
#include "Point.h"

typedef struct Sphere {
    Point center;
    Material material;
    double radius;

	Vector calculateNormalForPoint(Point p);
} Sphere;

#endif