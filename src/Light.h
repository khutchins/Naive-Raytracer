#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "StringProcessing.h"
#include "Point.h"
#include "Color.h"
#include <fstream>
#include <string>
#include <queue>

using namespace std;

typedef struct Light {
    Point origin;
    Color color;

	Light(ifstream &f);
} Light;

#endif