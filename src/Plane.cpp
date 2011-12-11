#include "Plane.h"

Plane::Plane(ifstream &f)
{
	this->isLight = false;
	this->isVisible = false;
	this->hastexture = false;

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

		//words with three arguments
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

		//words with two arguments
		else if(word == "size")
		{
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
		else if(word == "reflect" || word == "transparency")
		{
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "reflect") //read in the color coordinates
			{
				this->material.reflection = num1;
			}
			else if (word == "transparency")
			{
				this->material.transparency = num1;
			}
		}

		//words with one string argument
		else if(word == "texture")
		{
			string textureName;

			if(lineContents.size() < 1) break;

			textureName = lineContents.front();
			lineContents.pop();

			if(this->texture.ReadFromFile(textureName.c_str()))
				this->hastexture = true;
		}
		else
			break;
	}
}