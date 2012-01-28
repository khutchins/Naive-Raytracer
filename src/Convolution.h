#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include "EasyBMP/EasyBMP.h"
#include "Color.h"
#include <vector>

using namespace std;

BMP generateConvolutionBitmap(BMP originalImage, vector<vector<double>> &convolution);

#endif