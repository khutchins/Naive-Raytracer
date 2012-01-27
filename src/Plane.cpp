#include "Plane.h"

Plane::Plane(ifstream &f)
{
	this->isLight = false;
	this->isVisible = true;
	this->hasTexture = false;
	this->objectType = PLANE;

	while(!f.eof())
	{
		string line;
		getline(f,line);

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		//words with three arguments
		if(word == "origin" || word == "color" || word == "normal" || word == "up")
		{
			double num1 = 0;
			double num2 = 0;
			double num3 = 0;

			if(lineContents.size() < 3) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num3 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "origin") //read in the origin coordinates
			{
				this->origin.x = num1;
				this->origin.y = num2;
				this->origin.z = num3;
			}
			else if(word == "color") //read in the color coordinates
			{
				this->material.color.r = num1;
				this->material.color.g = num2;
				this->material.color.b = num3;
			}
			else if(word == "normal")
			{
				this->normal.x = num1;
				this->normal.y = num2;
				this->normal.z = num3;
				norm(this->normal);
			}
			else if(word == "up")
			{
				this->up.x = num1;
				this->up.y = num2;
				this->up.z = num3;
				norm(this->up);
			}
		}

		//words with two arguments
		else if(word == "size")
		{
			double num1 = 0;
			double num2 = 0;

			if(lineContents.size() < 2) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();
			num2 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			this->width = num1;
			this->height = num2;
		}

		//words with one double argument
		else if(word == "reflect" || word == "transparency")
		{
			double num1 = 0;

			if(lineContents.size() < 1) break;

			num1 = (double)atof(lineContents.front().c_str());
			lineContents.pop();

			if(word == "reflect")			this->material.reflection = num1;
			else if (word == "transparency")this->material.transparency = num1;
		}

		//words with one string argument
		else if(word == "texture")
		{
			string textureName;

			if(lineContents.size() < 1) break;

			textureName = lineContents.front();
			lineContents.pop();

			if(this->texture.ReadFromFile(textureName.c_str()))
				this->hasTexture = true;
		}
		else
			break;
	}

	//Warning: Vectors are not orthogonal
	if(abs(dot3(up,normal)) > 0.00001) {
		printf("Warning: Plane up vector ");
		up.print();
		printf(" and normal vector ");
		normal.print();
		printf(" are not orthogonal.\n");
	}
}

Plane::Plane(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin) {
	this->origin = origin;
	this->width = width;
	this->height = height;
	this->material = m;
	this->up = up;
	this->normal = normal;
	this->hasTexture = false;
	if(textureName.size() > 0) if(this->texture.ReadFromFile(textureName.c_str())) this->hasTexture = true;
	this->isLight = false;
	this->isVisible = true;
}


SceneObject* Plane::intersect(Ray* r, Point &intersect) {
	double dot = dot3(this->normal,r->dir);

	if(dot != 0) //If the normal and ray aren't perpendicular (ray and plane parallel)
	{
		double t = dot3(this->normal,this->origin - r->start) / dot;
		if(t >= 0) //If the ray is pointing toward the plane
		{
			//Calculate point of intersection on plane
			Point tempInt = r->dir * t + r->start;

			//See if the point of intersection is actually on the finite plane
			//First, find coordinates of vertices that make up the quad
			Vector left = cross3(this->normal,this->up);
			Point upMid = (this->up * this->height * 0.5) + this->origin;
			Point upLeft = upMid + (left * this->width * 0.5);
			Point upRight = upMid - (left * this->width * 0.5);
			Point botLeft = upLeft - (this->up * this->height);
			Point botRight = upRight - (this->up * this->height);

			//Calculate vectors that define the rectangle and vector to point
			Vector leftV = upRight - upLeft;
			Vector up = botLeft - upLeft;
			Vector cornerToPoint = tempInt - upLeft;

			double dotUpCTP = dot3(up,cornerToPoint);
			double dotUpUp = dot3(up,up);
			double dotLeftCTP = dot3(leftV,cornerToPoint);
			double dotLeftLeft = dot3(leftV,leftV);

			if(0 <= dotUpCTP && dotUpCTP <= dotUpUp && 0 <= dotLeftCTP && dotLeftCTP <= dotLeftLeft) {
				intersect = tempInt;
				return this;
			}
		}
	}
	return NULL;
}

Color Plane::calculateTextureFromMaterial(Point intercept) {
	BMP* temp;
	temp = &this->texture;
	int height = temp->TellHeight();
	int width = temp->TellWidth();
	double planeHeight = this->height;
	double pixelSize = height / planeHeight; //Width and height of pixel on plane

	//Figure out the boundaries of the plane
	Vector left = cross3(this->normal,this->up);

	Point upMid = (this->up * this->height * 0.5) + this->origin;
	Point upLeft = upMid + (left * this->width * 0.5);
	Point upRight = upMid - (left * this->width * 0.5);
	Point botLeft = upLeft - (this->up * this->height);
	Point botRight = upRight - (this->up * this->height);

	double heightPercentage = 0;
	double widthPercentage = 0;

	Ray top;
	top.dir = upLeft - upRight;
	top.start = upRight;

	Ray planeUp;
	planeUp.dir = this->up;
	planeUp.start = intercept;

	// ((p2 - p1)xd1) . (d1xd2) / ||d1xd2||^2
	//percentage along top of plane
	double tTopPoint = dot3(cross3((planeUp.start - top.start),planeUp.dir),cross3(top.dir,planeUp.dir))/magnitude2(cross3(top.dir,planeUp.dir));

	Ray side;
	side.dir = upLeft - botLeft;
	side.start = botLeft;
	norm(side.dir);

	Ray planeLeft;
	planeLeft.dir = left;
	planeLeft.start = intercept;

	//percentage along side of plane
	double tLeftPoint = 1 - (1.f/this->height) * dot3(cross3((planeLeft.start - side.start),planeLeft.dir),cross3(side.dir,planeLeft.dir))/magnitude2(cross3(side.dir,planeLeft.dir));
	
	int pixelX = abs((int)(tTopPoint * width));
	int pixelY = abs((int)(tLeftPoint * height));
	pixelX %= width;
	pixelY %= height;
	Color matColor;
	if(DIAGNOSTIC_STATUS == TEXTURE_MAPPING) {
		matColor.r = tLeftPoint;
		matColor.b = 1;
		matColor.g = tTopPoint;
	}
	else {
		matColor.r = temp->GetPixel(pixelX,pixelY).Red/255.f;
		matColor.g = temp->GetPixel(pixelX,pixelY).Green/255.f;
		matColor.b = temp->GetPixel(pixelX,pixelY).Blue/255.f;
	}
	return matColor;
}

Vector Plane::calculateNormalForPoint(Point p, Point raySource) {
	double distNormalSide = dist3Compare(p + this->normal, raySource);
	double distOtherSide = dist3Compare(p + this->normal*-1, raySource);
	if(distOtherSide < distNormalSide) return this->normal * -1;
	return this->normal;
}

double Plane::getReflection() {
	return this->material.reflection;
}

double Plane::getRefraction() {
	return this->material.transparency;
}

Color Plane::getColor() {
	return this->material.color;
}
