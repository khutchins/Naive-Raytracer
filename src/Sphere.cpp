#include "Sphere.h"

Vector Sphere::calculateNormalForPoint(Point p) {
	return p - this->center;
}

Sphere::Sphere(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
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
				this->center.x = num1;
				this->center.y = num2;
				this->center.z = num3;
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