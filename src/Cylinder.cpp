#include "Cylinder.h"

/*
====================
Cylinder::Cylinder
	Takes in the input stream and creates a Cylinder object from the parsed input
====================
*/
Cylinder::Cylinder(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_CYLINDER;
	this->hasTexture = false;
	Material material;
	double height;
	double radius;
	Vector up;

	children.resize(3);
	while(!f.eof())
	{
		string line;
		getline(f,line);

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		//words with three string arguments
		if(word == "origin" || word == "color" || word == "up")
		{
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

			if(word == "origin") //read in the origin coordinates
			{
				this->origin.x = num1;
				this->origin.y = num2;
				this->origin.z = num3;
			}
			else if(word == "color") //read in the color coordinates
			{
				material.color.r = num1;
				material.color.g = num2;
				material.color.b = num3;
			}
			else if(word == "up")
			{
				this->up.x = num1;
				this->up.y = num2;
				this->up.z = num3;
				norm(this->up);
			}
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency" || word == "height")
		{
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius")			radius = num1;
			else if(word == "reflect")		material.reflection = num1;
			else if(word == "transparency")	material.transparency = num1;
			else if(word == "height")		height = num1;
		}
		else
			break;
	}
	children[0] = new Tube(material,radius,height,up,origin);
	children[1] = new Disk(material,radius,/*up = */vectorPerpendicularTo(up),/*normal = */up,/*origin = */origin+up*(height*0.5));
	children[2] = new Disk(material,radius,/*up = */vectorPerpendicularTo(up),/*normal = */up,/*origin = */origin-up*(height*0.5));
}

/*
====================
Cylinder::intersect
	Computes intersection between the Cylinder and the ray, and returns the 
	primitive it intersects or NULL if it doesn't intersect along with the 
	point of intersection
====================
*/
SceneObject* Cylinder::intersect(Ray* r, Point &intersect) {
	SceneObject *closestObject = NULL;
	Point objectIntersect;

	for(size_t i = 0; i < children.size(); i++) {
		if(children[i]->intersect(r,objectIntersect)) {
			if(!closestObject || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
				intersect = objectIntersect;
				closestObject = children[i];
			}
		}
	}

	return closestObject;
}

/*
====================
Cylinder::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	If the ray doesn't hit the object, returns VectorZero.
====================
*/
Vector Cylinder::calculateNormalForPoint(Point p, Point raySource) {
	//TODO: This is wrong - compute closest primitive and get it's normal
	//return p - this->origin;
	return Vector::VectorZero();
}

/*
====================
Cylinder::getReflection
	Returns the reflection coefficient
====================
*/
double Cylinder::getReflection() {
	return this->material.reflection;
}

/*
====================
Cylinder::getRefraction
	Returns the refraction coefficient
====================
*/
double Cylinder::getRefraction() {
	return this->material.transparency;
}

/*
====================
Cylinder::getColor
	Returns the color of the object
====================
*/
Color Cylinder::getColor() {
	return this->material.color;
}
