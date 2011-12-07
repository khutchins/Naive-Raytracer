#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Point.h"

typedef struct _Camera {
    Point origin;
    Point direction;
    Point up;
    double zmin, zmax; // zmin is also the location of the image plane
    double width;      // Width at zmin, the image plane
    bool  perspective;
} Camera;

#endif