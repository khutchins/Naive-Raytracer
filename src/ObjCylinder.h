#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "ObjContainerObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Diagnostic.h"
#include "ObjDisk.h"
#include "ObjTube.h"
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>

using namespace std;

class Cylinder : public ContainerObject {
public:
	Vector up;

	Cylinder(ifstream &f);
	~Cylinder();
};

#endif
