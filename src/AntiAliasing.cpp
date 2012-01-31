#include "AntiAliasing.h"
#include "Raytrace.h"
#include "Convolution.h"

BMP generateNaiveAABMP(BMP originalImage) {
	convolution aa2DVector = getGaussianBlurConvolution();
	return generateConvolutionBitmap(originalImage,aa2DVector);
}

BMP generateEDAABMP(Camera *c, BMP& originalImage) {
	int numSamples;
	if(c->aa == EDAA_4) numSamples = 4;
	else				numSamples = 16;

	int imageWidth = originalImage.TellWidth();
	int imageHeight = originalImage.TellHeight();

	BMP edgeDetectionBMP = generateConvolutionBitmap(originalImage,getEdgeDetectionConvolution());

	for(int x = 0; x < imageWidth; x++) {
		for(int y = 0; y < imageHeight; y++) {
			if(originalImage(x,y)->Red + originalImage(x,y)->Green + originalImage(x,y)->Blue >= EDAA_THRESHHOLD) {
				Color col = c->renderPixel(x,y,numSamples);

				if(c->grayscale) {
					double grayscaleVal = col.r * 0.3 + col.g * 0.59 + col.b * 0.11;
					originalImage(imageWidth-x-1,imageHeight-y-1)->Red   = (unsigned char)grayscaleVal;
					originalImage(imageWidth-x-1,imageHeight-y-1)->Green = (unsigned char)grayscaleVal;
					originalImage(imageWidth-x-1,imageHeight-y-1)->Blue  = (unsigned char)grayscaleVal;
				}
				else {
					originalImage(imageWidth-x-1,imageHeight-y-1)->Red   = (unsigned char)col.r;
					originalImage(imageWidth-x-1,imageHeight-y-1)->Green = (unsigned char)col.g;
					originalImage(imageWidth-x-1,imageHeight-y-1)->Blue  = (unsigned char)col.b;
				}
				originalImage(imageWidth-x-1,imageHeight-y-1)->Alpha = 0;
			}
		}
	}
	return originalImage;
}

/*BMP generateEDBAABMP(Camera *c, BMP originalImage) {
	BMP image;
	image.SetSize(originalImage.TellWidth(),originalImage.TellHeight());
    image.SetBitDepth(32);

	int imageWidth = c->imageWidth;
	int imageHeight = c->imageHeight;

	BMP edgeDetectionBMP = generateConvolutionBitmap(originalImage,getEdgeDetectionConvolution());

	for(int y = 0; y < c->imageHeight; y++) {
		for(int x = 0; x < c->imageWidth; x++) {
			Color col;

			if(originalImage(x,y)->Red + originalImage(x,y)->Green + originalImage(x,y)->Blue >= EDAA_THRESHHOLD) {
				col = c->renderPixel(x,y,1);

				if(c->grayscale) {
					double grayscaleVal = col.r * 0.3 + col.g * 0.59 + col.b * 0.11;
					image(imageWidth-x-1,imageHeight-y-1)->Red   = (unsigned char)grayscaleVal;
					image(imageWidth-x-1,imageHeight-y-1)->Green = (unsigned char)grayscaleVal;
					image(imageWidth-x-1,imageHeight-y-1)->Blue  = (unsigned char)grayscaleVal;
				}
				else {
					image(imageWidth-x-1,imageHeight-y-1)->Red   = (unsigned char)col.r;
					image(imageWidth-x-1,imageHeight-y-1)->Green = (unsigned char)col.g;
					image(imageWidth-x-1,imageHeight-y-1)->Blue  = (unsigned char)col.b;
				}
			}
			else {
				image(imageWidth-x-1,imageHeight-y-1)->Red   = originalImage(imageWidth-x-1,imageHeight-y-1)->Red;
				image(imageWidth-x-1,imageHeight-y-1)->Green = originalImage(imageWidth-x-1,imageHeight-y-1)->Green;
				image(imageWidth-x-1,imageHeight-y-1)->Blue  = originalImage(imageWidth-x-1,imageHeight-y-1)->Blue;
			}
			image(imageWidth-x-1,imageHeight-y-1)->Alpha = 0;
		}
	}
	return originalImage;
}*/