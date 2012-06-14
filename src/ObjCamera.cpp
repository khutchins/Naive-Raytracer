#include "ObjCamera.h"

/*
====================
Camera::Camera
	Takes in the input stream and creates a camera object from the parsed input
====================
*/
Camera::Camera(ifstream &f)
{
	this->grayscale = false;
	this->isLight = false;
	this->isVisible = false;
	this->hasTexture = false;
	this->aa = AA_TYPE_NONE;
	this->imageHeight = 240;
	this->imageWidth = 320;
	this->name = "";

	//Material doesn't matter for the camera

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
				norm(this->direction);
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
				if(sNum == "none") this->aa = AA_TYPE_NONE;
				else if(sNum == "fsaa4") this->aa = AA_TYPE_FSAA_4;
				else if(sNum == "fsaa16") this->aa = AA_TYPE_FSAA_16;
				else if(sNum == "naive-average") this->aa = AA_TYPE_NAIVE_AVERAGE;
				else if(sNum == "edaa4") this->aa = AA_TYPE_EDAA_4;
				else if(sNum == "edaa16") this->aa = AA_TYPE_EDAA_16;
			}
		}
		else break;
	}
	if(this->perspective && this->zmin == 0)
		printf("Warning: Perspective camera with a zmin of zero.\n");

	//Warning: Vectors are not orthogonal
	if(abs(dot3(up,direction)) > 0.00001) {
		printf("Warning: Camera up vector ");
		up.print();
		printf(" and direction vector ");
		direction.print();
		printf(" are not orthogonal.\n");
	}

	double aspect = (double)imageHeight/(double)imageWidth;

	height = this->width * aspect;
	width2 = width/2.f;
	height2 = height/2.f;

	gapWidth2 = 1.f/((double)imageWidth)/2.f;
	gapHeight2 = 1.f/((double)imageHeight)/2.f;
}

/*
====================
Camera::renderPixel
	Takes in the (x,y) coordinates of the pixel to be rendered as well as the 
	number of samples to average for that pixel.  It returns the color of the 
	raytraced pixel.
====================
*/
Color Camera::renderPixel(int x, int y, int numSamples, Raytracer *raytracer) {
	int sqrtNumSamples = (int)sqrt((float)numSamples);

	double deltawidth = 1/(imageWidth * sqrtNumSamples);
	double deltaheight = 1/(imageHeight * sqrtNumSamples);

	double leftGap = 1.f/(imageWidth * sqrtNumSamples*2); //Gap between edge of "square" and leftmost sample.
	double topGap = 1.f/(imageHeight * sqrtNumSamples*2);
	double gapBetweenHorizontalSamples = 2*leftGap;
	double gapBetweenVerticalSamples = 2*topGap;

	bool isPerspective = this->perspective; //False if orthogonal, true if perspective

	double xCoord = -1*width2 + width*x/imageWidth;
	double yCoord = -1*height2 + height*y/imageHeight;

	Vector vLeft = cross3(this->direction,this->up); 

	//Point on image plane
	Point pPointOnImagePlane = this->origin + this->zmin * this->direction + xCoord * vLeft + yCoord * this->up;
	
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

	Ray* ray = new Ray(); //Ray from the camera to the image plane
	ray->dir = vCamToImagePlane;
	
	if(isPerspective)
		ray->start = this->origin;
	else
		ray->start = pPointOnImagePlane;

	bool lightWasSeen = false;

	Color col;
	if(numSamples > 1) {
		Color* colors = new Color[numSamples];
		for(int x2 = 0; x2 < sqrtNumSamples; x2++) {
			double adjXCoord = xCoord - gapWidth2 + leftGap + (x2-1)*gapBetweenHorizontalSamples;
			for(int y2 = 0; y2 < sqrtNumSamples; y2++) {
				double adjYCoord = yCoord - gapHeight2 + topGap + (y2-1)*gapBetweenVerticalSamples;
				pPointOnImagePlane = this->origin + this->zmin * this->direction + adjXCoord * vLeft + adjYCoord * this->up;
				if(isPerspective) ray->dir = pPointOnImagePlane - this->origin;
				if(!isPerspective) ray->start = pPointOnImagePlane;
				colors[x2*sqrtNumSamples + y2] = raytracer->raytrace(ray,lightWasSeen);
			}
		}
		col = Color::averageValues(colors,numSamples);
		delete colors;
	}
	else col = raytracer->raytrace(ray,lightWasSeen);
	if(DIAGNOSTIC_STATUS == DIAGNOSTIC_IS_LIT) col.adjustColorForDiagnosticIsLit();

	delete ray;
	return col;
}

/*
====================
Camera::renderScene
	Takes in the scene file location as well as the camera number (used if the 
	camera doesn't have a name).  Renders the scene completely (including any 
	anti-aliasing/convolutions and saves the file.  This is the meat of this 
	application.  It also takes in a reference to the raytracer in order to 
	render the scene
====================
*/
void Camera::renderScene(string filename, int cameraNum, Raytracer *raytracer) {
	BMP image;
	image.SetSize(this->imageWidth,this->imageHeight);
    image.SetBitDepth(32);
	int numSamples;
	if(aa == AA_TYPE_FSAA_4) numSamples = 4;
	else if(aa == AA_TYPE_FSAA_16) numSamples = 16;
	else numSamples = 1;

	for(int y = 0; y < this->imageHeight; y++) {
		for(int x = 0; x < this->imageWidth; x++) {
			Color col = renderPixel(x,y,numSamples,raytracer);

			if(this->grayscale) image.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorGrayscale(col.grayscaleValue()).RGBAPixel());
			else				image.SetPixel(imageWidth-x-1,imageHeight-y-1,col.RGBAPixel());
		}
	}

	string sceneName = filename.substr(0,filename.length()-4);
	sceneName += "-";
	if(name == "") {
		char temp[33];
		sprintf(temp,"%d",cameraNum);
		sceneName += temp;
	}
	else sceneName += name;
	sceneName += ".bmp";

	if(DIAGNOSTIC_STATUS == DIAGNOSTIC_EDGE_DETECTION)		generateConvolutionBitmap(image,getEdgeDetectionConvolution()).WriteToFile(sceneName.c_str());
	else if(aa == AA_TYPE_NAIVE_AVERAGE)					generateNaiveAABMP(image).WriteToFile(sceneName.c_str());
	else if(aa == AA_TYPE_EDAA_4 || aa == AA_TYPE_EDAA_16)	generateEDAABMP(this,image,raytracer).WriteToFile(sceneName.c_str());
	else													image.WriteToFile(sceneName.c_str());

	cout << "Finished rendering file " << sceneName << ".\n";
}
