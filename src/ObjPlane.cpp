#include "ObjPlane.h"

/*
====================
Plane::Plane
	Takes in the input stream and creates a Plane object from the parsed input
====================
*/
Plane::Plane(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->hasTexture = false;
	this->objectType = ENTITY_PLANE;

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
			else if(word == "normal") {
				this->normal = Vector(num1,num2,num3);
				this->normal.normalize();
			}
		}

		//words with two arguments
		else if(word == "size") {
			double num1 = 0;
			double num2 = 0;

			if(lineContents.size() < 2) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			this->width = num1;
			this->height = num2;
		}

		//words with one double argument
		else if(word == "reflect" || word == "transparency") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "reflect")			this->material.reflection = num1;
			else if (word == "transparency")this->material.transparency = num1;
		}

		//words with one string argument
		else if(word == "texture") {
			//TODO: attempt load texture from local directory first
			string textureName;

			if(lineContents.size() < 1) break;

			textureName = lineContents.front();
			lineContents.pop();

			if(this->texture.ReadFromFile(textureName.c_str()))
				this->hasTexture = true;
		}
		else break;
	}

	//Warning: Vectors are not orthogonal
	if(abs(up.dot(normal)) > 0.00001) {
		printf("Warning: Plane up vector ");
		up.print();
		printf(" and normal vector ");
		normal.print();
		printf(" are not orthogonal.\n");
	}
}

/*
====================
Plane::Plane
	Takes information for creation from parameters instead of input stream 
	so that it can be created by a complex object
====================
*/
Plane::Plane(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin) {
	this->origin = origin;
	this->width = width;
	this->height = height;
	this->material = m;
	this->up = up;
	this->normal = normal;
	this->hasTexture = false;
	if(textureName.size() > 0) if(this->texture.ReadFromFile(textureName.c_str())) this->hasTexture = true;
	this->isLight = false;
	this->isVisible = true;
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

			//See if the point of intersection is actually on the finite plane
			//First, find coordinates of vertices that make up the quad
			Vector left = normal.cross(up);
			Point upMid = (this->up * this->height * 0.5) + this->origin;
			Point upLeft = upMid + (left * this->width * 0.5);
			Point upRight = upMid - (left * this->width * 0.5);
			Point botLeft = upLeft - (this->up * this->height);
			Point botRight = upRight - (this->up * this->height);

			//Calculate vectors that define the rectangle and vector to point
			Vector leftV = upRight - upLeft;
			Vector up = botLeft - upLeft;
			Vector cornerToPoint = tempInt - upLeft;

			double dotUpCTP = up.dot(cornerToPoint);
			double dotUpUp = up.dot(up);
			double dotLeftCTP = leftV.dot(cornerToPoint);
			double dotLeftLeft = leftV.dot(leftV);

			if(0 <= dotUpCTP && dotUpCTP <= dotUpUp && 0 <= dotLeftCTP && dotLeftCTP <= dotLeftLeft) {
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
Color Plane::calculateTextureFromMaterial(Point intercept) {
	BMP* temp;
	temp = &this->texture;
	int height = temp->TellHeight();
	int width = temp->TellWidth();
	double planeHeight = this->height;
	double pixelSize = height / planeHeight; //Width and height of pixel on plane

	//Figure out the boundaries of the plane
	Vector left = normal.cross(up);

	Point upMid = (this->up * this->height * 0.5) + this->origin;
	Point upLeft = upMid + (left * this->width * 0.5);
	Point upRight = upMid - (left * this->width * 0.5);
	Point botLeft = upLeft - (this->up * this->height);
	Point botRight = upRight - (this->up * this->height);

	double heightPercentage = 0;
	double widthPercentage = 0;

	Ray top;
	top.dir = upLeft - upRight;
	top.start = upRight;

	Ray planeUp;
	planeUp.dir = this->up;
	planeUp.start = intercept;

	// ((p2 - p1)xd1) . (d1xd2) / ||d1xd2||^2
	//percentage along top of plane
	double tTopPoint = (planeUp.start - top.start).cross(planeUp.dir).dot(top.dir.cross(planeUp.dir)) / top.dir.cross(planeUp.dir).magnitude();
	//double tTopPoint = dot3((planeUp.start - top.start).cross(planeUp.dir), top.dir.cross(planeUp.dir))/magnitude2(top.dir.cross(planeUp.dir));

	Ray side;
	side.dir = upLeft - botLeft;
	side.start = botLeft;
	side.dir.normalize();

	Ray planeLeft;
	planeLeft.dir = left;
	planeLeft.start = intercept;

	//percentage along side of plane
	double tLeftPoint = 1 - (1.f/this->height) * (planeLeft.start - side.start).cross(planeLeft.dir).dot(side.dir.cross(planeLeft.dir)) / magnitude2(side.dir.cross(planeLeft.dir));
	//double tLeftPoint = 1 - (1.f/this->height) * dot3((planeLeft.start - side.start).cross(planeLeft.dir),side.dir.cross(planeLeft.dir))/magnitude2(side.dir.cross(planeLeft.dir));

	int pixelX = abs((int)(tTopPoint * width));
	int pixelY = abs((int)(tLeftPoint * height));
	pixelX %= width;
	pixelY %= height;
	Color matColor;
	if(DIAGNOSTIC_STATUS == DIAGNOSTIC_TEXTURE_MAPPING) {
		matColor = Color(tLeftPoint,1,tTopPoint);
	}
	else {
		matColor.r = temp->GetPixel(pixelX,pixelY).Red/255.f;
		matColor.g = temp->GetPixel(pixelX,pixelY).Green/255.f;
		matColor.b = temp->GetPixel(pixelX,pixelY).Blue/255.f;
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
	double distNormalSide = dist3Compare(p + this->normal, raySource);
	double distOtherSide = dist3Compare(p - this->normal, raySource);
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
