#include "ObjCylinder.h"

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
			else if(word == "color") material.color = Color(num1,num2,num3);
			else if(word == "up") {
				this->up = Vector(num1,num2,num3);
				norm(this->up);
			}
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency" || word == "height") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius")			radius = num1;
			else if(word == "reflect")		material.reflection = num1;
			else if(word == "transparency")	material.transparency = num1;
			else if(word == "height")		height = num1;
		}
		else break;
	}
	children[0] = new Tube(material,radius,height,up,origin);
	children[1] = new Disk(material,radius,/*up = */vectorPerpendicularTo(up),/*normal = */up,/*origin = */origin+up*(height*0.5));
	children[2] = new Disk(material,radius,/*up = */vectorPerpendicularTo(up),/*normal = */up,/*origin = */origin-up*(height*0.5));
}

Cylinder::~Cylinder() {
	for(int i = 0; i < children.size(); i++) {
		delete children[i];
	}
}