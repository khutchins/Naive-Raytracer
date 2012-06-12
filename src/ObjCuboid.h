#ifndef _CUBOID_H_
#define _CUBOID_H_

#include "ObjContainerObject.h"
#include "ObjSceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "ObjPlane.h"
#include "Material.h"
#include "Diagnostic.h"
#include <fstream>
#include <string>
#include <queue>
#include <cmath>

class Cuboid : public ContainerObject {
public:
	Vector	up;
	Vector	front;
	Point	origin;
	double	height;
	double	width;
	double	length;

	Cuboid(ifstream &f);
};

#endif
