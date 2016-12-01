#include "ObjDisk.h"

/*
====================
Disk::Disk
	Takes in the input stream and creates a Disk object from the parsed input
====================
*/
Disk::Disk(ifstream &f) {
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
		if(word == "origin" || word == "color" || word == "normal" || word == "up") {
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

			if(word == "origin") { //read in the origin coordinates
				this->origin = Point(num1,num2,num3);
			}
			else if(word == "color") { //read in the color coordinates
				this->material.color = Color(num1,num2,num3);
			}
			else if(word == "normal") {
				this->normal = Vector(num1,num2,num3);
			}
			else if(word == "up") {
				this->up = Vector(num1,num2,num3);
			}
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius")		this->radius = num1;
			else if(word == "reflect")	this->material.reflection = num1;
			else						this->material.transparency = num1;
		}
		else break;
	}
	sharedInit(material,radius,up,normal,origin);
}

/*
====================
Disk::Disk
	Takes information for creation from parameters instead of input stream 
	so that it can be created by a complex object
====================
*/
Disk::Disk(Material m, double radius, Vector up, Vector normal, Point origin) {
	sharedInit(m,radius,up,normal,origin);
}

/*
====================
Disk::sharedInit
	Shared initialization method for all Disk objects
====================
*/
void Disk::sharedInit(Material m, double radius, Vector up, Vector normal, Point origin) {
	//Warning if Vectors are not orthogonal
	if(abs(up.dot(normal)) > 0.00001) {
		printf("Warning: Disk up vector ");
		up.print();
		printf(" and normal vector ");
		normal.print();
		printf(" are not orthogonal.\n");
	}

	//Constant parameters for all disks
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_DISK;
	this->hasTexture = false;

	//Variable parameters for specific disk
	this->radius = radius;
	this->material = m;
	this->up = up;
	this->up.normalize();
	this->normal = normal;
	this->normal.normalize();
	this->origin = origin;
}

/*
====================
Disk::intersect
	Computes intersection between the Disk and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Disk::intersect(Ray* r, Point &intersect) {
	double dot = normal.dot(r->dir);

	if(dot != 0) { //If the normal and ray aren't perpendicular (ray and disk parallel)
		double t = normal.dot(origin - r->start) / dot;
		if(t >= 0) { //If the ray is pointing toward the disk
			//Calculate point of intersection on disk
			Point tempInt = r->dir * t + r->start;

			//See if disk contains the point
			if(tempInt.comparativeDistanceFrom(this->origin) < radius*radius) {
				intersect = tempInt;
				return this;
			}
		}
	}
	return NULL;
}

/*
====================
Disk::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	I assume an infinitely wide disk here, so this will always return the normal 
	as if the ray hit the disk
====================
*/
Vector Disk::calculateNormalForPoint(Point p, Point raySource) {
	double distNormalSide = (p + this->normal).comparativeDistanceFrom(raySource);
	double distOtherSide = (p - this->normal).comparativeDistanceFrom(raySource);
	if(distOtherSide < distNormalSide) return this->normal * -1;
	return this->normal;
}

/*
====================
Disk::getReflection
	Returns the reflection coefficient
====================
*/
double Disk::getReflection() {
	return this->material.reflection;
}

/*
====================
Disk::getRefraction
	Returns the refraction coefficient
====================
*/
double Disk::getRefraction() {
	return this->material.transparency;
}

/*
====================
Disk::getColor
	Returns the color of the object
====================
*/
Color Disk::getColor() {
	return this->material.color;
}