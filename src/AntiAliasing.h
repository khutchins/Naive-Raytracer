#ifndef _ANTI_ALIASING_H_
#define _ANTI_ALIASING_H_

#include "EasyBMP/EasyBMP.h"
#include "Convolution.h"
#include <vector>
#include "ObjCamera.h"
#include "Raytracer.h"

const int EDAA_THRESHOLD = 10;

class Camera;
class Raytracer;

BMP generateNaiveAABMP(BMP originalImage);
BMP generateEDAABMP(Camera *c, BMP &originalImage, Raytracer *raytracer);
BMP generateFXAABMP(Camera *c, BMP &originalImage);

#endif
