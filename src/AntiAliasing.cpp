#include "AntiAliasing.h"
#include "Raytrace.h"

BMP generateAABMP(BMP originalImage, Camera* c) {
	BMP imageAA;
	imageAA.SetSize(c->imageWidth,c->imageHeight);
	imageAA.SetBitDepth(32);

	for(int y = 0; y < c->imageHeight; y++)
	{
		for(int x = 0; x < c->imageWidth; x++)
		{
			int pixelAvg = 4;
			int alpha;
			Color newVal;
			RGBApixel temp = originalImage.GetPixel(x,y);
			alpha = temp.Alpha * 4;
			newVal.r = temp.Red * 4;
			newVal.g = temp.Green * 4;
			newVal.b = temp.Blue * 4;
			//if not left border, add the pixel to the left
			if(x != 0)
			{
				pixelAvg += 2;
				temp = originalImage.GetPixel(x-1,y);
				alpha += temp.Alpha * 2;
				newVal.r += temp.Red * 2;
				newVal.g += temp.Green * 2;
				newVal.b += temp.Blue * 2;				
			}
			//if not upper border, add the pixel above
			if(y != 0)
			{
				pixelAvg += 2;
				temp = originalImage.GetPixel(x,y-1);
				alpha += temp.Alpha * 2;
				newVal.r += temp.Red * 2;
				newVal.g += temp.Green * 2;
				newVal.b += temp.Blue * 2;
			}
			//if not upper left border, add pixel diagonally upleft
			if(x != 0 && y != 0)
			{
				pixelAvg += 1;
				temp = originalImage.GetPixel(x-1,y-1);
				alpha += temp.Alpha;
				newVal.r += temp.Red;
				newVal.g += temp.Green;
				newVal.b += temp.Blue;
			}
			//if not right border, add pixel to right
			if(x != c->imageWidth - 1)
			{
				pixelAvg += 2;
				temp = originalImage.GetPixel(x+1,y);
				alpha += temp.Alpha * 2;
				newVal.r += temp.Red * 2;
				newVal.g += temp.Green * 2;
				newVal.b += temp.Blue * 2;				
			}
			//if not bottom border, add pixel below
			if(y != c->imageHeight - 1)
			{
				pixelAvg += 2;
				temp = originalImage.GetPixel(x,y+1);
				alpha += temp.Alpha * 2;
				newVal.r += temp.Red * 2;
				newVal.g += temp.Green * 2;
				newVal.b += temp.Blue * 2;
			}
			//if not bottom right border, add pixel diagonally downright
			if(x != c->imageWidth - 1 && y != c->imageHeight - 1)
			{
				pixelAvg += 1;
				temp = originalImage.GetPixel(x+1,y+1);
				alpha += temp.Alpha;
				newVal.r += temp.Red;
				newVal.g += temp.Green;
				newVal.b += temp.Blue;
			}
			//if not bottom left border, add pixel diagonally downleft
			if(x != 0 && y != c->imageHeight - 1)
			{
				pixelAvg += 1;
				temp = originalImage.GetPixel(x-1,y+1);
				alpha += temp.Alpha;
				newVal.r += temp.Red;
				newVal.g += temp.Green;
				newVal.b += temp.Blue;
			}
			//if not top right border, add pixel diagonally upright
			if(x != c->imageWidth - 1 && y != 0)
			{
				pixelAvg += 1;
				temp = originalImage.GetPixel(x+1,y-1);
				alpha += temp.Alpha;
				newVal.r += temp.Red;
				newVal.g += temp.Green;
				newVal.b += temp.Blue;
			}
			//average all values and give result to pixel
			alpha /= pixelAvg;
			newVal.r /= pixelAvg;
			newVal.g /= pixelAvg;
			newVal.b /= pixelAvg;
			imageAA(x,y)->Alpha = alpha;
			imageAA(x,y)->Red = (ebmpBYTE)newVal.r;
			imageAA(x,y)->Green = (ebmpBYTE)newVal.g;
			imageAA(x,y)->Blue = (ebmpBYTE)newVal.b;
		}
	}
	return imageAA;
}