#ifndef _CONTAINER_OBJECT_H_
#define _CONTAINER_OBJECT_H_

#include "ObjSceneObject.h"
#include "VectorMath.h"
#include <vector>

using namespace std;

class ContainerObject : public SceneObject {
public:
	vector<SceneObject *> children;

	//Inherited from SceneObject
	SceneObject* intersect(Ray* r, Point &intersect) override;
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

	//New methods
	~ContainerObject();
};

#endif