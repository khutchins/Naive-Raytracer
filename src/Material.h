#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"

typedef struct _Material {
    Color color;
    double reflection;   // Valid range 0-1
    double transparency; // Valid range 0-1
} Material;

#endif