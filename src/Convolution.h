#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include "EasyBMP/EasyBMP.h"
#include "Color.h"
#include <vector>

using namespace std;

typedef vector<vector<double>> convolution;

BMP generateConvolutionBitmap(BMP originalImage, convolution convolution);

convolution getEdgeDetectionConvolution();
convolution getGaussianBlurConvolution();

#endif