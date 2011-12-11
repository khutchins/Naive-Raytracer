#include "Raytrace.h"
#include "VectorMath.h"

using namespace std;

queue<Camera*> cameraQ;
queue<Light*> lightQ;
queue<Plane*> planeQ;
queue<Sphere*> sphereQ;

const double aspect = (double)H/(double)W;
double zmaxG = 1000;

int cameraNum = 0;

EntityID lastProc = NONE;

DiagnosticStatus DIAGNOSTIC_STATUS = NORMAL;

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
		for(int y = 0; y < H; y++)
		{
			for(int x = 0; x < W; x++)
			{
				double height = c->width * aspect; //Height of image plane
				double width = c->width; //Width of image plane
				bool isPerspective = c->perspective; //False if orthogonal, true if perspective

				double uc = -1*width/2 + width/2 * 2*x/W;
				double vr = -1*height/2 + height/2 * 2*y/H;

				norm(c->direction);
				norm(c->up);

				Vector vLeft = cross3(c->direction,c->up); 

				//Point on image plane
				Point pPointOnImagePlane = c->origin + c->zmin * c->direction + uc * vLeft + vr * c->up;
				
				Vector vCamToImagePlane; //Vector from the camera to the image plane

				if(isPerspective) //If camera is perspective
				{
					vCamToImagePlane = pPointOnImagePlane - c->origin;
				}
				else //If camera is orthogonal
				{
					vCamToImagePlane = c->direction;
				}
				//vCamToImagePlane = c->direction;

				Ray* r = new Ray(); //Ray from the camera to the image plane
				r->dir = vCamToImagePlane;
				
				if(isPerspective)
					r->start = c->origin;
				else
					r->start = pPointOnImagePlane;

				bool lightT = false;
				Color* col = raytrace(r,lightT);
				if(DIAGNOSTIC_STATUS & IS_LIT == IS_LIT) col->adjustColorForDiagnosticIsLit();

				if(c->grayscale) {
					double grayscaleVal = col->r * 0.3 + col->g * 0.59 + col->b * 0.11;
					image(W-x-1,H-y-1)->Red   = (unsigned char)grayscaleVal;
					image(W-x-1,H-y-1)->Green = (unsigned char)grayscaleVal;
					image(W-x-1,H-y-1)->Blue  = (unsigned char)grayscaleVal;
				}
				else {
					image(W-x-1,H-y-1)->Red   = (unsigned char)col->r;
					image(W-x-1,H-y-1)->Green = (unsigned char)col->g;
					image(W-x-1,H-y-1)->Blue  = (unsigned char)col->b;
				}
			    image(W-x-1,H-y-1)->Alpha = 0;

				delete r;
				delete col;
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
	if(closestS)	sDist = dist3(sInt,r->start);
	else			sDist = numeric_limits<double>::max();

	double pDist;
	if(closestP)	pDist = dist3(pInt,r->start);
	else			pDist = numeric_limits<double>::max();

	double lDist;
	if(closestL)	lDist = dist3(lInt,r->start);
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

			llocal = calculateLocalLighting(pInt,closestP->normal,PLANE);

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

			delete materialTexture;
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
		Point lStart = intercept + 0.00001 * (l->origin - intercept);

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
			Vector lToObject = lDir;

			norm(lToObject);

			receivedColor->r /= 255.f;
			receivedColor->g /= 255.f;
			receivedColor->b /= 255.f;

			double lightSourceIntensity = cosAngle(lToObject,normal);
			if(lightSourceIntensity < 0) lightSourceIntensity = 0;
			llocal->r += receivedColor->r * lightSourceIntensity;
			llocal->g += receivedColor->g * lightSourceIntensity;
			llocal->b += receivedColor->b * lightSourceIntensity;
			if(llocal->r > 1) llocal->r = 1;
			if(llocal->g > 1) llocal->g = 1;
			if(llocal->b > 1) llocal->b = 1;
		}
		delete lightRay;
		delete receivedColor;
	}
	return llocal;
}

Color* calculateTextureOnPlaneFromMaterial(Plane* plane, Point intercept) {
	BMP* temp;
	temp = &plane->texture;
	int height = temp->TellHeight();
	int width = temp->TellWidth();
	double planeHeight = plane->height;
	double pixelSize = height / planeHeight; //Width and height of pixel on plane

	//Figure out the boundaries of the plane
	Vector left = cross3(plane->normal,plane->up);

	Point upMid = (plane->up * plane->height * 0.5) + plane->center;
	Point upLeft = upMid + (left * plane->width * 0.5);
	Point upRight = upMid - (left * plane->width * 0.5);
	Point botLeft = upLeft - (plane->up * plane->height);
	Point botRight = upRight - (plane->up * plane->height);

	double heightPercentage = 0;
	double widthPercentage = 0;

	Ray top;
	top.dir = upLeft - upRight;
	top.start = upRight;

	Ray planeUp;
	planeUp.dir = plane->up;
	planeUp.start = intercept;

	// ((p2 - p1)xd1) . (d1xd2) / ||d1xd2||^2
	double tTopPoint = dot3(cross3((planeUp.start - top.start),planeUp.dir),cross3(top.dir,planeUp.dir))/magnitude2(cross3(top.dir,planeUp.dir));

	Ray side;
	side.dir = upLeft - botLeft;
	side.start = botLeft;
	norm(side.dir);

	Ray planeLeft;
	planeLeft.dir = left;
	planeLeft.start = intercept;

	double tLeftPoint = dot3(cross3((planeLeft.start - side.start),planeLeft.dir),cross3(side.dir,planeLeft.dir))/magnitude2(cross3(side.dir,planeLeft.dir));
	
	int pixelX = abs((int)(tTopPoint * width));
	int pixelY = abs((int)(tLeftPoint * height));
	pixelX %= width;
	pixelY %= height;
	Color *matColor = new Color(temp->GetPixel(pixelX,pixelY).Red/255.f, temp->GetPixel(pixelX,pixelY).Green/255.f, temp->GetPixel(pixelX,pixelY).Blue/255.f);
	return matColor;
}

//Calc reflected vector
Color* calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	double angle = dot3(normal,r.dir);
	Vector reflectVec = -2 * angle * normal + r.dir;

	//Generate reflected ray
	Ray reflectRay;
	reflectRay.dir = reflectVec;
	reflectRay.start = intercept + 0.0001 * reflectVec;

	bool lig = false;
	lastProc = id;
	return raytrace(&reflectRay,lig);
	lastProc = NONE;
}

//Calc refracted vector
Color* calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	Ray refractRay;
	refractRay.dir = r.dir;
	refractRay.start = intercept + 0.0001 * r.dir;

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

		Vector dist = tempS->center - r->start;

		norm(dist);
		norm(r->dir);

		double a = dot3(r->dir,r->dir);
		double b = 2 * dot3(r->start - tempS->center,r->dir);
		double c = dot3(r->start - tempS->center,r->start - tempS->center) - tempS->radius * tempS->radius;

		double disc = discrim(a,b,c);

		//No intersection, do nothing
		if(disc < 0) continue;
		else if(disc >= 0) //Find closest intersection
		{
			double discSqrt = sqrt(disc);
			double quad;
			if (b < 0)	quad = (-b - discSqrt)/2.f;
			else quad = (-b + discSqrt)/2.f;

			double t0 = quad/a;
			double t1 = c/quad;
			if(t0 > t1) swap(t0,t1);

			double t;
			if(t0 < 0 && t1 < 0) continue;
			if(t0 < 0) t = t1;
			else t = t0;

			Point tempInt = r->start + t * r->dir;
			double distance = dist3(tempInt,r->start);
			if(distance <= zmaxG)
			{
				if(!closestSphere) //If no previous sphere intersections found
				{
					closestSphere = tempS;
					sInt = tempInt;
				}
				else if(dist3(sInt, r->start) > dist3(tempInt, r->start)) //If closer than previous
				{
					closestSphere = tempS;
					sInt = tempInt;
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

		double dot = dot3(tempP->normal,r->dir);

		if(dot != 0) //If the normal and ray aren't perpendicular (ray and plane parallel)
		{
			double t = dot3(tempP->normal,tempP->center - r->start)	/ dot;
			if(t >= 0) //If the ray is pointing toward the plane
			{
				//Calculate point of intersection on plane
				Point tempInt = r->dir * t + r->start;

				//See if the point of intersection is actually on the finite plane
				//First, find coordinates of vertices that make up the quad
				Vector left = cross3(tempP->normal,tempP->up);
				Point upMid = (tempP->up * tempP->height * 0.5) + tempP->center;
				Point upLeft = upMid + (left * tempP->width * 0.5);
				Point upRight = upMid - (left * tempP->width * 0.5);
				Point botLeft = upLeft - (tempP->up * tempP->height);
				Point botRight = upRight - (tempP->up * tempP->height);

				//Now see if the ray is inbetween all of these rays
				//This if statement isn't entirely correct
				if(tempInt.x <= (max(upLeft.x,max(upRight.x,botLeft.x))) && tempInt.x >= (min(upLeft.x,min(upRight.x,botLeft.x)))
					&& tempInt.y <= (max(upLeft.y,max(upRight.y,botLeft.y))) && tempInt.y >= (min(upLeft.y,min(upRight.y,botLeft.y)))
					&& tempInt.z <= (max(upLeft.z,max(upRight.z,botLeft.z))) && tempInt.z >= (min(upLeft.z,min(upRight.z,botLeft.z))))
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

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		if(word == "camera")
		{
			Camera* c = new Camera(sceneFile);
			cameraQ.push(c);
		}
		else if(word == "plane")
		{
			Plane* p = new Plane(sceneFile);
			planeQ.push(p);
		}
		else if(word == "sphere")
		{
			Sphere* s = new Sphere(sceneFile);
			sphereQ.push(s);
		}
		else if(word == "light")
		{
			Light* l = new Light(sceneFile);
			lightQ.push(l);
		}
		else
		{
			cout << "Incorrect syntax in line \n\"" << line << "\"." << endl;
			return 0;
		}
	}
	return 1;
}