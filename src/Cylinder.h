#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "ContainerObject.h"
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

class Cylinder : public ContainerObject {
public:
	Vector up;
	Material material;

	Cylinder(ifstream &f);
};

#endif
