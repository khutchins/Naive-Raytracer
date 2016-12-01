#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <vector>

#include "Color.h"
#include "EasyBMP/EasyBMP.h"

using namespace std;

namespace Convolution {

	typedef vector<vector<double>> Convolution;

	BMP generateConvolutionBitmap(BMP originalImage, const Convolution &convolution);

	Color convolutePoint(int i_x, int i_y, const BMP *originalImage, const Convolution &convolution);

	Convolution getEdgeDetectionConvolution();

	Convolution getGaussianBlurConvolution();
}

#endif