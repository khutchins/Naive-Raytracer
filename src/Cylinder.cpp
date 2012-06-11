#include "Cylinder.h"

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
	tube = new Tube(material,radius,height,up,origin);
	top = new Disk(material,radius,/*up = */vectorPerpendicularTo(up),/*normal = */up,/*origin = */origin+up*(height*0.5));
	bottom = new Disk(material,radius,/*up = */vectorPerpendicularTo(up),/*normal = */up,/*origin = */origin-up*(height*0.5));
}

SceneObject* Cylinder::intersect(Ray* r, Point &intersect) {
	SceneObject *closestObject = NULL;
	bool objectHit = false;
	Point objectIntersect;
	if(tube->intersect(r,objectIntersect)) {
		intersect = objectIntersect;
		objectHit = true;
	}
	if(top->intersect(r,objectIntersect)) {
		if(!objectHit || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
			intersect = objectIntersect;
			objectHit = true;
		}
	}
	if(bottom->intersect(r,objectIntersect)) {
		if(!objectHit || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
			intersect = objectIntersect;
			objectHit = true;
		}
	}
	return closestObject;
}

Vector Cylinder::calculateNormalForPoint(Point p, Point raySource) {
	return p - this->origin;
}

double Cylinder::getReflection() {
	return this->material.reflection;
}

double Cylinder::getRefraction() {
	return this->material.transparency;
}

Color Cylinder::getColor() {
	return this->material.color;
}
