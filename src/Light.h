#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Point.h"
#include "Color.h"

typedef struct Light {
    Point origin;
    Color color;
} Light;

#endif