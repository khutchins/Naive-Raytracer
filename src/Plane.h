#ifndef _PLANE_H_
#define _PLANE_H_

#include "Point.h"
#include "Material.h"
#include "EasyBMP\EasyBMP.h"

typedef struct _Plane {
    Point center;
    Vector normal;
    Vector up;
    double width;
    double height;
    bool hastexture;
    Material material;
    BMP texture;
} Plane;

#endif