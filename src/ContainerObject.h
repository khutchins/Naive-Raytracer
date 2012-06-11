#ifndef _CONTAINER_OBJECT_H_
#define _CONTAINER_OBJECT_H_

#include "SceneObject.h"
#include <vector>

using namespace std;

class ContainerObject : public SceneObject {
public:
	vector<SceneObject *> children;

	//Inherited from SceneObject
	virtual SceneObject* intersect(Ray* r, Point &intersect) {return NULL;}
	virtual Vector calculateNormalForPoint(Point p, Point raySource) {return Vector::VectorZero();}
	virtual double getReflection() { return 0; }
	virtual double getRefraction() { return 0; }
	virtual Color getColor() { return Color::ColorBlack(); }
	virtual Color calculateTextureFromMaterial(Point intercept) { return Color::ColorBlack(); }

	//New methods
	//~ContainerObject();
};

#endif