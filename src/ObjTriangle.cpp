#include "ObjTriangle.h"

/*
====================
Triangle::Triangle
	Takes in the input stream and creates a Triangle object from the parsed input
====================
*/
Triangle::Triangle(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_TRIANGLE;
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
		if(word == "color" || word == "vertex1" || word == "vertex2" || word == "vertex3") {
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

			if(word == "vertex1") { //read in the origin coordinates
				this->vertex1 = Point(num1,num2,num3);
			}
			else if(word == "vertex2") { //read in the origin coordinates
				this->vertex2 = Point(num1,num2,num3);
			}
			else if(word == "vertex3") { //read in the origin coordinates
				this->vertex3 = Point(num1,num2,num3);
			}
			else if(word == "color") { //read in the color coordinates
				this->material.color = Color(num1,num2,num3);
			}
		}

		//words with one argument
		else if(word == "reflect" || word == "transparency") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "transparency")	this->material.transparency = num1;
			else if(word == "reflect")	this->material.reflection = num1;
		}
		else break;
	}

	this->normal = (vertex3 - vertex1).cross(vertex2 - vertex1);
	this->origin = vertex1;
	this->normal.normalize();
}

/*
====================
Triangle::Triangle
	Takes information for creation from parameters instead of input stream 
	so that it can be created by a complex object
====================
*/
Triangle::Triangle(Material m, Point vertex1, Point vertex2, Point vertex3) {
	//Constant parameters for all triangles
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_TRIANGLE;
	this->hasTexture = false;

	//Variable parameters for specific triangle
	this->material = m;
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	this->vertex3 = vertex3;
	this->normal = (vertex3 - vertex1).cross(vertex2 - vertex1);
	this->origin = vertex1;
	this->normal.normalize();
}

/*
====================
Triangle::intersect
	Computes intersection between the Triangle and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Triangle::intersect(Ray* r, Point &intersect) {
	//TODO
	double dot = normal.dot(r->dir);

	if(dot != 0) { //If the normal and ray aren't perpendicular (which would mean ray and triangle are parallel)
		double t = normal.dot(origin - r->start) / dot;
		if(t >= 0) { //If the ray is pointing toward the Triangle
			//Calculate point of intersection on Triangle
			Point tempInt = r->dir * t + r->start;

			//See if Triangle contains the point (using the same side technique)
			if(pointsOnSameSideOfLine(tempInt,vertex1,vertex2,vertex3) 
				&& pointsOnSameSideOfLine(tempInt,vertex2,vertex1,vertex3) 
				&& pointsOnSameSideOfLine(tempInt,vertex3,vertex1,vertex2)) {
				intersect = tempInt;
				return this;
			}
		}
	}
	return NULL;
}

/*
====================
Triangle::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	I assume an infinitely wide Triangle, so this will always return the normal 
	as if the ray hit the Triangle
====================
*/
Vector Triangle::calculateNormalForPoint(Point p, Point raySource) {
	double distNormalSide = (p + this->normal).comparitiveDistanceFrom(raySource);
	double distOtherSide =	(p - this->normal).comparitiveDistanceFrom(raySource);
	if(distOtherSide < distNormalSide) return this->normal * -1;
	return this->normal;
}

/*
====================
Triangle::getReflection
	Returns the reflection coefficient
====================
*/
double Triangle::getReflection() {
	return this->material.reflection;
}

/*
====================
Triangle::getRefraction
	Returns the refraction coefficient
====================
*/
double Triangle::getRefraction() {
	return this->material.transparency;
}

/*
====================
Triangle::getColor
	Returns the color of the object
====================
*/
Color Triangle::getColor() {
	return this->material.color;
}