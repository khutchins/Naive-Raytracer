#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "Ray.h"
#include "Point.h"

typedef enum EntityID {
	SPHERE,
	PLANE,
	LIGHT,
	NONE
};

class SceneObject {
public:
	bool isLight;
	bool isVisible;
	EntityID objectType;

	virtual bool intersect(Ray* r, Point &intersect) {return false;}
};

#endif