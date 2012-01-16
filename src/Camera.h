#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cstdlib>
#include "StringProcessing.h"
#include "SceneObject.h"
#include "Point.h"
#include "Raytrace.h"
#include "Color.h"
#include "EasyBMP/EasyBMP.h"
#include "AntiAliasing.h"
#include <fstream>
#include <string>
#include <queue>

using namespace std;

enum AntiAliasingType {
	NO_AA,
	NAIVE_AVERAGE, //weighted average of pixels post processing
	FSAA_4, //full scene anti-aliasing with 4 samples (super-sampling)
	FSAA_16 //full scene anti-aliasing with 16 samples  (super-sampling)
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

	Camera(ifstream &f);
	void renderScene(string filename, int cameraNum);
};

#endif
