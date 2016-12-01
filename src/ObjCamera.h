#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cstdlib>
#include "StringProcessing.h"
#include "ObjSceneObject.h"
#include "Point.h"
#include "Raytracer.h"
#include "Color.h"
#include "EasyBMP/EasyBMP.h"
#include "AntiAliasing.h"
#include "Diagnostic.h"
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class Raytracer;

enum AntiAliasingType {
	AA_TYPE_NONE,

	AA_TYPE_NAIVE_AVERAGE, //weighted average of pixels post processing

	AA_TYPE_FSAA_4, //full scene anti-aliasing with 4 samples (super-sampling)
	AA_TYPE_FSAA_16, //full scene anti-aliasing with 16 samples  (super-sampling)
	AA_TYPE_SSAA_4  = AA_TYPE_FSAA_4, //alias for FSAA4
	AA_TYPE_SSAA_16 = AA_TYPE_FSAA_16, //alias for FSAA16

	AA_TYPE_EDAA_4, //super-sampling with 4 samples on pixels that are detected at edges using the ED convolution
	AA_TYPE_EDAA_16, //super-sampling with 16 samples on pixels that are detected at edges using the ED convolution

	AA_TYPE_FXAA //gaussian blur convolution on pixels that are detected to be an edge using the ED convolution
};

class Camera : public SceneObject{
public:
    Vector direction;
    Vector up;
    double zmin, zmax; // zmin is also the location of the image plane
    double width;      // Width at zmin, the image plane
	int imageWidth;
	int imageHeight;
	AntiAliasingType aa;
    bool perspective;
	bool grayscale;
	string name;
	DiagnosticStatus diagnosticStatus;
	long long raysTraced;

	//Class variables for optimization
	double height,width2,height2;
	double gapWidth2,gapHeight2, aspect;
	int sqrtNumSamples;

	explicit Camera(ifstream &f);
	Color renderPixel(int x, int y, int numSamples, Raytracer *raytracer);
	void renderScene(string filename, int cameraNum, Raytracer *raytracer);

	void renderChunk(BMP image, int samples, int startX, int endX, int startY, int endY, Raytracer *raytracer);
};

#endif
