#include "Raytrace.h"
#include "VectorMath.h"

using namespace std;

queue<Camera*> cameraQ;
queue<SceneObject*> objectQ;

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

				Color col = raytrace(r,lightT);
				if(DIAGNOSTIC_STATUS == IS_LIT) col.adjustColorForDiagnosticIsLit();

				if(c->grayscale) {
					double grayscaleVal = col.r * 0.3 + col.g * 0.59 + col.b * 0.11;
					image(W-x-1,H-y-1)->Red   = (unsigned char)grayscaleVal;
					image(W-x-1,H-y-1)->Green = (unsigned char)grayscaleVal;
					image(W-x-1,H-y-1)->Blue  = (unsigned char)grayscaleVal;
				}
				else {
					image(W-x-1,H-y-1)->Red   = (unsigned char)col.r;
					image(W-x-1,H-y-1)->Green = (unsigned char)col.g;
					image(W-x-1,H-y-1)->Blue  = (unsigned char)col.b;
				}
			    image(W-x-1,H-y-1)->Alpha = 0;

				delete r;
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

Color raytrace(Ray* r, bool &light)
{
	Color c = Color::ColorBlack();

	//find nearest intersection
	SceneObject* closestO = NULL;
	Point oInt;

	closestO = findClosestObject(r, oInt);
	if(DIAGNOSTIC_STATUS == IS_HIT) {
		if(closestO) return Color::ColorWhite();
		else return Color::ColorBlack();
	}

	double oDist;
	if(closestO)	oDist = dist3(oInt,r->start);
	else			oDist = numeric_limits<double>::max();

	//If >=1 entity found
	if(oDist != numeric_limits<double>::max())
	{
		if(!closestO->isLight && !light) //if not a light
		{
			Color materialTexture;
			Color llocal = Color::ColorBlack();
			Color reflect = Color::ColorBlack();
			Color refract = Color::ColorBlack();

			Vector normal = closestO->calculateNormalForPoint(oInt);
			norm(normal);

			llocal = calculateLocalLighting(oInt,normal,closestO->objectType);
			if(closestO->getReflection() != 0)	 reflect = calculateReflectedRay(*r,oInt,normal,closestO->objectType);
			if(closestO->getRefraction() != 0) refract = calculateRefractedRay(*r,oInt,normal,closestO->objectType);
			if(closestO->hasTexture) materialTexture = closestO->calculateTextureFromMaterial(oInt);
						
			if(closestO->hasTexture)
			{
				c.r = llocal.r * materialTexture.r * 255 + closestO->getReflection() * reflect.r + closestO->getRefraction() * refract.r;
				c.g = llocal.g * materialTexture.g * 255 + closestO->getReflection() * reflect.g + closestO->getRefraction() * refract.g;
				c.b = llocal.b * materialTexture.b * 255 + closestO->getReflection() * reflect.b + closestO->getRefraction() * refract.b;
			}
			else
			{
				c.r = llocal.r * closestO->getColor().r * 255 + closestO->getReflection() * reflect.r + closestO->getRefraction() * refract.r;
				c.g = llocal.g * closestO->getColor().g * 255 + closestO->getReflection() * reflect.g + closestO->getRefraction() * refract.g;
				c.b = llocal.b * closestO->getColor().b * 255 + closestO->getReflection() * reflect.b + closestO->getRefraction() * refract.b;
			}
			light = false;
		}
		else if (closestO->isLight) //Light is closest
		{
			c.r = closestO->getColor().r * 255;
			c.g = closestO->getColor().g * 255;
			c.b = closestO->getColor().b * 255;

			light = true;
		}
		else light = false;
	}

	return c;
}

Color calculateLocalLighting(Point intercept, Vector normal, EntityID id) {
	Color llocal = Color::ColorBlack();
	for(unsigned int i = 0; i < objectQ.size(); i++)
	{
		SceneObject* l = objectQ.front();
		objectQ.pop();
		objectQ.push(l);
		if(!l->isLight) continue;

		Ray* lightRay = new Ray();
		bool lig = true;

		//Start of the ray (moved a bit so we won't intercept the object)
		Point lStart = intercept + numeric_limits<double>::epsilon() * (l->origin - intercept);

		//Direction from the object *to* the light source
		Vector lDir = l->origin - lStart;

		lightRay->dir = lDir;
		lightRay->start = lStart;

		lastProc = id;
		Color receivedColor = raytrace(lightRay,lig);
		lastProc = NONE;

		if(lig) //We see the light from the point
		{
			//Direction from the light source to the plane.  Used for calculating lambert
			Vector lToObject = lDir;

			norm(lToObject);

			receivedColor.r /= 255.f;
			receivedColor.g /= 255.f;
			receivedColor.b /= 255.f;

			double lightSourceIntensity = cosAngle(lToObject,normal);
			if(lightSourceIntensity < 0) lightSourceIntensity = 0;
			llocal.r += receivedColor.r * lightSourceIntensity;
			llocal.g += receivedColor.g * lightSourceIntensity;
			llocal.b += receivedColor.b * lightSourceIntensity;
			if(llocal.r > 1) llocal.r = 1;
			if(llocal.g > 1) llocal.g = 1;
			if(llocal.b > 1) llocal.b = 1;
		}
		delete lightRay;
	}
	return llocal;
}

//Calc reflected vector
Color calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	double angle = dot3(normal,r.dir);
	Vector reflectVec = -2 * angle * normal + r.dir;

	//Generate reflected ray
	Ray reflectRay;
	reflectRay.dir = reflectVec;
	reflectRay.start = intercept + numeric_limits<double>::epsilon() * reflectVec;

	bool lig = false;
	lastProc = id;
	return raytrace(&reflectRay,lig);
	lastProc = NONE;
}

//Calc refracted vector
Color calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	Ray refractRay;
	refractRay.dir = r.dir;
	refractRay.start = intercept + numeric_limits<double>::epsilon() * r.dir;

	bool lig = false;
	lastProc = id;
	return raytrace(&refractRay,lig);
	lastProc = NONE;
}

SceneObject *findClosestObject(Ray *r, Point &intersect) {
	SceneObject *closestObject = NULL;
	for(unsigned int i = 0; i < objectQ.size(); i++) //look for closest object
	{
		SceneObject* tempO = objectQ.front();
		objectQ.pop();
		objectQ.push(tempO);

		Point objectIntersect;
		if(tempO->intersect(r,objectIntersect)) {
			if(!closestObject) //If no previous object intersections found
			{
				closestObject = tempO;
				intersect = objectIntersect;
			}
			else if(dist3(objectIntersect, r->start) < dist3(intersect, r->start)) //If closer than previous
			{
				closestObject = tempO;
				intersect = objectIntersect;
			}
		}
	}

	return closestObject;
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
			objectQ.push(p);
		}
		else if(word == "sphere")
		{
			Sphere* s = new Sphere(sceneFile);
			objectQ.push(s);
		}
		else if(word == "light")
		{
			Light* l = new Light(sceneFile);
			objectQ.push(l);
		}
		else
		{
			cout << "Incorrect syntax in line \n\"" << line << "\"." << endl;
			return 0;
		}
	}
	return 1;
}