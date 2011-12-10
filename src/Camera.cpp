#include "Camera.h"

Camera::Camera(ifstream &f)
{
	this->grayscale = false;
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
		if(word == "origin" || word == "direction" || word == "up")
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
			else if(word == "direction")
			{
				this->direction.x = num1;
				this->direction.y = num2;
				this->direction.z = num3;
			}
			else if(word == "up")
			{
				this->up.x = num1;
				this->up.y = num2;
				this->up.z = num3;
			}
		}

		//words with two arguments
		else if(word == "z")
		{
			double num1 = 0;
			double num2 = 0;

			if(lineContents.size() < 2) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			this->zmin = num1;
			this->zmax = num2;
		}

		//words with one argument
		else if(word == "width" || word == "perspective" || word == "grayscale")
		{
			string sNum1;
			double num1 = 0;

			if(lineContents.size() < 1) break;

			sNum1 = lineContents.front();
			lineContents.pop();
			bool bool1 = (sNum1[0] != '0') ? true : false;
			num1 = (double)atof(sNum1.c_str());

			if(word == "width") //read in the color coordinates
			{
				this->width = num1;
			}
			else if(word == "perspective")
			{
				this->perspective = bool1;
			}
			else if(word == "grayscale")
			{
				this->grayscale = bool1;
			}
		}
		else break;
	}
	if(this->perspective && this->zmin == 0)
		printf("Warning: Perspective camera with a zmin of zero.\n");
}