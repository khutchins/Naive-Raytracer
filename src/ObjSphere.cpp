#include "ObjSphere.h"

/*
====================
Sphere::Sphere
	Takes in the input stream and creates a Sphere object from the parsed input
====================
*/
Sphere::Sphere(ifstream &f) {
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

		//words with three string arguments
		if(word == "origin" || word == "color") {
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
			else if(word == "color")	this->material.color = Color(num1,num2,num3);
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency") {
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius")			this->radius = num1;
			else if(word == "reflect")		this->material.reflection = num1;
			else if(word == "transparency")	this->material.transparency = num1;
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

	sharedInit(origin,material,radius,textureName);
}

/*
====================
Sphere::Sphere
	Takes in arguments and creates a Sphere object
====================
*/
Sphere::Sphere(Point origin, Material material, double radius, string textureName) {
	sharedInit(origin,material,radius,textureName);
}

/*
====================
Sphere::sharedInit
	Shared initialization method for all Sphere objects
====================
*/
void Sphere::sharedInit(Point origin, Material material, double radius, string textureName) {
	this->isLight = false;
	this->isVisible = true;
	this->objectType = ENTITY_SPHERE;
	this->hasTexture = false;
	if(textureName.size() > 0 && this->texture.ReadFromFile(textureName.c_str())) {
		this->hasTexture = true;
	}

	this->origin = origin;
	this->material = material;
	this->radius = radius;
}

/*
====================
Sphere::intersect
	Computes intersection between the Sphere and the ray, and returns itself if 
	it is hit or NULL if it is not along with the point of intersection
====================
*/
SceneObject* Sphere::intersect(Ray* r, Point &intersect) {
	Vector dist = this->origin - r->start;

	dist.normalize();
	r->dir.normalize();

	const double a = 1; //if not normalized, a = r->dot.dot(r->dot); (or |r->dir|^2)
	const double b = 2 * (r->start - this->origin).dot(r->dir);
	const double c = (r->start - this->origin).dot(r->start - this->origin) - this->radius * this->radius;

	const double discriminant = discrim(a,b,c);

	//No intersection, do nothing
	if(discriminant < 0) return NULL;
	else if(discriminant >= 0) { //Find closest intersection
		const double discSqrt = sqrt(discriminant);
		double quad;
		if (b < 0)	quad = (-b - discSqrt)/2.f;
		else		quad = (-b + discSqrt)/2.f;

		double t0 = quad/a;
		double t1 = c/quad;
		if(t0 > t1) swap(t0,t1);

		double t;
		if(t0 < 0 && t1 < 0) return NULL;
		if(t0 < 0)	t = t1;
		else		t = t0;

		intersect = r->start + t * r->dir;
		return this;
	}
	return NULL;
}

/*
====================
Sphere::calculateNormalForPoint
	Calculates the normal of the point p on the object as hit from ray r.  
	I assume an infinitely large Sphere, so this will always return the normal
	as if the ray hit the Sphere at the given point
====================
*/
Vector Sphere::calculateNormalForPoint(Point p, Point raySource) {
	return p - this->origin;
}

/*
====================
Sphere::getReflection
	Returns the reflection coefficient
====================
*/
double Sphere::getReflection() {
	return this->material.reflection;
}

/*
====================
Sphere::getRefraction
	Returns the refraction coefficient
====================
*/
double Sphere::getRefraction() {
	return this->material.transparency;
}

/*
====================
Sphere::getColor
	Returns the color of the object (ignoring texture color)
====================
*/
Color Sphere::getColor() {
	return this->material.color;
}

Color Sphere::calculateTextureFromMaterial(Point intercept, bool diagnosticEnabled) {
	BMP* texture;
	texture = &this->texture;
	int height = texture->TellHeight();
	int width  = texture->TellWidth();

	Vector vectorToIntercept = intercept - origin;
	vectorToIntercept.normalize();
	double u = 0.5 + atan2(vectorToIntercept.z,vectorToIntercept.x) / 2 / 3.1415;
	double v = 0.5 - asin(vectorToIntercept.y) / 3.1415;

	int pixelX = abs((int)(u * width));
	int pixelY = abs((int)(v * height));
	pixelX %= width;
	pixelY %= height;
	Color matColor;

	if(diagnosticEnabled) {
		matColor = Color(v,0,sin(u * 3.1415));
	}
	else {
		matColor.r = texture->GetPixel(pixelX,pixelY).Red/255.f;
		matColor.g = texture->GetPixel(pixelX,pixelY).Green/255.f;
		matColor.b = texture->GetPixel(pixelX,pixelY).Blue/255.f;
	}

	return matColor;
}
