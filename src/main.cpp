#include "Raytracer.h"
#include "SpeedTest.h"
#include <string>

using namespace std;

#define MANUAL_FILENAME 1

int raytrace(string fileName) {
	Raytracer *raytracer = new Raytracer();
	int retValue = raytracer->start(fileName);
	delete raytracer;
	return retValue;
}

void speedTest() {
	objectIntersectionSpeedTest();
}

int main(int argc, char * argv[]) {
	bool runSpeedTests = false;
	string fn = ""; //name of file

	int retValue;
	if(runSpeedTests) { 
		speedTest();
		retValue = 0;
	} else {
		if(argc >= 2) fn = argv[1];
#if MANUAL_FILENAME
		fn = "K:/git/Naive-Raytracer/scenes/sphereplanelight.txt";
#endif

		if (fn.empty()) {
			cout << "Incorrect argument amount. Program terminating.\n";
		}

		retValue = raytrace(fn);
	}

	return retValue;
}