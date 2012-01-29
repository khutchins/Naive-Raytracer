#include "Convolution.h"

BMP generateConvolutionBitmap(BMP originalImage, vector<vector<double>> &convolution) {
	BMP imageConvolution;
	imageConvolution.SetSize(originalImage.TellWidth(),originalImage.TellHeight());

	int imageWidth = originalImage.TellWidth();
	int imageHeight = originalImage.TellHeight();
	
	int convolutionWidth = convolution.size();
	int convolutionHeight = convolution.begin()->size();

	for(int i_y = 0; i_y < imageConvolution.TellHeight(); i_y++) {
		for(int i_x = 0; i_x < imageConvolution.TellWidth(); i_x++) {
			Color newColor = Color::ColorBlack();
			double newAlpha = 0;

			for(int c_y = 0; c_y < convolutionHeight; c_y++) {
				for(int c_x = 0; c_x < convolutionWidth; c_x++) {
					int x = min(max(0,i_x+c_x-(convolutionWidth-1)/2),imageWidth-1);
					int y = min(max(0,i_y+c_y-(convolutionHeight-1)/2),imageHeight-1);

					RGBApixel tempPixel = originalImage.GetPixel(x,y);
					newColor.r += tempPixel.Red * convolution[c_x][c_y];
					newColor.g += tempPixel.Green * convolution[c_x][c_y];
					newColor.b += tempPixel.Blue * convolution[c_x][c_y];
					newAlpha += tempPixel.Alpha * convolution[c_x][c_y];
				}
			}

			newColor.r = min(max(0,(int)newColor.r),255);
			newColor.g = min(max(0,(int)newColor.g),255);
			newColor.b = min(max(0,(int)newColor.b),255);
			newAlpha = min(max(0,(int)newAlpha),255);

			imageConvolution(i_x,i_y)->Red = (ebmpBYTE)newColor.r;
			imageConvolution(i_x,i_y)->Green = (ebmpBYTE)newColor.g;
			imageConvolution(i_x,i_y)->Blue = (ebmpBYTE)newColor.b;
			imageConvolution(i_x,i_y)->Alpha = (ebmpBYTE)newAlpha;
		}
	}
	return imageConvolution;
}