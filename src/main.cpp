#include "Raytracer.h"
#include "SpeedTest.h"
#include <string>

using namespace std;

int raytrace(string fileName) {
	Raytracer *raytracer = new Raytracer();
	int retValue = raytracer->start(fileName);
	delete raytracer;
	return retValue;
}

void speedTest() {
	objectIntersectionSpeedTest();
}

int main(int argc, char * argv[])
{
	bool manualFilename = false; //If true, use a predefined scene file
	bool runSpeedTests = false;
	string fn; //name of file
	if(manualFilename) fn = "K:\\git\\Naive-Raytracer\\scenes";

	if(argc != 2 && !manualFilename) cout << "Incorrect argument amount. Program terminating.\n";
	else if(argc == 2 && !manualFilename) fn = argv[1];

	int retValue;
	if(runSpeedTests) { 
		speedTest();
		retValue = 0;
	}
	else retValue = raytrace(fn);

	std::cout << "Press ENTER to continue...";
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

	return retValue;
}