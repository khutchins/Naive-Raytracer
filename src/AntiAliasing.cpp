#include "AntiAliasing.h"
#include "Raytrace.h"

BMP generateAABMP(BMP originalImage) {
	convolution aa2DVector = getGaussianBlurConvolution();
	return generateConvolutionBitmap(originalImage,aa2DVector);
}