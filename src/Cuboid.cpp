#include "Cuboid.h"

Cuboid::Cuboid(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_CUBOID;
	this->hasTexture = false;
	Material material;
	string textureName;
	
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

			if(word == "origin") {
				this->origin.x = num1;
				this->origin.y = num2;
				this->origin.z = num3;
			}
			else if(word == "color") {
				material.color.r = num1;
				material.color.g = num2;
				material.color.b = num3;
			}
			else if(word == "up") {
				this->up.x = num1;
				this->up.y = num2;
				this->up.z = num3;
				norm(this->up);
			}
			else if(word == "front") {
				this->front.x = num1;
				this->front.y = num2;
				this->front.z = num3;
				norm(this->front);
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
	norm(left);

	sides[0] = new Plane(material, length, width, front, up, textureName, origin+up*height*0.5f); //Top plane
	sides[1] = new Plane(material, length, width, front, -1*up, textureName, origin-up*height*0.5f); //Bottom plane
	sides[2] = new Plane(material, length, height, up, front, textureName, origin+front*width*0.5f); //Front plane
	sides[3] = new Plane(material, length, height, up, -1*front, textureName, origin-front*width*0.5f); //Back plane
	sides[4] = new Plane(material, width, height, up, left, textureName, origin+left*length*0.5f); //left plane
	sides[5] = new Plane(material, width, height, up, -1*left, textureName, origin-left*length*0.5f); //right plane
}

SceneObject* Cuboid::intersect(Ray* r, Point &intersect) {
	SceneObject* objectHit = NULL;
	Point objectIntersect;

	for(int i = 0; i < 6; i++) {
		if(sides[i]->intersect(r,objectIntersect)) {
			if(!objectHit || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
				intersect = objectIntersect;
				objectHit = sides[i];
			}
		}
	}

	return objectHit;
}

Vector Cuboid::calculateNormalForPoint(Point p, Point raySource) {
	bool objectHit = false;
	Point objectIntersect, intersect;
	Plane* closestObject;
	Ray *r = new Ray();
	r->start = raySource;
	r->dir = p-raySource;

	for(int i = 0; i < 6; i++) {
		if(sides[i]->intersect(r,objectIntersect)) {
			if(!objectHit || dist3Compare(r->start,objectIntersect) < dist3Compare(r->start,intersect)) {
				intersect = objectIntersect;
				objectHit = true;
				closestObject = sides[i];
			}
		}
	}
	delete r;
	return closestObject->normal;
}

double Cuboid::getReflection() {
	return sides[0]->material.reflection;
}

double Cuboid::getRefraction() {
	return sides[0]->material.transparency;
}

Color Cuboid::getColor() {
	return sides[0]->material.color;
}
