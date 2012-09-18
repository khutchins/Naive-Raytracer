#include "ObjCuboid.h"

/*
====================
Cuboid::Cuboid
	Takes in the input stream and creates a cuboid object from the parsed input
====================
*/
Cuboid::Cuboid(ifstream &f) {
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_CUBOID;
	this->hasTexture = false;
	Material material;
	string textureName;

	children.resize(6);

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
		if(word == "origin" || word == "color" || word == "up" || word == "front") {
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
			else if(word == "color")	material.color = Color(num1,num2,num3);
			else if(word == "up") {
				this->up = Vector(num1,num2,num3);
				this->up.normalize();
			}
			else if(word == "front") {
				this->front = Vector(num1,num2,num3);
				this->front.normalize();
			}
		}

		//words with one number argument
		else if(word == "reflect" || word == "transparency" || word == "height"
				|| word == "width" || word == "length") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "reflect")		material.reflection = num1;
			else if(word == "transparency")	material.transparency = num1;
			else if(word == "height")		height = num1;
			else if(word == "width")		width = num1;
			else if(word == "length")		length = num1;
		}

		//words with one string argument
		else if(word == "texture") {
			string textureName;

			if(lineContents.size() < 1) break;

			textureName = lineContents.front();
			lineContents.pop();
		}

		else break;
	}

	//Warning: Vectors are not orthogonal
	if(abs(dot3(up,front)) > 0.00001) {
		printf("Warning: Cuboid up vector ");
		up.print();
		printf(" and front vector ");
		front.print();
		printf(" are not orthogonal.\n");
	}
	
	Vector left = cross3(up,front);
	left.normalize();

	children[0] = new Plane(material, length, width, front, up, textureName, origin+up*height*0.5f); //Top plane
	children[1] = new Plane(material, length, width, front, -1*up, textureName, origin-up*height*0.5f); //Bottom plane
	children[2] = new Plane(material, length, height, up, front, textureName, origin+front*width*0.5f); //Front plane
	children[3] = new Plane(material, length, height, up, -1*front, textureName, origin-front*width*0.5f); //Back plane
	children[4] = new Plane(material, width, height, up, left, textureName, origin+left*length*0.5f); //left plane
	children[5] = new Plane(material, width, height, up, -1*left, textureName, origin-left*length*0.5f); //right plane
}

Cuboid::~Cuboid() {
	for(size_t i = 0; i < children.size(); i++) {
		delete children[i];
	}
}