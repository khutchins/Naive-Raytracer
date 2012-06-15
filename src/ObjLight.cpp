#include "ObjLight.h"

/*
====================
Light::Light
	Takes in the input stream and creates a Light object from the parsed input
====================
*/
Light::Light(ifstream &f)
{
	this->isVisible = true;
	this->isLight = true;
	this->objectType = ENTITY_LIGHT;
	this->hasTexture = false;
	this->material.color.r = 0;
	this->material.color.g = 0;
	this->material.color.b = 0;
	this->material.reflection = 0;
	this->material.transparency = 0;
	while(!f.eof()) {
		string line;
		getline(f,line);

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

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
		else break;

	}
}

/*
====================
Light::intersect
	Computes intersection between the Light and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Light::intersect(Ray* r, Point &intersect) {
	//Four cases here
	//tempL->origin.x - r->start.x == 0 && r->dir.x == 0.  This is fine, they intersect at all points.  Assign infinity
	//tempL->origin.x - r->start.x != 0 && r->dir.x == 0.  These will *never* hit. Assign quiet NaN
	//tempL->origin.x - r->start.x == 0 && r->dir.x != 0.  This only hits if all are zero. Assign zero
	//tempL->origin.x - r->start.x != 0 && r->dir.x != 0.  This will hit at the point calculated.  Assign that t
	Vector t; //Increments to each intercept. If they are roughly the same, we intersect the light
	if(r->dir.x == 0 && abs(this->origin.x - r->start.x) < 0.001)	t.x = std::numeric_limits<double>::infinity();
	else if(r->dir.x == 0)											t.x = std::numeric_limits<double>::quiet_NaN();
	else															t.x = (this->origin.x - r->start.x) / r->dir.x;
	if(r->dir.y == 0 && abs(this->origin.y - r->start.y) < 0.001)	t.y = std::numeric_limits<double>::infinity();
	else if(r->dir.y == 0)											t.y = std::numeric_limits<double>::quiet_NaN();
	else															t.y = (this->origin.y - r->start.y) / r->dir.y;
	if(r->dir.z == 0 && abs(this->origin.z - r->start.z) < 0.001)	t.z = std::numeric_limits<double>::infinity();
	else if(r->dir.z == 0)											t.z = std::numeric_limits<double>::quiet_NaN();
	else															t.z = (this->origin.z - r->start.z) / r->dir.z;

	//if light is behind the camera, no intersection
	if(t.x < 0 || t.y < 0 || t.z < 0) return NULL;

	bool txyEquiv = (abs(t.x-t.y) < 0.001) || t.x == std::numeric_limits<double>::infinity() || t.y == std::numeric_limits<double>::infinity();
	bool tyzEquiv = (abs(t.y-t.z) < 0.001) || t.y == std::numeric_limits<double>::infinity() || t.z == std::numeric_limits<double>::infinity();
	bool txzEquiv = (abs(t.x-t.z) < 0.001) || t.x == std::numeric_limits<double>::infinity() || t.z == std::numeric_limits<double>::infinity();

	if(txyEquiv && tyzEquiv && txzEquiv) { //If the ts are roughly the same, we hit the light
		intersect = this->origin;
		return this;
	}
	return NULL;
}

/*
====================
Light::getColor
	Returns the color of the object
====================
*/
Color Light::getColor() {
	return this->material.color;
}