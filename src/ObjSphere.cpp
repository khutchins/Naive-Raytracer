#include "ObjSphere.h"

/*
====================
Sphere::Sphere
	Takes in the input stream and creates a Sphere object from the parsed input
====================
*/
Sphere::Sphere(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_SPHERE;
	this->hasTexture = false;
	while(!f.eof()) {
		string line;
		getline(f,line);

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		//words with three string arguments
		if(word == "origin" || word == "color") {
			double num1 = 0;
			double num2 = 0;
			double num3 = 0;

			if(lineContents.size() < 3) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num3 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "origin")		this->origin = Point(num1,num2,num3);
			else if(word == "color")	this->material.color = Color(num1,num2,num3);
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius")			this->radius = num1;
			else if(word == "reflect")		this->material.reflection = num1;
			else if(word == "transparency")	this->material.transparency = num1;
		}
		else break;
	}
}

/*
====================
Sphere::intersect
	Computes intersection between the Sphere and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Sphere::intersect(Ray* r, Point &intersect) {
	Vector dist = this->origin - r->start;

	dist.normalize();
	r->dir.normalize();

	double a = r->dir.dot(r->dir);
	double b = 2 * (r->start - this->origin).dot(r->dir);
	double c = (r->start - this->origin).dot(r->start - this->origin) - this->radius * this->radius;

	double disc = discrim(a,b,c);

	//No intersection, do nothing
	if(disc < 0) return false;
	else if(disc >= 0) { //Find closest intersection
		double discSqrt = sqrt(disc);
		double quad;
		if (b < 0)	quad = (-b - discSqrt)/2.f;
		else quad = (-b + discSqrt)/2.f;

		double t0 = quad/a;
		double t1 = c/quad;
		if(t0 > t1) swap(t0,t1);

		double t;
		if(t0 < 0 && t1 < 0) return false;
		if(t0 < 0) t = t1;
		else t = t0;

		intersect = r->start + t * r->dir;
		return this;
	}
	return NULL;
}

/*
====================
Sphere::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	I assume an infinitely wide Sphere, so this will always return the normal 
	as if the ray hit the Sphere
====================
*/
Vector Sphere::calculateNormalForPoint(Point p, Point raySource) {
	return p - this->origin;
}

/*
====================
Sphere::getReflection
	Returns the reflection coefficient
====================
*/
double Sphere::getReflection() {
	return this->material.reflection;
}

/*
====================
Sphere::getRefraction
	Returns the refraction coefficient
====================
*/
double Sphere::getRefraction() {
	return this->material.transparency;
}

/*
====================
Sphere::getColor
	Returns the color of the object (ignoring texture color)
====================
*/
Color Sphere::getColor() {
	return this->material.color;
}