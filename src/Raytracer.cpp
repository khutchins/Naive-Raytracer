#include "Raytracer.h"
#include "VectorMath.h"
#include <random>

using namespace std;

//maximum number of iterations
#define MAXIMUM_ITERATIONS 5

/*
====================
Raytracer::Raytracer()
	Initializes values for raytracer's execution
====================
*/
Raytracer::Raytracer() {
	zmaxG = 1000;
	cameraNum = 0;
	iterations = 0;
	currentCamera = nullptr;
}

/*
====================
Raytracer::~Raytracer()
	Releases raytracer's memory
====================
*/
Raytracer::~Raytracer() {
	for(int i = 0; objectQueue.size() > 0; i++) {
		SceneObject* object = objectQueue.front();
		objectQueue.pop();
		delete object;
	}
}

/*
====================
Raytracer::start
	The initiating function.  Processes inputs and has the cameras all render 
	their scenes
====================
*/
int Raytracer::start(string fn) {
	//Read input from file
	if(processInput(fn) == 0) return 0;

	for(int i = 0; cameraQueue.size() > 0; i++) {
		cameraNum = i;
		Camera* c = cameraQueue.front();
		
		zmaxG = c->zmax;
		currentCamera = c;
		c->renderScene(fn,cameraNum,this);

		cameraQueue.pop();

		delete c;
	}

    return 0;
}

/*
====================
Raytracer::raytrace
	computes the color viewable by the ray r.  Light is set to true if the 
	closest object is a light
====================
*/
Color Raytracer::raytrace(Ray* r, bool &lightWasSeen, bool lightRay) {
	incrementCameraRayCount();
	Color c = Color::ColorBlack();

	//find nearest intersection
	SceneObject* closestO = NULL;
	Point oInt;

	closestO = findClosestObject(r, oInt);
	if(getCameraDiagnosticStatus() == DIAGNOSTIC_IS_HIT) {
		if(closestO) return Color::ColorWhite();
		else return Color::ColorBlack();
	}

	double oDist;
	if(closestO)	oDist = oInt.comparativeDistanceFrom(r->start);
	else			oDist = numeric_limits<double>::max();

	//If > 0 entities found
	if(closestO != nullptr && oDist != numeric_limits<double>::max()) {
		if(!closestO->isLight && lightRay) { //if not a light and we're looking for a light
			lightWasSeen = false;
			return Color::ColorBlack();
		}
		else if(!closestO->isLight) { //if not a light
			Color materialTexture;
			Color llocal;
			Color reflect = Color::ColorBlack();
			Color refract = Color::ColorBlack();
			double percentDiffuse;

			Vector normal = closestO->calculateNormalForPoint(oInt,r->start);
			normal.normalize();

			if(getCameraDiagnosticStatus() == DIAGNOSTIC_FULLBRIGHT_AND_DIFFUSE) {
				llocal = Color::ColorWith(1,1,1);
				percentDiffuse = 1;
				if(closestO->hasTexture) materialTexture = closestO->calculateTextureFromMaterial(oInt, getCameraDiagnosticStatus() == DIAGNOSTIC_TEXTURE_MAPPING);
			}
			else {
				if(getCameraDiagnosticStatus() == DIAGNOSTIC_FULLBRIGHT) {
					llocal = Color::ColorWith(1,1,1);
				}
				else llocal = calculateLocalLighting(oInt,normal,closestO->objectType);

				//calculate the reflected and refracted rays (if necessary)
				iterations++;
				if(closestO->getReflection() != 0 && iterations < MAXIMUM_ITERATIONS) reflect = calculateReflectedRay(*r,oInt,normal,closestO->objectType);
				if(closestO->getRefraction() != 0 && iterations < MAXIMUM_ITERATIONS) refract = calculateRefractedRay(*r,oInt,normal,closestO->objectType);
				iterations--;

				//If the object has texture mapping, calculate the texture for the point
				if(closestO->hasTexture) { 
					materialTexture = closestO->calculateTextureFromMaterial(oInt, getCameraDiagnosticStatus() == DIAGNOSTIC_TEXTURE_MAPPING);
				}

				//Calculate the percent diffusion of the object
				percentDiffuse = 1.f - closestO->getReflection() - closestO->getRefraction();
				if(percentDiffuse < 0) percentDiffuse = 0;
			}

			if(closestO->hasTexture) {
				if(getCameraDiagnosticStatus() == DIAGNOSTIC_TEXTURE_MAPPING) {
					c = materialTexture * 255;
					return c;
				}
				c = llocal * materialTexture * 255 * percentDiffuse + closestO->getReflection() * reflect + closestO->getRefraction() * refract;
			}
			else {
				c = llocal * closestO->getColor() * 255 * percentDiffuse + closestO->getReflection() * reflect + closestO->getRefraction() * refract;
			}
			lightWasSeen = false;
		}
		else if (closestO->isLight) { //Light is closest
			c = closestO->getColor() * 255;
			lightWasSeen = true;
		}
		else lightWasSeen = false;
	}

	return c;
}

/*
====================
Raytracer::calculateLocalLighting
	Calculates the lighting on the object given the intercept, the object's 
	normal, and the ID of the Entity.  Returns the color of the object
====================
*/
Color Raytracer::calculateLocalLighting(Point intercept, Vector normal, EntityID id) {
	Color llocal = Color(currentCamera->globalIllumination);
	for(unsigned int i = 0; i < objectQueue.size(); i++) {
		SceneObject* l = objectQueue.front();
		objectQueue.pop();
		objectQueue.push(l);
		if(!l->isLight) continue;

		bool lightWasSeen = true;

		//Start of the ray (moved a bit so we won't intercept the object)
		Point lStart = intercept + 0.000001 * (l->origin - intercept);

		std::vector<std::unique_ptr<Ray>> rays = l->raysForLighting(lStart);
		int samples = rays.size();

		for (int i = 0; i < samples; i++) {
			Color receivedColor = raytrace(rays[i].get(),lightWasSeen,true);

			if(lightWasSeen) { //We see the light from the point
				//Direction from the light source to the plane.  Used for calculating lambert
				Vector lToObject = rays[i]->dir;

				lToObject.normalize();

				receivedColor.r /= 255.f * samples;
				receivedColor.g /= 255.f * samples;
				receivedColor.b /= 255.f * samples;

				double lightSourceIntensity = lToObject.cosAngleBetween(normal);
				if(lightSourceIntensity < 0) lightSourceIntensity = 0;
				llocal.r += receivedColor.r * lightSourceIntensity;
				llocal.g += receivedColor.g * lightSourceIntensity;
				llocal.b += receivedColor.b * lightSourceIntensity;
				if(llocal.r > 1) llocal.r = 1;
				if(llocal.g > 1) llocal.g = 1;
				if(llocal.b > 1) llocal.b = 1;
			}
		}

	}
	return llocal;
}

/*
====================
Raytracer::calculateReflectedRay
	Based on the incoming ray r, the intercept, the normal, and the entity ID, 
	calculate the color of the reflected ray (not adjusted for the reflectivity 
	of the object
====================
*/
Color Raytracer::calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	double angle = normal.dot(r.dir);
	Vector reflectVec = -2 * angle * normal + r.dir;

	//Generate reflected ray
	Ray reflectRay;
	reflectRay.dir = reflectVec;
	reflectRay.start = intercept + 0.000001 * reflectVec;

	bool lightWasSeen = false;
	return raytrace(&reflectRay,lightWasSeen,false);
}

/*
====================
Raytracer::calculateRefractedRay
	Based on the incoming ray r, the intercept, the normal, and the entity ID, 
	calculate the color of the refracted ray (not adjusted for the refractivity 
	of the object)
====================
*/
Color Raytracer::calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id) {
	Ray refractRay;
	refractRay.dir = r.dir;
	refractRay.start = intercept + 0.000001 * r.dir;

	bool lightWasSeen = false;
	return raytrace(&refractRay,lightWasSeen,false);
}

/*
====================
Raytracer::findClosestObject
	Iterates through the object queue and locates the closest object
====================
*/
SceneObject *Raytracer::findClosestObject(Ray *r, Point &intersect) {
	SceneObject *closestObject = NULL;
	for(unsigned int i = 0; i < objectQueue.size(); i++) { //look for closest object
		SceneObject* tempO = objectQueue.front();
		objectQueue.pop();
		objectQueue.push(tempO);

		Point objectIntersect;
		if((tempO = tempO->intersect(r, objectIntersect))) {
			if(!closestObject || objectIntersect.comparativeDistanceFrom(r->start) < intersect.comparativeDistanceFrom(r->start)) {
				closestObject = tempO;
				intersect = objectIntersect;
			}
		}
	}

	return closestObject;
}

/*
====================
Raytracer::processInput
	processes the input file to populate the scene
====================
*/
int Raytracer::processInput(string filename) {
	ifstream sceneFile(filename);

	if(!sceneFile.good()) {
		cout << "Not good.\n";
	}
	if(!sceneFile.is_open()) {
		cout << "Scene file failed to open.\n";
		return 0;
	}

	string line; //We will be temporarily storing input here
	int lineNumber = 0;

	while(!sceneFile.eof()) { //Loop through all input
		getline(sceneFile,line);
		lineNumber++;

		queue<string> lineContents;
		explode(line," ",&lineContents);
		
		if(lineContents.size() == 0) continue;
		string word = lineContents.front();
		lineContents.pop();

		if(word[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		if(word == "camera") {
			Camera* c = new Camera(sceneFile);
			cameraQueue.push(c);
		}
		else if(word == "plane") {
			Plane* p = new Plane(sceneFile);
			objectQueue.push(p);
		}
		else if(word == "infplane") {
			InfPlane *ip = new InfPlane(sceneFile);
			objectQueue.push(ip);
		}
		else if(word == "sphere") {
			Sphere* s = new Sphere(sceneFile);
			objectQueue.push(s);
		}
		else if(word == "light") {
			Light* l = new Light(sceneFile);
			objectQueue.push(l);
		}
		else if(word == "disk") {
			Disk* d = new Disk(sceneFile);
			objectQueue.push(d);
		}
		else if(word == "tube") {
			Tube* t = new Tube(sceneFile);
			objectQueue.push(t);
		}
		else if(word == "cylinder") {
			Cylinder* c = new Cylinder(sceneFile);
			objectQueue.push(c);
		}
		else if(word == "cuboid" || word == "box") {
			Cuboid* c = new Cuboid(sceneFile);
			objectQueue.push(c);
		}
		else if(word == "triangle") {
			Triangle* t = new Triangle(sceneFile);
			objectQueue.push(t);
		}
		else {
			cout << "Incorrect syntax on or before line " << lineNumber << ": \n\"" << line << "\"." << endl;
			return 0;
		}
	}
	return 1;
}

/*
====================
Raytracer::addSceneObject
	adds a scene object to the object queue
====================
*/
void Raytracer::addSceneObject(SceneObject *o) {
	objectQueue.push(o);
}

/*
====================
Raytracer::removeAllSceneObjects
	removes all scene objects from the object queue
====================
*/
void Raytracer::removeAllSceneObjects() {
	while(objectQueue.size() > 0) objectQueue.pop();
}

/*
 * Increment number of rays traced on camera, if applicable
 */
void Raytracer::incrementCameraRayCount() {
	if(currentCamera != nullptr) {
		currentCamera->raysTraced++;
	}
}

/*
 * Get diagnostic status if camera exists, normal otherwise
 */
DiagnosticStatus Raytracer::getCameraDiagnosticStatus() const {
	if(currentCamera != nullptr) {
		return currentCamera->diagnosticStatus;
	}
	return DIAGNOSTIC_NORMAL;
}