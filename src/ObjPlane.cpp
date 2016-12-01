#include "ObjPlane.h"

/*
====================
Plane::Plane
	Takes in the input stream and creates a Plane object from the parsed input
====================
*/
Plane::Plane(ifstream &f) {
	string textureName;

	while(!f.eof()) {
		string line;
		getline(f,line);

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		//words with three arguments
		if(word == "origin" || word == "color" || word == "normal" || word == "up") {
			double num1 = 0;
			double num2 = 0;
			double num3 = 0;

			if(lineContents.size() < 3) break;

			num1 = atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = atof(lineContents.front().c_str());
			lineContents.pop();
			num3 = atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "origin")		this->origin = Point(num1,num2,num3);
			else if(word == "color")	material.color = Color(num1,num2,num3);
			else if(word == "up") {
				this->up = Vector(num1,num2,num3);
			}
			else if(word == "normal") {
				this->normal = Vector(num1,num2,num3);
			}
		}

		//words with two arguments
		else if(word == "size") {
			double num1 = 0;
			double num2 = 0;

			if(lineContents.size() < 2) break;

			num1 = atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = atof(lineContents.front().c_str());
			lineContents.pop();

			this->width = num1;
			this->height = num2;
		}

		//words with one double argument
		else if(word == "reflect" || word == "transparency") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "reflect")			this->material.reflection = num1;
			else if (word == "transparency")this->material.transparency = num1;
		}

		//words with one string argument
		else if(word == "texture") {
			//TODO: attempt load texture from local directory first
			if(lineContents.size() < 1) break;

			textureName = lineContents.front();
			lineContents.pop();
		}
		else break;
	}

	sharedInit(material,width,height,up,normal,textureName,origin);
}

/*
====================
Plane::Plane
	Takes information for creation from parameters instead of input stream 
	so that it can be created by a complex object
====================
*/
Plane::Plane(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin) {
	sharedInit(m,width,height,up,normal,textureName,origin);
}

/*
====================
Plane::sharedInit
	Shared initialization method for all Plane objects
====================
*/
void Plane::sharedInit(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin) {
	//Warns if Vectors are not orthogonal
	if(abs(up.dot(normal)) > 0.00001) {
		printf("Warning: Plane up vector ");
		up.print();
		printf(" and normal vector ");
		normal.print();
		printf(" are not orthogonal.\n");
	}

	this->objectType = ENTITY_PLANE;

	this->origin = origin;
	this->width = width;
	this->height = height;
	this->material = m;
	this->up = up;
	this->normal = normal;
	this->hasTexture = false;
	if(textureName.size() > 0 && this->texture.ReadFromFile(textureName.c_str())) {
		this->hasTexture = true;
	}
	this->isLight = false;
	this->isVisible = true;

	this->up.normalize();
	this->normal.normalize();

	Vector left = this->normal.cross(this->up);
	Point upMid = (this->up * this->height * 0.5) + this->origin;
	vertex1 = upMid + (left * this->width * 0.5);
	vertex2 = upMid - (left * this->width * 0.5);
	vertex3 = vertex1 - (this->up * this->height);
	vertex4 = vertex2 - (this->up * this->height);
	topLine =  vertex2 - vertex1;
	leftLine = vertex3 - vertex1;
}

/*
====================
Plane::intersect
	Computes intersection between the Plane and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Plane::intersect(Ray* r, Point &intersect) {
	double dot = normal.dot(r->dir);

	if(dot != 0) { //If the normal and ray aren't perpendicular (ray and plane parallel)
		double t = normal.dot(origin - r->start) / dot;
		if(t >= 0) { //If the ray is pointing toward the plane
			//Calculate point of intersection on plane
			Point tempInt = r->dir * t + r->start;

			//Calculate vectors that define the rectangle and vector to point
			// +----------> topLine
			// |\
			// | \ topLeftToPoint
			// v
			// leftLine
			Vector topLeftToPoint = tempInt - vertex1;

			double leftProjectionLength	= leftLine.dot(topLeftToPoint);
			double leftLineLength		= this->height * this->height;
			double topProjectionLength	= topLine.dot(topLeftToPoint);
			double topLineLength		= this->width * this->width;

			if(0 <= leftProjectionLength && leftProjectionLength <= leftLineLength && 
			   0 <= topProjectionLength  && topProjectionLength  <= topLineLength) {
				intersect = tempInt;
				return this;
			}
		}
	}
	return NULL;
}

/*
====================
calculateTextureFromMaterial
	based on the point of interception, calculates what color from the texture 
	should be returned
====================
*/
Color Plane::calculateTextureFromMaterial(Point intercept, bool diagnosticEnabled) {
	BMP* texture;
	texture = &this->texture;
	int height = texture->TellHeight();
	int width  = texture->TellWidth();

	Vector topLeftToPoint = intercept - vertex1;

	double leftProjectionLength	= leftLine.dot(topLeftToPoint);
	double leftLineLength		= this->height * this->height;
	double topProjectionLength	= topLine.dot(topLeftToPoint);
	double topLineLength		= this->width * this->width;

	double heightPercentage = leftProjectionLength / leftLineLength;
	double widthPercentage  = topProjectionLength  / topLineLength;

	int pixelX = abs((int)(widthPercentage * width));
	int pixelY = abs((int)(heightPercentage * height));
	pixelX %= width;
	pixelY %= height;
	Color matColor;

	if(diagnosticEnabled) {
		matColor = Color(heightPercentage,1,widthPercentage);
	}
	else {
		matColor.r = texture->GetPixel(pixelX,pixelY).Red/255.f;
		matColor.g = texture->GetPixel(pixelX,pixelY).Green/255.f;
		matColor.b = texture->GetPixel(pixelX,pixelY).Blue/255.f;
	}

	return matColor;
}

/*
====================
Plane::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	I assume an infinitely wide Plane, so this will always return the normal 
	as if the ray hit the Plane
====================
*/
Vector Plane::calculateNormalForPoint(Point p, Point raySource) {
	double distNormalSide = (p + this->normal).comparativeDistanceFrom(raySource);
	double distOtherSide =	(p - this->normal).comparativeDistanceFrom(raySource);
	if(distOtherSide < distNormalSide) return this->normal * -1;
	return this->normal;
}

/*
====================
Plane::getReflection
	Returns the reflection coefficient
====================
*/
double Plane::getReflection() {
	return this->material.reflection;
}

/*
====================
Plane::getRefraction
	Returns the refraction coefficient
====================
*/
double Plane::getRefraction() {
	return this->material.transparency;
}

/*
====================
Plane::getColor
	Returns the color of the object (ignoring texture color)
====================
*/
Color Plane::getColor() {
	return this->material.color;
}
