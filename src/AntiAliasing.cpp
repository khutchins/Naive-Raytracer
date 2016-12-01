#include "AntiAliasing.h"
#include "Raytracer.h"
#include "Convolution.h"

/*
====================
pixelExceedsThreshold
	Takes a color and a threshold, and sees if the three color values added
	together exceed the threshold.  An example for its use is to see if the
	edge detection convolution considers it an edge
====================
*/
bool pixelExceedsThreshold(Color c, int threshold) {
	return (c.r + c.g + c.b > threshold);
}

/*
====================
generateNaiveAABMP
	Takes a rendered image, applies the Gaussian blur convolution to it,
	and returns the result.
====================
*/
BMP generateNaiveAABMP(const BMP originalImage) {
	return Convolution::generateConvolutionBitmap(originalImage,Convolution::getGaussianBlurConvolution());
}

/*
====================
generateEDAABMP
	Takes the camera used to render the scene as well as a BMP object 
	containing the rendered image.  EDAA generates an edge detection bitmap
	first and then renders additional samples where the edges are calculated 
	to be.  The result is returned in the original BMP passed in.
====================
*/
BMP generateEDAABMP(Camera *c, BMP& originalImage, Raytracer* raytracer) {
	int numSamples;
	if(c->aa == AA_TYPE_EDAA_4) numSamples = 4;
	else						numSamples = 16;

	int imageWidth = originalImage.TellWidth();
	int imageHeight = originalImage.TellHeight();

	BMP edgeDetectionBMP = Convolution::generateConvolutionBitmap(originalImage,Convolution::getEdgeDetectionConvolution());

	for(int x = 0; x < imageWidth; x++) {
		for(int y = 0; y < imageHeight; y++) {
			Color edc = Color::colorFromRGBAPixel(edgeDetectionBMP(imageWidth-x-1,imageHeight-y-1));
			if(pixelExceedsThreshold(edc,EDAA_THRESHOLD)) {
				Color col = c->renderPixel(x,y,numSamples,raytracer);

				if(c->diagnosticStatus == DIAGNOSTIC_EDAA_THRESHOLD)	originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorWhite().RGBAPixel());
				else if(c->grayscale)									originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorGrayscale(col.grayscaleValue()).RGBAPixel());
				else													originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,col.RGBAPixel());
			}
			else if(c->diagnosticStatus == DIAGNOSTIC_EDAA_THRESHOLD)	originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorBlack().RGBAPixel());
		}
	}
	return originalImage;
}

/*
====================
generateFXAABMP
	Takes the BMP object containing the rendered image.  FXAA generates an 
	edge detection bitmap first and then smooths the image where the edges 
	are calculated to be.  The result is returned in the original BMP passed in.
	It's similar to EDAA, but smooths the object instead of rendering additional 
	samples.  It should be less computationally expensive.
====================
*/
BMP generateFXAABMP(Camera *c, BMP& originalImage) {
	int imageWidth = originalImage.TellWidth();
	int imageHeight = originalImage.TellHeight();

	BMP edgeDetectionBMP = Convolution::generateConvolutionBitmap(originalImage,Convolution::getEdgeDetectionConvolution());
	Convolution::Convolution gaussianBlurConvolution = Convolution::getGaussianBlurConvolution();

	for(int x = 0; x < imageWidth; x++) {
		for(int y = 0; y < imageHeight; y++) {
			Color edc = Color::colorFromRGBAPixel(edgeDetectionBMP(imageWidth-x-1,imageHeight-y-1));
			if(pixelExceedsThreshold(edc,EDAA_THRESHOLD)) {
				Color col = Convolution::convolutePoint(imageWidth-x-1,imageHeight-y-1,&originalImage,gaussianBlurConvolution);

				if(c->diagnosticStatus == DIAGNOSTIC_EDAA_THRESHOLD)	originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorWhite().RGBAPixel());
				else													originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,col.RGBAPixel());
			}
			else if(c->diagnosticStatus == DIAGNOSTIC_EDAA_THRESHOLD) originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorBlack().RGBAPixel());
		}
	}
	return originalImage;
}