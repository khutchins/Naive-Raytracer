#ifndef _ANTI_ALIASING_H_
#define _ANTI_ALIASING_H_

#include "EasyBMP/EasyBMP.h"
#include "Convolution.h"
#include <vector>
#include "Camera.h"

const int EDAA_THRESHHOLD = 10;

class Camera;

BMP generateNaiveAABMP(BMP originalImage);
BMP generateEDAABMP(Camera *c, BMP &originalImage);
//BMP generateEDBAABMP(Camera *c, BMP originalImage);

#endif
