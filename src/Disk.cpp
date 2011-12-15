#include "Disk.h"

Disk::Disk(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->objectType = DISK;
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
		if(word == "origin" || word == "color" || word == "normal" || word == "up")
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
			else if(word == "normal")
			{
				this->normal.x = num1;
				this->normal.y = num2;
				this->normal.z = num3;
				norm(this->normal);
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

bool Disk::intersect(Ray* r, Point &intersect) {
	double dot = dot3(this->normal,r->dir);

	if(dot != 0) //If the normal and ray aren't perpendicular (ray and disk parallel)
	{
		double t = dot3(this->normal,this->origin - r->start) / dot;
		if(t >= 0) //If the ray is pointing toward the disk
		{
			//Calculate point of intersection on disk
			Point tempInt = r->dir * t + r->start;

			//See if disk contains the point
			if(dist3Compare(tempInt,this->origin) < radius*radius) {
				intersect = tempInt;
				return true;
			}
		}
	}
	return false;
}

Vector Disk::calculateNormalForPoint(Point p, Point raySource) {
	double distNormalSide = dist3Compare(p + this->normal, raySource);
	double distOtherSide = dist3Compare(p + this->normal*-1, raySource);
	if(distOtherSide < distNormalSide) return this->normal * -1;
	return this->normal;
}

double Disk::getReflection() {
	return this->material.reflection;
}

double Disk::getRefraction() {
	return this->material.transparency;
}

Color Disk::getColor() {
	return this->material.color;
}