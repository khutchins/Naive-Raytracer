#include "AntiAliasing.h"
#include "Raytrace.h"

BMP generateAABMP(BMP originalImage) {
	convolution aa2DVector;
	aa2DVector.resize(3);
	for(int i = 0; i < 3; i++) aa2DVector[i].resize(3);

	const double DIV = 16;

	aa2DVector[0][0] = 1.f/DIV;
	aa2DVector[0][1] = 2.f/DIV;
	aa2DVector[0][2] = 1.f/DIV;
	aa2DVector[1][0] = 2.f/DIV;
	aa2DVector[1][1] = 4.f/DIV;
	aa2DVector[1][2] = 2.f/DIV;
	aa2DVector[2][0] = 1.f/DIV;
	aa2DVector[2][1] = 2.f/DIV;
	aa2DVector[2][2] = 1.f/DIV;

	return generateConvolutionBitmap(originalImage,aa2DVector);
}