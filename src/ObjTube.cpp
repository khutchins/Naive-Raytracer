#include "ObjTube.h"

/*
====================
Tube::Tube
	Takes in the input stream and creates a Tube object from the parsed input
====================
*/
Tube::Tube(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_TUBE;
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
		if(word == "origin" || word == "color" || word == "up") {
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

			if(word == "origin") this->origin = Point(num1,num2,num3);
			else if(word == "color") this->material.color = Color(num1,num2,num3);
			else if(word == "up") {
				this->up = Vector(num1,num2,num3);
				this->up.normalize();
			}
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency" || word == "height") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius")			this->radius = num1;
			else if(word == "reflect")		this->material.reflection = num1;
			else if(word == "transparency") this->material.transparency = num1;
			else if(word == "height")		this->height = num1;
		}
		else break;
	}
}

/*
====================
Tube::Tube
	Takes information for creation from parameters instead of input stream 
	so that it can be created by a complex object
====================
*/
Tube::Tube(Material m, double radius, double height, Vector up, Point origin) {
	this->material = m;
	this->radius = radius;
	this->height = height;
	this->up = up;
	this->origin = origin;
}

/*
====================
Tube::intersect
	Computes intersection between the Tube and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Tube::intersect(Ray* r, Point &intersect) {
	Vector dist = this->origin - r->start;

	//norm(dist);
	//norm(r->dir);

	//double a = dot3(r->dir,r->dir);
	//double b = 2 * dot3(r->start - this->origin,r->dir);
	//double c = dot3(r->start - this->origin,r->start - this->origin) - this->radius * this->radius;

	//double disc = discrim(a,b,c);

	////Parallel to tube, does not intersect
	//if(dot3(r->dir,this->up) == 0) return false;
	//else if(disc >= 0) { //Find closest intersection
	//	double discSqrt = sqrt(disc);
	//	double quad;
	//	if (b < 0)	quad = (-b - discSqrt)/2.f;
	//	else quad = (-b + discSqrt)/2.f;

	//	double t0 = quad/a;
	//	double t1 = c/quad;
	//	if(t0 > t1) swap(t0,t1);

	//	double t;
	//	if(t0 < 0 && t1 < 0) return false;
	//	if(t0 < 0) t = t1;
	//	else t = t0;

	//	intersect = r->start + t * r->dir;
	//	return this;
	//}
	//return NULL;
	
	/* 
	Ray: O + V * t
	Cylinder: [this->origin,this->up,this->radius]
	A = this->origin
	O = r->start
	V = r->dir
	*/

	Vector AB = this->up;
	Vector AO = r->start - this->origin;
	Vector AOxAB = AO.cross(AB);
	Vector VxAB = r->dir.cross(AB);
	double ab2 = AB.dot(AB);

	double a = VxAB.dot(VxAB);
	double b = 2 * VxAB.dot(AOxAB);
	double c = AOxAB.dot(AOxAB) - this->radius*this->radius * ab2;

	double t = quadratic(a,b,c);
	if(t < 0) return NULL;

	intersect = r->start + t * r->dir;
	return this;
}

/*
====================
Tube::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
====================
*/
Vector Tube::calculateNormalForPoint(Point p, Point raySource) {
	//TODO: this is not correct, fix
	//return p - this->origin;
	return Vector::VectorZero();
}

/*
====================
Tube::getReflection
	Returns the reflection coefficient
====================
*/
double Tube::getReflection() {
	return this->material.reflection;
}

/*
====================
Tube::getRefraction
	Returns the refraction coefficient
====================
*/
double Tube::getRefraction() {
	return this->material.transparency;
}

/*
====================
Tube::getColor
	Returns the color of the object
====================
*/
Color Tube::getColor() {
	return this->material.color;
}