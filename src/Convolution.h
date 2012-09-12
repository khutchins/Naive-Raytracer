#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include "EasyBMP/EasyBMP.h"
#include "Color.h"
#include <vector>

using namespace std;

typedef vector<vector<double>> Convolution;

Color convolutePoint(int i_x, int i_y, BMP *originalImage, const Convolution &convolution);
BMP generateConvolutionBitmap(BMP originalImage, const Convolution &convolution);

Convolution getEdgeDetectionConvolution();
Convolution getGaussianBlurConvolution();

#endif