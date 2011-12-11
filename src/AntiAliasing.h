#ifndef _ANTI_ALIASING_H_
#define _ANTI_ALIASING_H_

#include "EasyBMP\EasyBMP.h"

typedef enum AntiAliasingType {
	NO_AA,
	NAIVE_AVERAGE
};

BMP generateAABMP(BMP originalImage);

#endif