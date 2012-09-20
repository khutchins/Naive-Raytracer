#include "ObjContainerObject.h"

/*
====================
ContainerObject::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	If the ray doesn't hit the object, returns VectorZero.
====================
*/
Vector ContainerObject::calculateNormalForPoint(Point p, Point raySource) {
	bool objectHit = false;
	Point objectIntersect, intersect;
	SceneObject* closestObject;
	Ray *r = new Ray();
	r->start = raySource;
	r->dir = p-raySource;

	for(size_t i = 0; i < children.size(); i++) {
		if(children[i]->intersect(r,objectIntersect)) {
			if(!objectHit || r->start.comparitiveDistanceFrom(objectIntersect) < (r->start).comparitiveDistanceFrom(intersect)) {
				intersect = objectIntersect;
				objectHit = true;
				closestObject = children[i];
			}
		}
	}
	delete r;
	if(closestObject)	return closestObject->calculateNormalForPoint(p, raySource);
	else				return Vector::VectorZero();
}

/*
====================
ContainerObject::intersect
	Computes intersection between the ContainerObject and the ray, and returns
	the primitive it intersects or NULL if it doesn't intersect along with the 
	point of intersection
====================
*/
SceneObject* ContainerObject::intersect(Ray* r, Point &intersect) {
	SceneObject *closestObject = NULL;
	bool objectHit = false;
	Point objectIntersect;

	for(size_t i = 0; i < children.size(); i++) {
		if(children[i]->intersect(r,objectIntersect)) {
			if(!objectHit || r->start.comparitiveDistanceFrom(objectIntersect) < r->start.comparitiveDistanceFrom(intersect)) {
				intersect = objectIntersect;
				closestObject = children[i];
				objectHit = true;
			}
		}
	}

	return closestObject;
}

/*
====================
ContainerObject::getReflection
	Returns the reflection coefficient
====================
*/
double ContainerObject::getReflection() {
	return children[0]->material.reflection;
}

/*
====================
ContainerObject::getRefraction
	Returns the refraction coefficient
====================
*/
double ContainerObject::getRefraction() {
	return children[0]->material.transparency;
}

/*
====================
ContainerObject::getColor
	Returns the color of the object
====================
*/
Color ContainerObject::getColor() {
	return children[0]->material.color;
}


ContainerObject::~ContainerObject() {
	for(size_t i = 0; i < children.size(); i++) {
		delete children[i];
	}
}