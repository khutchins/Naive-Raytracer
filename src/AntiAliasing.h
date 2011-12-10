#ifndef _ANTI_ALIASING_
#define _ANTI_ALIASING_

#include "EasyBMP\EasyBMP.h"

typedef enum AntiAliasingType {
	NO_AA,
	NAIVE_AVERAGE
};

BMP generateAABMP(BMP originalImage);

#endif