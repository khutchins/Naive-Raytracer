#include "Raytrace.h"
#include "VectorMath.h"

using namespace std;

queue<Camera*> cameraQ;
queue<SceneObject*> objectQ;

double zmaxG = 1000;

int cameraNum = 0;
int iterations = 0;

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

	for(int i = 0; cameraQ.size() > 0; i++)
	{
		cameraNum = i;
		Camera* c = cameraQ.front();
		
		zmaxG = c->zmax;
		c->renderScene(fn,cameraNum);

		cameraQ.pop();
	}

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
	if(closestO)	oDist = dist3Compare(oInt,r->start);
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

			Vector normal = closestO->calculateNormalForPoint(oInt,r->start);
			norm(normal);

			llocal = calculateLocalLighting(oInt,normal,closestO->objectType);
			iterations++;
			if(closestO->getReflection() != 0 && iterations < 5) reflect = calculateReflectedRay(*r,oInt,normal,closestO->objectType);
			if(closestO->getRefraction() != 0 && iterations < 5) refract = calculateRefractedRay(*r,oInt,normal,closestO->objectType);
			iterations--;
			if(closestO->hasTexture) materialTexture = closestO->calculateTextureFromMaterial(oInt);
			
			double percentDiffuse = 1.f - closestO->getReflection() - closestO->getRefraction();
			if(percentDiffuse < 0) percentDiffuse = 0;

			if(closestO->hasTexture)
			{
				if(DIAGNOSTIC_STATUS == TEXTURE_MAPPING) {
					c.r = materialTexture.r * 255;
					c.g = materialTexture.g * 255;
					c.b = materialTexture.b * 255;
					return c;
				}
				c.r = llocal.r * materialTexture.r * 255 * percentDiffuse + closestO->getReflection() * reflect.r + closestO->getRefraction() * refract.r;
				c.g = llocal.g * materialTexture.g * 255 * percentDiffuse + closestO->getReflection() * reflect.g + closestO->getRefraction() * refract.g;
				c.b = llocal.b * materialTexture.b * 255 * percentDiffuse + closestO->getReflection() * reflect.b + closestO->getRefraction() * refract.b;
			}
			else
			{
				c.r = llocal.r * closestO->getColor().r * 255 * percentDiffuse + closestO->getReflection() * reflect.r + closestO->getRefraction() * refract.r;
				c.g = llocal.g * closestO->getColor().g * 255 * percentDiffuse + closestO->getReflection() * reflect.g + closestO->getRefraction() * refract.g;
				c.b = llocal.b * closestO->getColor().b * 255 * percentDiffuse + closestO->getReflection() * reflect.b + closestO->getRefraction() * refract.b;
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
		Point lStart = intercept + 0.000001 * (l->origin - intercept);

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
	reflectRay.start = intercept + 0.000001 * reflectVec;

	bool lig = false;
	lastProc = id;
	return raytrace(&reflectRay,lig);
	lastProc = NONE;
}

//Calc refracted vector
Color calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	Ray refractRay;
	refractRay.dir = r.dir;
	refractRay.start = intercept + 0.000001 * r.dir;

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
			else if(dist3Compare(objectIntersect, r->start) < dist3Compare(intersect, r->start)) //If closer than previous
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