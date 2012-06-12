#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "ObjSceneObject.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Color.h"
#include <fstream>
#include <string>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

class Light : public SceneObject{
public:
	Light(ifstream &f);
	SceneObject* intersect(Ray* r, Point &intersect);
	Color getColor();
};

#endif