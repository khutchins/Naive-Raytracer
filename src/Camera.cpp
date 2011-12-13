#include "Camera.h"

Camera::Camera(ifstream &f)
{
	this->grayscale = false;
	this->isLight = false;
	this->isVisible = false;
	this->hasTexture = false;
	this->aa = NO_AA;
	this->imageHeight = 240;
	this->imageWidth = 320;
	this->name = "";
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
		else if(word == "z" || word == "imagesize")
		{
			double num1 = 0;
			double num2 = 0;

			if(lineContents.size() < 2) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "z") {
				this->zmin = num1;
				this->zmax = num2;
			}
			else if (word == "imagesize") {
				this->imageWidth = (int)num1;
				this->imageHeight = (int)num2;
			}
		}

		//words with one argument
		else if(word == "width" || word == "perspective" || word == "grayscale" 
				|| word == "aa"	|| word == "name")
		{
			string sNum;
			double num = 0;

			if(lineContents.size() < 1) break;

			sNum = lineContents.front();
			lineContents.pop();
			bool bool1 = (sNum[0] != '0') ? true : false;
			num = (double)atof(sNum.c_str());

			if(word == "width")				this->width = num;
			else if(word == "perspective")	this->perspective = bool1;
			else if(word == "grayscale")	this->grayscale = bool1;
			else if(word == "name")			this->name = sNum;
			else if(word == "aa")
			{
				if(sNum == "none") this->aa = NO_AA;
				else if(sNum == "fsaa4") this->aa = FSAA_4;
				else if(sNum == "fsaa16") this->aa = FSAA_16;
				else if(sNum == "naive-average") this->aa = NAIVE_AVERAGE;
			}
		}
		else break;
	}
	if(this->perspective && this->zmin == 0)
		printf("Warning: Perspective camera with a zmin of zero.\n");
}

void Camera::renderScene(string filename, int cameraNum) {
	BMP image;
	image.SetSize(this->imageWidth,this->imageHeight);
    image.SetBitDepth(32);
	int numSamples;
	if(aa == FSAA_4) numSamples = 4;
	else if(aa == FSAA_16) numSamples = 16;
	else numSamples = 1;
	int sqrtNumSamples = (int)sqrt((float)numSamples);
	double aspect = (double)imageHeight/(double)imageWidth;

	double width = this->width;
	double height = this->width * aspect;
	double width2 = width/2.f;
	double height2 = height/2.f;
	double deltawidth = 1/(imageWidth * sqrtNumSamples);
	double deltaheight = 1/(imageHeight * sqrtNumSamples);

	for(int y = 0; y < this->imageHeight; y++)
	{
		for(int x = 0; x < this->imageWidth; x++)
		{
			bool isPerspective = this->perspective; //False if orthogonal, true if perspective

			double uc = -1*width/2 + width/2 * 2.f*x/imageWidth;
			double vr = -1*height/2 + height/2 * 2.f*y/imageHeight;

			norm(this->direction);
			norm(this->up);

			Vector vLeft = cross3(this->direction,this->up); 

			//Point on image plane
			Point pPointOnImagePlane = this->origin + this->zmin * this->direction + uc * vLeft + vr * this->up;
			
			Vector vCamToImagePlane; //Vector from the camera to the image plane

			if(isPerspective) //If camera is perspective
			{
				vCamToImagePlane = pPointOnImagePlane - this->origin;
			}
			else //If camera is orthogonal
			{
				vCamToImagePlane = this->direction;
			}
			//vCamToImagePlane = c->direction;

			Ray* r = new Ray(); //Ray from the camera to the image plane
			r->dir = vCamToImagePlane;
			
			if(isPerspective)
				r->start = this->origin;
			else
				r->start = pPointOnImagePlane;

			bool lightT = false;

			Color col;
			if(aa == FSAA_4 || aa == FSAA_16) {
				Color* colors = new Color[numSamples];
				for(int x2 = 0; x2 < sqrtNumSamples; x2++) {
					for(int y2 = 0; y2 < sqrtNumSamples; y2++) {
						double uc = -width/2 + width/2 * 2*(x+x2)/imageWidth;
						double vr = -height/2 + height/2 * 2*(y+y2)/imageHeight;
						pPointOnImagePlane = this->origin + this->zmin * this->direction + uc * vLeft + vr * this->up;
						if(isPerspective) r->dir = pPointOnImagePlane - this->origin;
						colors[x2*sqrtNumSamples + y2] = raytrace(r,lightT);
					}
				}
				col = Color::averageValues(colors,numSamples);
				delete colors;
			}
			else col = raytrace(r,lightT);
			if(DIAGNOSTIC_STATUS == IS_LIT) col.adjustColorForDiagnosticIsLit();

			if(this->grayscale) {
				double grayscaleVal = col.r * 0.3 + col.g * 0.59 + col.b * 0.11;
				image(imageWidth-x-1,imageHeight-y-1)->Red   = (unsigned char)grayscaleVal;
				image(imageWidth-x-1,imageHeight-y-1)->Green = (unsigned char)grayscaleVal;
				image(imageWidth-x-1,imageHeight-y-1)->Blue  = (unsigned char)grayscaleVal;
			}
			else {
				image(imageWidth-x-1,imageHeight-y-1)->Red   = (unsigned char)col.r;
				image(imageWidth-x-1,imageHeight-y-1)->Green = (unsigned char)col.g;
				image(imageWidth-x-1,imageHeight-y-1)->Blue  = (unsigned char)col.b;
			}
			image(imageWidth-x-1,imageHeight-y-1)->Alpha = 0;

			delete r;
		}
	}

	string sceneName = filename.substr(0,filename.length()-4);
	sceneName += "-";
	if(name == "") {
		char temp[33];
		_itoa_s(cameraNum,temp,10);
		sceneName += temp;
	}
	else sceneName += name;
	sceneName += ".bmp";

	if(aa == NAIVE_AVERAGE) generateAABMP(image,this).WriteToFile(sceneName.c_str());
	else image.WriteToFile(sceneName.c_str());
}