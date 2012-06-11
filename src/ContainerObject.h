#ifndef _CONTAINER_OBJECT_H_
#define _CONTAINER_OBJECT_H_

#include "SceneObject.h"
#include "VectorMath.h"
#include <vector>

using namespace std;

class ContainerObject : public SceneObject {
public:
	vector<SceneObject *> children;

	//Inherited from SceneObject
	virtual SceneObject* intersect(Ray* r, Point &intersect);
	virtual Vector calculateNormalForPoint(Point p, Point raySource);
	virtual double getReflection();
	virtual double getRefraction();
	virtual Color getColor();
	//virtual Color calculateTextureFromMaterial(Point intercept);

	//New methods
	~ContainerObject();
};

#endif