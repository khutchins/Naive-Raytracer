#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "StringProcessing.h"
#include "Point.h"
#include <fstream>
#include <string>
#include <queue>

using namespace std;

typedef struct Camera {
    Point origin;
    Vector direction;
    Vector up;
    double zmin, zmax; // zmin is also the location of the image plane
    double width;      // Width at zmin, the image plane
    bool perspective;
	bool grayscale;

	Camera(ifstream &f);
} Camera;

#endif