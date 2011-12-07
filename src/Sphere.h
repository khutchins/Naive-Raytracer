#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Point.h"
#include "Material.h"
#include "Color.h"

typedef struct _Sphere {
    Point center;
    Material material;
    double radius;
} Sphere;

#endif