#include "Raytracer.h"
#include <string>

using namespace std;

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

	Raytracer *raytracer = new Raytracer();
	int retValue = raytracer->start(fn);
	delete raytracer;

	std::cout << "Press ENTER to continue...";
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

	return retValue;
}