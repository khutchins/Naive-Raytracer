#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "SceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Diagnostic.h"
#include "Disk.h"
#include "Tube.h"
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>

using namespace std;

class Cylinder : public SceneObject {
public:
	Disk *top;
	Disk *bottom;
	Tube *tube;
	Vector up;
	Material material;

	Cylinder(ifstream &f);
	Vector calculateNormalForPoint(Point p, Point raySource);
	bool intersect(Ray* r, Point &intersect);
	double getReflection();
	double getRefraction();
	Color getColor();
};

#endif
