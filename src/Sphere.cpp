#include "Sphere.h"

Vector Sphere::calculateNormalForPoint(Point p) {
	return p - this->origin;
}

Sphere::Sphere(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = SPHERE;
	this->hasTexture = false;
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
		if(word == "origin" || word == "color")
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
				this->material.color.r = num1;
				this->material.color.g = num2;
				this->material.color.b = num3;
			}
		}

		//words with one argument
		else if(word == "radius" || word == "reflect" || word == "transparency")
		{
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "radius") //read in the origin coordinates
			{
				this->radius = num1;
			}
			else if(word == "reflect") //read in the color coordinates
			{
				this->material.reflection = num1;
			}
			else
			{
				this->material.transparency = num1;
			}
		}
		else
			break;
	}
}

bool Sphere::intersect(Ray* r, Point &intersect) {
	Vector dist = this->origin - r->start;

	norm(dist);
	norm(r->dir);

	double a = dot3(r->dir,r->dir);
	double b = 2 * dot3(r->start - this->origin,r->dir);
	double c = dot3(r->start - this->origin,r->start - this->origin) - this->radius * this->radius;

	double disc = discrim(a,b,c);

	//No intersection, do nothing
	if(disc < 0) return false;
	else if(disc >= 0) //Find closest intersection
	{
		double discSqrt = sqrt(disc);
		double quad;
		if (b < 0)	quad = (-b - discSqrt)/2.f;
		else quad = (-b + discSqrt)/2.f;

		double t0 = quad/a;
		double t1 = c/quad;
		if(t0 > t1) swap(t0,t1);

		double t;
		if(t0 < 0 && t1 < 0) return false;
		if(t0 < 0) t = t1;
		else t = t0;

		intersect = r->start + t * r->dir;
		return true;
	}
	return false;
}

double Sphere::getReflection() {
	return this->material.reflection;
}

double Sphere::getRefraction() {
	return this->material.transparency;
}

Color Sphere::getColor() {
	return this->material.color;
}