#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "ObjSceneObject.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Color.h"
#include <fstream>
#include <string>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

class Light : public SceneObject{
public:
	explicit Light(ifstream &f);
	Light(Point origin, Material material);

	SceneObject* intersect(Ray* r, Point &intersect) override;
	Color getColor() override;

private:
	void sharedInit(Point origin, Material material);
};

#endif