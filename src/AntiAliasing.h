#ifndef _ANTI_ALIASING_H_
#define _ANTI_ALIASING_H_

#include "EasyBMP\EasyBMP.h"
#include "Camera.h"

class Camera;

BMP generateAABMP(BMP originalImage, Camera* c);

#endif