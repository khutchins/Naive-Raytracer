#include "AntiAliasing.h"
#include "Raytracer.h"
#include "Convolution.h"

/*
====================
pixelExceedsThreshhold
	Takes a color and a threshhold, and sees if the three color values added
	together exceed the threshhold.  An example for its use is to see if the 
	edge detection convolution considers it an edge
====================
*/
bool pixelExceedsThreshhold(Color c, int threshhold) {
	return (c.r + c.g + c.b > threshhold);
}

/*
====================
generateNaiveAABMP
	Takes a rendered image, applies the Gaussian blur convolution to it,
	and returns the result.
====================
*/
BMP generateNaiveAABMP(BMP originalImage) {
	Convolution aa2DVector = getGaussianBlurConvolution();
	return generateConvolutionBitmap(originalImage,aa2DVector);
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

	BMP edgeDetectionBMP = generateConvolutionBitmap(originalImage,getEdgeDetectionConvolution());

	for(int x = 0; x < imageWidth; x++) {
		for(int y = 0; y < imageHeight; y++) {
			Color edc = Color::colorFromRGBAPixel(edgeDetectionBMP(imageWidth-x-1,imageHeight-y-1));
			if(pixelExceedsThreshhold(edc,EDAA_THRESHHOLD)) {
				Color col = c->renderPixel(x,y,numSamples,raytracer);

				if(DIAGNOSTIC_STATUS == DIAGNOSTIC_EDAA_THRESHHOLD)		originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorWhite().RGBAPixel());
				else if(c->grayscale)									originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorGrayscale(col.grayscaleValue()).RGBAPixel());
				else													originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,col.RGBAPixel());
			}
			else if(DIAGNOSTIC_STATUS == DIAGNOSTIC_EDAA_THRESHHOLD)	originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorBlack().RGBAPixel());
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
BMP generateFXAABMP(BMP& originalImage) {
	int imageWidth = originalImage.TellWidth();
	int imageHeight = originalImage.TellHeight();

	BMP edgeDetectionBMP = generateConvolutionBitmap(originalImage,getEdgeDetectionConvolution());
	Convolution gaussianBlurConvolution = getGaussianBlurConvolution();

	for(int x = 0; x < imageWidth; x++) {
		for(int y = 0; y < imageHeight; y++) {
			Color edc = Color::colorFromRGBAPixel(edgeDetectionBMP(imageWidth-x-1,imageHeight-y-1));
			if(pixelExceedsThreshhold(edc,EDAA_THRESHHOLD)) {
				Color col = convolutePoint(x,y,&originalImage,gaussianBlurConvolution);

				if(DIAGNOSTIC_STATUS == DIAGNOSTIC_EDAA_THRESHHOLD)	originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorWhite().RGBAPixel());
				else												originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,col.RGBAPixel());
			}
			else if(DIAGNOSTIC_STATUS == DIAGNOSTIC_EDAA_THRESHHOLD) originalImage.SetPixel(imageWidth-x-1,imageHeight-y-1,Color::ColorBlack().RGBAPixel());
		}
	}
	return originalImage;
}