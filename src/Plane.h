#ifndef _PLANE_H_
#define _PLANE_H_

#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include <fstream>
#include <string>
#include <queue>
#include "EasyBMP\EasyBMP.h"

using namespace std;

class Plane {
public:
    Point center;
    Vector normal;
    Vector up;
    double width;
    double height;
    bool hastexture;
    Material material;
    BMP texture;

	Plane(std::ifstream &f);
};

#endif