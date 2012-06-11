#include "ContainerObject.h"

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
	Plane* closestObject;
	Ray *r = new Ray();
	r->start = raySource;
	r->dir = p-raySource;

	for(int i = 0; i < children.size(); i++) {
		if(children[i]->intersect(r,objectIntersect)) {
			if(!objectHit || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
				intersect = objectIntersect;
				objectHit = true;
				closestObject = (Plane *)children[i];
			}
		}
	}
	delete r;
	if(closestObject)	return closestObject->normal;
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
			if(!objectHit || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
				intersect = objectIntersect;
				objectHit = children[i];
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
	return ((Plane *)children[0])->material.reflection;
}

/*
====================
ContainerObject::getRefraction
	Returns the refraction coefficient
====================
*/
double ContainerObject::getRefraction() {
	return ((Plane *)children[0])->material.transparency;
}

/*
====================
ContainerObject::getColor
	Returns the color of the object
====================
*/
Color ContainerObject::getColor() {
	return ((Plane *)children[0])->material.color;
}


~ContainerObject::~ContainerObject() {
	while(SceneObject *result = children->at(0)) {
		free(result);
		children->erase(0);
	}
}