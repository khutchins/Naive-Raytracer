#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include "EasyBMP/EasyBMP.h"
#include "Color.h"
#include <vector>

using namespace std;

typedef vector<vector<double>> Convolution;

BMP generateConvolutionBitmap(BMP originalImage, Convolution convolution);

Convolution getEdgeDetectionConvolution();
Convolution getGaussianBlurConvolution();

#endif