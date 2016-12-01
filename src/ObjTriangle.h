#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "ObjSceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class Triangle : public SceneObject {
public:
    Point vertex1;
	Point vertex2;
	Point vertex3;
    Material material;

	Triangle(std::ifstream &f);
	Triangle(Material m, Point vertex1, Point vertex2, Point vertex3);
	SceneObject* intersect(Ray* r, Point &intersect) override;
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

private:
	Vector normal;

	void sharedInit(Material m, Point vertex1, Point vertex2, Point );
};

#endif
