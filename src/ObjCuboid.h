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
	double	height;
	double	width;
	double	length;

	explicit Cuboid(ifstream &f);
	Cuboid(Point origin, Vector up, Vector front, double width, double height, double length, Material material, string textureName = string());

private:
	void sharedInit(Point origin, Vector up, Vector front, double width, double height, double length, Material material, string textureName = string());
};

#endif
