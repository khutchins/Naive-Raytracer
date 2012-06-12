#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Material.h"
#include <cstdlib>

enum EntityID {
	ENTITY_SPHERE,
	ENTITY_PLANE,
	ENTITY_LIGHT,
	ENTITY_DISK,
	ENTITY_CYLINDER,
	ENTITY_TUBE,
	ENTITY_CUBOID,
	ENTITY_NONE
};

class SceneObject {
public:
	Point origin;
	bool isLight;
	bool isVisible;
	bool hasTexture; //Only can be true for planes currently
	EntityID objectType;
	Material material;

	virtual SceneObject* intersect(Ray* r, Point &intersect) {return NULL;}
	virtual Vector calculateNormalForPoint(Point p, Point raySource) {return Vector::VectorZero();}
	virtual double getReflection() { return 0; }
	virtual double getRefraction() { return 0; }
	virtual Color getColor() { return Color::ColorBlack(); }
	virtual Color calculateTextureFromMaterial(Point intercept) { return Color::ColorBlack(); }
};

#endif
