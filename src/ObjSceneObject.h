#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Material.h"
#include <cstdlib>
#include <memory>
#include <vector>

enum EntityID {
	ENTITY_SPHERE,
	ENTITY_INFPLANE,
	ENTITY_PLANE,
	ENTITY_LIGHT,
	ENTITY_DISK,
	ENTITY_CYLINDER,
	ENTITY_TUBE,
	ENTITY_CUBOID,
	ENTITY_TRIANGLE,
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
	virtual Color calculateTextureFromMaterial(Point intercept, bool diagnosticEnabled) { return Color::ColorBlack(); }
	virtual std::vector<std::unique_ptr<Ray>> raysForLighting(Point origin) { std::vector<std::unique_ptr<Ray>> rays; return rays; };
};

#endif
