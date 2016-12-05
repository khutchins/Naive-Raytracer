#include "SpeedTest.h"
#include <time.h>

/*
====================
generalIntersectionSpeedTest
	Tests speed of intersection with provided object.
	Returns time of execution in seconds
====================
*/
double generalIntersectionSpeedTest(const unsigned int numberOfRaysToCast, SceneObject *o) {
	bool lightWasSeen = false;

	//Generate scene
	Raytracer *raytracer = new Raytracer();
	raytracer->addSceneObject(o);

	clock_t startTime = clock();

	//Test intersections
	for(size_t i = 0; i < numberOfRaysToCast; i++) {
		Ray *r = new Ray(Point(0,0,0),Vector(rand()%100,rand()%100,rand()%100));
		raytracer->raytrace(r,lightWasSeen,false);
		delete r;
	}

	clock_t endTime = clock();
	double seconds = (endTime - startTime) / (double)CLOCKS_PER_SEC;

	//Kill scene
	raytracer->removeAllSceneObjects();
	delete raytracer;

	return seconds;
}

/*
====================
outputObjectSpeedTestResult
	Outputs to console the results of the speed test
====================
*/
void outputObjectSpeedTestResult(const unsigned int numberOfRaysCasted, const double seconds, const string objectName) {
	cout << "Finished testing intersections of " << objectName << " " << numberOfRaysCasted << " times in " << seconds << " seconds.\n";
}

/*
====================
cuboidIntersectionSpeedTest
	Checks intersection speed of cuboid
====================
*/
void cuboidIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	Cuboid *o = new Cuboid(Point(0,0,5),Vector(0,0,1),Vector(0,1,0),2,3,1,Material());
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"cuboid");
	delete o;
}

/*
====================
diskIntersectionSpeedTest
	Checks intersection speed of disk
====================
*/
void diskIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	Disk *o = new Disk(Material(),3,Vector(0,1,0),Vector(0,0,-1),Point(0,0,5));
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"disk");
	delete o;
}

/*
====================
lightIntersectionSpeedTest
	Checks intersection speed of point light
====================
*/
void lightIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	Light *o = new Light(Point(0,0,5),Material());
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"light");
	delete o;
}

/*
====================
planeIntersectionSpeedTest
	Checks intersection speed of plane
====================
*/
void planeIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	Plane *o = new Plane(Material(),3,2,Vector(0,1,0),Vector(0,0,-1),"",Point(0,0,5),false);
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"plane");
	delete o;
}

/*
====================
infPlaneIntersectionSpeedTest
	Checks intersection speed of an infinite plane
====================
*/
void infPlaneIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	InfPlane *o = new InfPlane(Material(),Vector(0,1,0),Vector(0,0,-1),"",Point(0,0,5));
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"infPlane");
	delete o;
}

/*
====================
sphereIntersectionSpeedTest
	Checks intersection speed of sphere
====================
*/
void sphereIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	Sphere *o = new Sphere(Point(0,0,5),Material(),3);
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"sphere");
	delete o;
}

/*
====================
triangleIntersectionSpeedTest
	Checks intersection speed of triangle
====================
*/
void triangleIntersectionSpeedTest(const unsigned int numberOfRaysToCast) {
	Triangle *o = new Triangle(Material(),Point(-1,-1,5),Point(1,-1,5),Point(0,2,5));
	double seconds = generalIntersectionSpeedTest(numberOfRaysToCast,o);
	outputObjectSpeedTestResult(numberOfRaysToCast,seconds,"triangle");
	delete o;
}

/*
====================
objectIntersectionSpeedTest
	Checks intersection speed of all objects
====================
*/
void objectIntersectionSpeedTest() {
	unsigned int numberOfRaysToCast = 1000000;

	cuboidIntersectionSpeedTest(numberOfRaysToCast);
	diskIntersectionSpeedTest(numberOfRaysToCast);
	lightIntersectionSpeedTest(numberOfRaysToCast);
	planeIntersectionSpeedTest(numberOfRaysToCast);
	infPlaneIntersectionSpeedTest(numberOfRaysToCast);
	sphereIntersectionSpeedTest(numberOfRaysToCast);
	triangleIntersectionSpeedTest(numberOfRaysToCast);
}