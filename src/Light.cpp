#include "Light.h"

Light::Light(ifstream &f)
{
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
			this->color.r = num1;
			this->color.g = num2;
			this->color.b = num3;
		}
		else
			break;

	}
}