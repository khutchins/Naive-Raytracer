#include "Raytrace.h"
#include "VectorMath.h"

using namespace std;

queue<Camera*> cameraQ;
queue<Light*> lightQ;
queue<Plane*> planeQ;
queue<Sphere*> sphereQ;

const double aspect = (double)H/(double)W;
double zmaxG = 1000;

bool aa = false;
int cameraNum = 0;

EntityID lastProc = NONE;

int main(int argc, char * argv[])
{
	bool manualFilename = false; //If true, use a predefined scene file
	string fn; //name of file
	if(manualFilename)
		fn = "K:\\git\\Naive-Raytracer\\scenes";

	if(argc != 2 && !manualFilename)
		cout << "Incorrect argument amount. Program terminating.\n";
	else if(argc == 2 && !manualFilename)
		fn = argv[1];

	//Read input from file
	if(processInput(fn) == 0) return 0;

    BMP image;
    image.SetSize(W,H);
    image.SetBitDepth(32);
	for(int i = 0; cameraQ.size() > 0; i++)
	{
		cameraNum = i;
		Camera* c = cameraQ.front();
		zmaxG = c->zmax;
		for(int x=0; x<W; x++)
		{
			for(int y=0; y<H; y++)
			{
				double height = c->width * aspect; //Height of image plane
				double width = c->width; //Width of image plane
				bool isPerspective = c->perspective; //False if orthogonal, true if perspective

				double uc = -1*width/2 + width/2 * 2*x/W;
				double vr = -1*height/2 + height/2 * 2*y/H;

				norm(c->direction);
				norm(c->up);

				Vector vLeft = cross3(c->direction,c->up); 

				Point pPointOnImagePlane; //Point on the image plane
				
				pPointOnImagePlane.x = c->origin.x + c->zmin * c->direction.x + uc * vLeft.x + vr * c->up.x;
				pPointOnImagePlane.y = c->origin.y + c->zmin * c->direction.y + uc * vLeft.y + vr * c->up.y;
				pPointOnImagePlane.z = c->origin.z + c->zmin * c->direction.z + uc * vLeft.z + vr * c->up.z;
				
				Vector vCamToImagePlane; //Vector from the camera to the image plane

				if(isPerspective) //If camera is perspective
				{
					vCamToImagePlane = pPointOnImagePlane - c->origin;
				}
				else //If camera is orthogonal
				{
					vCamToImagePlane = c->direction;
				}

				Ray* r = new Ray(); //Ray from the camera to the image plane
				r->dir = vCamToImagePlane;
				
				if(isPerspective)
					r->start = c->origin;
				else
					r->start = pPointOnImagePlane;

				bool lightT = false;
				Color* col = raytrace(r,lightT);

			    image(W-x-1,H-y-1)->Red   = (unsigned char)col->r;
			    image(W-x-1,H-y-1)->Green = (unsigned char)col->g;
			    image(W-x-1,H-y-1)->Blue  = (unsigned char)col->b;
			    image(W-x-1,H-y-1)->Alpha = 0;
			}
		}

		string sceneName = fn.substr(0,fn.length()-4);
		char temp[33];
		_itoa_s(i,temp,10);
		sceneName += "-";
		sceneName += temp;
		sceneName += ".bmp";

		string sceneName_aa = sceneName.substr(0,sceneName.length()-4)+"_aa_naive.bmp";
		generateAABMP(image).WriteToFile(sceneName_aa.c_str());

		image.WriteToFile(sceneName.c_str());
		cameraQ.pop();
	}
	
    BMP texture;
    return 0;
}

Color* raytrace(Ray* r, bool &light)
{
	Color* c = new Color();

	//find nearest intersection
	Sphere* closestS = NULL;
	Point sInt;
	Plane* closestP = NULL;
	Point pInt;
	Light* closestL = NULL;
	Point lInt;

	closestS = findClosestSphere(r, sInt);
	closestP = findClosestPlane(r, pInt);
	closestL = findClosestLight(r, lInt);

	double sDist;
	if(closestS)	sDist = dist3(sInt.x,r->start.x,sInt.y,r->start.y,sInt.z,r->start.z);
	else			sDist = numeric_limits<double>::max();

	double pDist;
	if(closestP)	pDist = dist3(pInt.x,r->start.x,pInt.y,r->start.y,pInt.z,r->start.z);
	else			pDist = numeric_limits<double>::max();

	double lDist;
	if(closestL)	lDist = dist3(lInt.x,r->start.x,lInt.y,r->start.y,lInt.z,r->start.z);
	else			lDist = numeric_limits<double>::max();

	//If >=1 entity found
	if(sDist != numeric_limits<double>::max() || pDist != numeric_limits<double>::max() || lDist != numeric_limits<double>::max())
	{
		if(sDist < pDist && sDist < lDist && !light) //Sphere is closest
		{
			Color *llocal = new Color();
			Color *reflect = new Color();
			Color *refract = new Color();

			Vector sphereNormal = closestS->calculateNormalForPoint(sInt);
			norm(sphereNormal);

			llocal = calculateLocalLighting(sInt,sphereNormal,SPHERE);
			if(closestS->material.reflection != 0)	 reflect = calculateReflectedRay(*r,sInt,sphereNormal,SPHERE);
			if(closestS->material.transparency != 0) refract = calculateRefractedRay(*r,sInt,sphereNormal,SPHERE);
						
			c->r = closestS->material.color.r * llocal->r * 255 + closestS->material.reflection * reflect->r + closestS->material.transparency * refract->r;
			c->g = closestS->material.color.g * llocal->g * 255 + closestS->material.reflection * reflect->g + closestS->material.transparency * refract->g;
			c->b = closestS->material.color.b * llocal->b * 255 + closestS->material.reflection * reflect->b + closestS->material.transparency * refract->b;
			light = false;

			delete llocal;
			delete reflect;
			delete refract;
		}
		else if( pDist < sDist && pDist < lDist && !light) //Plane is closest
		{
			Color* materialTexture = NULL;
			Color* llocal = new Color();
			Color* reflect = new Color();
			Color* refract = new Color();

			llocal = calculateLocalLighting(pInt,closestP->up,PLANE);
			if(closestP->material.reflection != 0)	 reflect = calculateReflectedRay(*r,pInt,closestP->normal,PLANE);
			if(closestP->material.transparency != 0) refract = calculateRefractedRay(*r, pInt, closestP->normal,PLANE);
			if(closestP->hastexture) materialTexture = calculateTextureOnPlaneFromMaterial(closestP,pInt);

			if(materialTexture)
			{
				c->r = llocal->r * materialTexture->r * 255 + closestP->material.reflection * reflect->r + closestP->material.transparency * refract->r;
				c->g = llocal->g * materialTexture->g * 255 + closestP->material.reflection * reflect->g + closestP->material.transparency * refract->g;
				c->b = llocal->b * materialTexture->b * 255 + closestP->material.reflection * reflect->b + closestP->material.transparency * refract->b;
			}
			else
			{
				c->r = llocal->r * closestP->material.color.r * 255 + closestP->material.reflection * reflect->r + closestP->material.transparency * refract->r;
				c->g = llocal->g * closestP->material.color.g * 255 + closestP->material.reflection * reflect->g + closestP->material.transparency * refract->g;
				c->b = llocal->b * closestP->material.color.b * 255 + closestP->material.reflection * reflect->b + closestP->material.transparency * refract->b;
			}
			light = false;

			delete llocal;
			delete reflect;
			delete refract;
		}
		else if ( lDist < pDist && lDist < sDist ) //Light is closest
		{
			c->r = closestL->color.r * 255;
			c->g = closestL->color.g * 255;
			c->b = closestL->color.b * 255;

			light = true;
		}
		else light = false;
	}

	return c;
}

Color* calculateLocalLighting(Point intercept, Vector normal, EntityID id) {
	Color* llocal = new Color();
	for(unsigned int i = 0; i < lightQ.size(); i++)
	{
		Light* l = lightQ.front();
		lightQ.pop();
		lightQ.push(l);

		Ray* lightRay = new Ray();
		bool lig = true;

		//Start of the ray (moved a bit so we won't intercept the object)
		Point lStart;
		lStart.x = intercept.x + 0.01 * (l->origin.x - intercept.x);
		lStart.y = intercept.y + 0.01 * (l->origin.y - intercept.y);
		lStart.z = intercept.z + 0.01 * (l->origin.z - intercept.z);

		//Direction from the object *to* the light source
		Vector lDir = l->origin - lStart;

		lightRay->dir = lDir;
		lightRay->start = lStart;

		lastProc = id;
		Color* receivedColor = raytrace(lightRay,lig);
		lastProc = NONE;

		if(lig) //We see the light from the point
		{
			//Direction from the light source to the plane.  Used for calculating lambert
			Vector lToObject;
			lToObject.x = 1*lDir.x;
			lToObject.y = 1*lDir.y;
			lToObject.z = 1*lDir.z;

			norm(lToObject);

			receivedColor->r /= 255.f;
			receivedColor->g /= 255.f;
			receivedColor->b /= 255.f;

			double lightSourceIntensity = cosAngle(lToObject,normal);
			if(lightSourceIntensity < 0) lightSourceIntensity = 0;
			llocal->r += receivedColor->r * lightSourceIntensity;
			llocal->g += receivedColor->g * lightSourceIntensity;
			llocal->b += receivedColor->b * lightSourceIntensity;
			if(llocal->r > 1)
				llocal->r = 1;
			if(llocal->g > 1)
				llocal->g = 1;
			if(llocal->b > 1)
				llocal->b = 1;
		}
		delete lightRay;
	}
	return llocal;
}

Color* calculateTextureOnPlaneFromMaterial(Plane* plane, Point intercept) {
	Color* materialColor = new Color();
	BMP* temp;
	temp = &plane->texture;
	int height = temp->TellHeight();
	int width = temp->TellWidth();
	double planeHeight = plane->height;
	double pixelSize = height / planeHeight; //Width and height of pixel on plane

	//Figure out the boundaries of the plane
	Vector left = cross3(plane->normal,plane->up);

	Point upMid;
	upMid.x = (plane->up.x * plane->height / 2) + plane->center.x;
	upMid.y = (plane->up.y * plane->height / 2) + plane->center.y;
	upMid.z = (plane->up.z * plane->height / 2) + plane->center.z;

	Point upLeft;
	upLeft.x = upMid.x + (left.x * plane->width / 2);
	upLeft.y = upMid.y + (left.y * plane->width / 2);
	upLeft.z = upMid.z + (left.z * plane->width / 2);

	Point upRight;
	upRight.x = upMid.x - (left.x * plane->width / 2);
	upRight.y = upMid.y - (left.y * plane->width / 2);
	upRight.z = upMid.z - (left.z * plane->width / 2);

	Point botLeft;
	botLeft.x = upLeft.x - (plane->up.x * plane->height);
	botLeft.y = upLeft.y - (plane->up.y * plane->height);
	botLeft.z = upLeft.z - (plane->up.z * plane->height);

	Point botRight;
	botRight.x = upRight.x - (plane->up.x * plane->height);
	botRight.y = upRight.y - (plane->up.y * plane->height);
	botRight.z = upRight.z - (plane->up.z * plane->height);

	double heightPercentage = 0;
	double widthPercentage = 0;

	Ray top;
	top.dir = upLeft - upRight;
	top.start = upRight;

	Ray planeUp;
	planeUp.dir = plane->up;
	planeUp.start = intercept;

	//Point topIntersect;
	// ((p2 - p1)xd1) . (d1xd2) / ||d1xd2||^2
	//double tTopPoint = dot3(cross3((planeUp.start - top.start),planeUp.dir),cross3(top.dir,planeUp.dir))/magnitude(cross3(top.dir,planeUp.dir))/magnitude(cross3(top.dir,planeUp.dir));

	Ray side;
	side.dir = upLeft - botLeft;
	side.start = botLeft;
	norm(side.dir);

	Ray planeLeft;
	planeLeft.dir = left;
	planeLeft.start = intercept;

	//Point sideIntersect;
	//double tLeftPoint = dot3(cross3((planeLeft.start - side.start),planeLeft.dir),cross3(side.dir,planeLeft.dir))/magnitude(cross3(side.dir,planeLeft.dir))/magnitude(cross3(side.dir,planeLeft.dir));

	if(side.dir.x - top.dir.x != 0 && side.dir.x != 0)
	{
		double sideT = (intercept.x - side.start.x)/(side.dir.x - top.dir.x);
		heightPercentage = sideT / side.dir.x;
	}
	else if(side.dir.y - top.dir.y != 0 && side.dir.y != 0)
	{
		double sideT = (intercept.y - side.start.y)/(side.dir.y - top.dir.y);
		heightPercentage = sideT / side.dir.y;
	}
	else if(side.dir.z - top.dir.z != 0 && side.dir.z != 0)
	{
		double sideT = (intercept.z - side.start.z)/(side.dir.z - top.dir.z);
		heightPercentage = sideT / side.dir.z;
	}
	if(top.dir.x - side.dir.x != 0 && top.dir.x != 0)
	{
		double topT = (intercept.x - top.start.x)/(top.dir.x - side.dir.x);
		widthPercentage = topT / top.dir.x;
	}
	else if(top.dir.y - side.dir.y != 0 && top.dir.y != 0)
	{
		double topT = (intercept.y - top.start.y)/(top.dir.y - side.dir.y);
		widthPercentage = topT / top.dir.y;
	}
	else if(top.dir.z - side.dir.z != 0 && top.dir.z != 0)
	{
		double topT = (intercept.z - top.start.z)/(top.dir.z - side.dir.z);
		widthPercentage = topT / top.dir.z;
	}
	
	int pixelX = abs((int)(widthPercentage * width));
	int pixelY = abs((int)(heightPercentage * height));
	pixelX %= width;
	pixelY %= height;
	Color *matColor = new Color(temp->GetPixel(pixelX,pixelY).Red/255.f, temp->GetPixel(pixelX,pixelY).Green/255.f, temp->GetPixel(pixelX,pixelY).Blue/255.f);
	return matColor;
}

//Calc reflected vector
Color* calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	Vector reflectVec;
	double angle = dot3(normal,r.dir);
	reflectVec.x = -2 * angle * normal.x + r.dir.x;
	reflectVec.y = -2 * angle * normal.y + r.dir.y;
	reflectVec.z = -2 * angle * normal.z + r.dir.z;

	//Generate reflected ray
	Ray reflectRay;
	reflectRay.dir = reflectVec;
	reflectRay.start.x = intercept.x + 0.01 * reflectVec.x;
	reflectRay.start.y = intercept.y + 0.01 * reflectVec.y;
	reflectRay.start.z = intercept.z + 0.01 * reflectVec.z;

	bool lig = false;
	lastProc = id;
	return raytrace(&reflectRay,lig);
	lastProc = NONE;
}

//Calc refracted vector
Color* calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	Ray refractRay;
	refractRay.dir = r.dir;
	refractRay.start.x = intercept.x + 0.01 * r.dir.x;
	refractRay.start.y = intercept.y + 0.01 * r.dir.y;
	refractRay.start.z = intercept.z + 0.01 * r.dir.z;

	bool lig = false;
	lastProc = id;
	return raytrace(&refractRay,lig);
	lastProc = NONE;
}

Sphere *findClosestSphere(Ray *r, Point &sInt) {
	Sphere *closestSphere = NULL;
	for(unsigned int i = 0; i < sphereQ.size(); i++) //look for closest sphere
	{
		Sphere* tempS = sphereQ.front();
		sphereQ.pop();
		sphereQ.push(tempS);

		Vector dist;
		dist.x = tempS->center.x - r->start.x;
		dist.y = tempS->center.y - r->start.y;
		dist.z = tempS->center.z - r->start.z;

		norm(dist.x,dist.y,dist.z);
	
		double a = magnitude(r->dir);
		double b = 2 * dot3(r->start - tempS->center,r->dir);
		double c = magnitude(r->start - tempS->center) - tempS->radius * tempS->radius;

		double disc = discrim(a,b,c);

		if(disc < 0) //No intersection, do nothing
			;
		else if(disc >= 0) //Find closest intersection
		{
			double t = quadratic2(a,b,c);
			double tempIntX = r->start.x + t * r->dir.x;
			double tempIntY = r->start.y + t * r->dir.y;
			double tempIntZ = r->start.z + t * r->dir.z;
			double distance = dist3(tempIntX,r->start.x,tempIntY,r->start.y,tempIntZ,r->start.z);
			if(distance <= zmaxG)
			{
				if(!closestSphere) //If no previous sphere intersections found
				{
					closestSphere = tempS;
					sInt.x = tempIntX;
					sInt.y = tempIntY;
					sInt.z = tempIntZ;
				}
				else if(dist3(sInt, r->start) < dist3(tempIntX, r->start.x, tempIntY, r->start.y, tempIntZ, r->start.z)) //If closer than previous
				{
					closestSphere = tempS;
					sInt.x = tempIntX;
					sInt.y = tempIntY;
					sInt.z = tempIntZ;
				}
			}
		}
	}

	return closestSphere;
}

Plane *findClosestPlane(Ray *r, Point &pInt) {
	Plane *closestPlane = NULL;
	for(unsigned int i = 0; i < planeQ.size(); i++) //look for closest plane
	{
		Plane* tempP = planeQ.front();
		planeQ.pop();
		planeQ.push(tempP);

		double dot = dot3(tempP->normal.x,r->dir.x,tempP->normal.y,r->dir.y,tempP->normal.z,r->dir.z);

		if(dot != 0) //If the normal and ray aren't perpendicular (ray and plane parallel)
		{
			double t = dot3(tempP->normal,tempP->center - r->start)	/ dot3(tempP->normal,r->dir);

			if(t >= 0) //If the ray is pointing toward the plane
			{
				//Calculate point of intersection on plane
				Point tempInt;
				tempInt.x = r->dir.x * t + r->start.x;
				tempInt.y = r->dir.y * t + r->start.y;
				tempInt.z = r->dir.z * t + r->start.z;

				Vector inter;
				inter.x = t * r->dir.x + r->start.x;
				inter.y = t * r->dir.y + r->start.y;
				inter.z = t * r->dir.z + r->start.z;

				//See if the point of intersection is actually on the finite plane
				//First, find coordinates of vertices that make up the quad
				Vector left = cross3(tempP->normal,tempP->up);

				Vector upMid;
				upMid.x = (tempP->up.x * tempP->height / 2) + tempP->center.x;
				upMid.y = (tempP->up.y * tempP->height / 2) + tempP->center.y;
				upMid.z = (tempP->up.z * tempP->height / 2) + tempP->center.z;

				Vector upLeft;
				upLeft.x = upMid.x + (left.x * tempP->width / 2);
				upLeft.y = upMid.y + (left.y * tempP->width / 2);
				upLeft.z = upMid.z + (left.z * tempP->width / 2);

				Vector upRight;
				upRight.x = upMid.x - (left.x * tempP->width / 2);
				upRight.y = upMid.y - (left.y * tempP->width / 2);
				upRight.z = upMid.z - (left.z * tempP->width / 2);

				Vector botLeft;
				botLeft.x = upLeft.x - (tempP->up.x * tempP->height);
				botLeft.y = upLeft.y - (tempP->up.y * tempP->height);
				botLeft.z = upLeft.z - (tempP->up.z * tempP->height);

				Vector botRight;
				botRight.x = upRight.x - (tempP->up.x * tempP->height);
				botRight.y = upRight.y - (tempP->up.y * tempP->height);
				botRight.z = upRight.z - (tempP->up.z * tempP->height);

				//Now see if the ray is inbetween all of these rays
				if(inter.x <= (max(upLeft.x,max(upRight.x,botLeft.x))) && inter.x >= (min(upLeft.x,min(upRight.x,botLeft.x)))
					&& inter.y <= (max(upLeft.y,max(upRight.y,botLeft.y))) && inter.y >= (min(upLeft.y,min(upRight.y,botLeft.y)))
					&& inter.z <= (max(upLeft.z,max(upRight.z,botLeft.z))) && inter.z >= (min(upLeft.z,min(upRight.z,botLeft.z))))
				{
					//See if it's the closest plane so far
					if(!closestPlane)
					{
						closestPlane = tempP;
						pInt = tempInt;
					}
					if(dist3(r->start, pInt) > dist3(r->start, tempInt))
					{
						closestPlane = tempP;
						pInt = tempInt;
					}
				}
			}
		}
	}
	return closestPlane;
}

Light *findClosestLight(Ray *r, Point &lInt) {
	Light *closestLight = NULL;
	for(unsigned int i = 0; i < lightQ.size(); i++) //look for closest light
	{
		Light* tempL = lightQ.front();
		lightQ.pop();
		lightQ.push(tempL);
		
		//Four cases here
		//tempL->origin.x - r->start.x == 0 && r->dir.x == 0.  This is fine, they intersect at all points.  Assign infinity
		//tempL->origin.x - r->start.x != 0 && r->dir.x == 0.  These will *never* hit. Assign quiet NaN
		//tempL->origin.x - r->start.x == 0 && r->dir.x != 0.  This only hits if all are zero. Assign zero
		//tempL->origin.x - r->start.x != 0 && r->dir.x != 0.  This will hit at the point calculated.  Assign that t
		Vector t; //Increments to each intercept. If they are roughly the same, we intersect the light
		if(r->dir.x == 0 && abs(tempL->origin.x - r->start.x) < 0.001)	t.x = std::numeric_limits<double>::infinity();
		else if(r->dir.x == 0)											t.x = std::numeric_limits<double>::quiet_NaN();
		else															t.x = (tempL->origin.x - r->start.x) / r->dir.x;
		if(r->dir.y == 0 && abs(tempL->origin.y - r->start.y) < 0.001)	t.y = std::numeric_limits<double>::infinity();
		else if(r->dir.y == 0)											t.y = std::numeric_limits<double>::quiet_NaN();
		else															t.y = (tempL->origin.y - r->start.y) / r->dir.y;
		if(r->dir.z == 0 && abs(tempL->origin.z - r->start.z) < 0.001)	t.z = std::numeric_limits<double>::infinity();
		else if(r->dir.z == 0)											t.z = std::numeric_limits<double>::quiet_NaN();
		else															t.z = (tempL->origin.z - r->start.z) / r->dir.z;

		//if light is behind the camera, go to the next one
		if(t.x < 0 || t.y < 0 || t.z < 0) continue;

		bool txyEquiv = (abs(t.x-t.y) < 0.001) || t.x == std::numeric_limits<double>::infinity() || t.y == std::numeric_limits<double>::infinity();
		bool tyzEquiv = (abs(t.y-t.z) < 0.001) || t.y == std::numeric_limits<double>::infinity() || t.z == std::numeric_limits<double>::infinity();
		bool txzEquiv = (abs(t.x-t.z) < 0.001) || t.x == std::numeric_limits<double>::infinity() || t.z == std::numeric_limits<double>::infinity();

		if(txyEquiv && tyzEquiv && txzEquiv) //If the ts are roughly the same
		{
			if(!closestLight)
			{
				closestLight = tempL;
				lInt = tempL->origin;
			}
			else if(dist3(tempL->origin,r->start) < dist3(closestLight->origin, r->start))
			{
				closestLight = tempL;
				lInt = tempL->origin;
			}
		}
	}
	return closestLight;
}

Camera* makeCamera(ifstream &f)
{
	Camera* c = new Camera();

	while(!f.eof())
	{
		string line;
		getline(f,line);

		while(line.length() > 0 && line[0] == ' ')
			line = line.substr(1,line.length());
		if(line.length() < 1)
			continue;
		if(line[0] == '#' || line[0] == '\n' || line[0] == '\r')
			continue;

		size_t space = line.find(' '); //Find and extract first word
		string word = line.substr(0,space);
		string restOfLine = line.substr(space+1,line.length());

		if(word == "origin" || word == "direction" || word == "up")
		{
			string numS1;
			string numS2;
			string numS3;
			double num1 = 0;
			double num2 = 0;
			double num3 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS2 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS3 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());
			num2 = (double)atof(numS2.c_str());
			num3 = (double)atof(numS3.c_str());

			if(word == "origin") //read in the origin coordinates
			{
				c->origin.x = num1;
				c->origin.y = num2;
				c->origin.z = num3;
			}
			else if(word == "direction")
			{
				c->direction.x = num1;
				c->direction.y = num2;
				c->direction.z = num3;
			}
			else if(word == "up")
			{
				c->up.x = num1;
				c->up.y = num2;
				c->up.z = num3;
			}
		}

		else if(word == "z")
		{
			string numS1;
			string numS2;
			double num1 = 0;
			double num2 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS2 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());
			num2 = (double)atof(numS2.c_str());

			c->zmin = num1;
			c->zmax = num2;
		}

		else if(word == "width" || word == "perspective")
		{
			string numS1;
			double num1 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			bool pers;
			restOfLine = restOfLine.substr(space+1,line.length());

			if(numS1[0] == '0')
				pers = false;
			else
				pers = true;
			num1 = (double)atof(numS1.c_str());

			if(word == "width") //read in the color coordinates
			{
				c->width = num1;
			}
			else
			{
				c->perspective = pers;
			}
		}
		else
			break;
	}

	return c;
}

Light* makeLight(ifstream &f)
{
	Light* l = new Light();

	while(!f.eof())
	{
		string line;
		getline(f,line);

		while(line.length() > 0 && line[0] == ' ')
			line = line.substr(1,line.length());
		if(line.length() < 1)
			continue;
		if(line[0] == '#' || line[0] == '\n' || line[0] == '\r')
			continue;

		size_t space = line.find(' '); //Find and extract first word
		string word = line.substr(0,space);
		string restOfLine = line.substr(space+1,line.length());

		string numS1;
		string numS2;
		string numS3;
		double num1 = 0;
		double num2 = 0;
		double num3 = 0;

		space = restOfLine.find(' ');
		numS1 = restOfLine.substr(0,space);
		restOfLine = restOfLine.substr(space+1,line.length());

		space = restOfLine.find(' ');
		numS2 = restOfLine.substr(0,space);
		restOfLine = restOfLine.substr(space+1,line.length());

		space = restOfLine.find(' ');
		numS3 = restOfLine.substr(0,space);
		restOfLine = restOfLine.substr(space+1,line.length());

		num1 = (double)atof(numS1.c_str());
		num2 = (double)atof(numS2.c_str());
		num3 = (double)atof(numS3.c_str());

		if(word == "origin") //read in the origin coordinates
		{
			l->origin.x = num1;
			l->origin.y = num2;
			l->origin.z = num3;
		}
		else if(word == "color") //read in the color coordinates
		{
			l->color.r = num1;
			l->color.g = num2;
			l->color.b = num3;
		}
		else
			break;

	}

	return l;
}

Plane* makePlane(ifstream &f)
{
	Plane* p = new Plane();
	p->hastexture = false;

	while(!f.eof())
	{
		string line;
		getline(f,line);

		while(line.length() > 0 && line[0] == ' ')
			line = line.substr(1,line.length());
		if(line.length() < 1)
			continue;
		if(line[0] == '#' || line[0] == '\n' || line[0] == '\r')
			continue;

		size_t space = line.find(' '); //Find and extract first word
		string word = line.substr(0,space);
		string restOfLine = line.substr(space+1,line.length());

		if(word == "origin" || word == "color" || word == "normal" || word == "up")
		{
			string numS1;
			string numS2;
			string numS3;
			double num1 = 0;
			double num2 = 0;
			double num3 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS2 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS3 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());
			num2 = (double)atof(numS2.c_str());
			num3 = (double)atof(numS3.c_str());

			if(word == "origin") //read in the origin coordinates
			{
				p->center.x = num1;
				p->center.y = num2;
				p->center.z = num3;
			}
			else if(word == "color") //read in the color coordinates
			{
				p->material.color.r = num1;
				p->material.color.g = num2;
				p->material.color.b = num3;
			}
			else if(word == "normal")
			{
				p->normal.x = num1;
				p->normal.y = num2;
				p->normal.z = num3;
				norm(p->normal);
			}
			else if(word == "up")
			{
				p->up.x = num1;
				p->up.y = num2;
				p->up.z = num3;
				norm(p->up);
			}
		}

		else if(word == "size")
		{
			string numS1;
			string numS2;
			double num1 = 0;
			double num2 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS2 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());
			num2 = (double)atof(numS2.c_str());

			p->width = num1;
			p->height = num2;
		}

		else if(word == "reflect" || word == "transparency")
		{
			string numS1;
			double num1 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());

			if(word == "reflect") //read in the color coordinates
			{
				p->material.reflection = num1;
			}
			else
			{
				p->material.transparency = num1;
			}
		}

		else if(word == "texture")
		{
			p->hastexture = true;
			string textName;
			space = restOfLine.find(' ');
			p->texture.ReadFromFile(restOfLine.substr(0,space).c_str());
		}
		else
			break;
	}
	return p;
}

Sphere* makeSphere(ifstream &f)
{
	Sphere* s = new Sphere();

	while(!f.eof())
	{
		string line;
		getline(f,line);

		while(line.length() > 0 && line[0] == ' ')
			line = line.substr(1,line.length());
		if(line.length() < 1)
			continue;
		if(line[0] == '#' || line[0] == '\n' || line[0] == '\r')
			continue;

		size_t space = line.find(' '); //Find and extract first word
		string word = line.substr(0,space);
		string restOfLine = line.substr(space+1,line.length());

		if(word == "origin" || word == "color")
		{
			string numS1;
			string numS2;
			string numS3;
			double num1 = 0;
			double num2 = 0;
			double num3 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS2 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			space = restOfLine.find(' ');
			numS3 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());
			num2 = (double)atof(numS2.c_str());
			num3 = (double)atof(numS3.c_str());

			if(word == "origin") //read in the origin coordinates
			{
				s->center.x = num1;
				s->center.y = num2;
				s->center.z = num3;
			}
			else if(word == "color") //read in the color coordinates
			{
				s->material.color.r = num1;
				s->material.color.g = num2;
				s->material.color.b = num3;
			}
		}

		else if(word == "radius" || word == "reflect" || word == "transparency")
		{
			string numS1;
			double num1 = 0;

			space = restOfLine.find(' ');
			numS1 = restOfLine.substr(0,space);
			restOfLine = restOfLine.substr(space+1,line.length());

			num1 = (double)atof(numS1.c_str());

			if(word == "radius") //read in the origin coordinates
			{
				s->radius = num1;
			}
			else if(word == "reflect") //read in the color coordinates
			{
				s->material.reflection = num1;
			}
			else
			{
				s->material.transparency = num1;
			}
		}
		else
			break;
	}

	return s;
}

int processInput(string filename) 
{
	ifstream sceneFile;
	string line; //We will be temporarily storing input here

	sceneFile.open(filename.c_str()); //Open file
	
	if(!sceneFile.is_open())
	{
		cout << "Scene file failed to open.\n";
		return 0;
	}
	while(!sceneFile.eof()) //Loop through all input
	{
		getline(sceneFile,line);

		while(line.length() > 0 && line[0] == ' ')
			line = line.substr(1,line.length());
		if(line.length() < 1)
			continue;
		if(line[0] == '#' || line[0] == '\n' || line[0] == '\r')
			continue;

		size_t space = line.find(' '); //Find and extract first word
		string word = line.substr(0,space);

		if(word == "camera")
		{
			cameraQ.push(makeCamera(sceneFile));
		}
		else if(word == "plane")
		{
			planeQ.push(makePlane(sceneFile));
		}
		else if(word == "sphere")
		{
			sphereQ.push(makeSphere(sceneFile));
		}
		else if(word == "light")
		{
			lightQ.push(makeLight(sceneFile));
		}
		else
		{
			cout << "Incorrect syntax in line \n\"" << line << "\"." << endl;
			return 0;
		}
	}
	return 1;
}