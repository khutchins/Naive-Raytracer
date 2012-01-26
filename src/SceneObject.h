#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include <cstdlib>

enum EntityID {
	SPHERE,
	PLANE,
	LIGHT,
	DISK,
	CYLINDER,
	TUBE,
	CUBOID,
	NONE
};

class SceneObject {
public:
	Point origin;
	bool isLight;
	bool isVisible;
	bool hasTexture; //Only can be true for planes currently
	EntityID objectType;

	virtual bool intersect(Ray* r, Point &intersect) {return false;}
	virtual Vector calculateNormalForPoint(Point p, Point raySource) {return Vector::VectorZero();}
	virtual double getReflection() { return 0; }
	virtual double getRefraction() { return 0; }
	virtual Color getColor() { return Color::ColorBlack(); }
	virtual Color calculateTextureFromMaterial(Point intercept) { return Color::ColorBlack(); }
};

#endif
