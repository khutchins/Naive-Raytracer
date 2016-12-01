#ifndef _INF_PLANE_H_
#define _INF_PLANE_H_

#include "ObjSceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Diagnostic.h"
#include <fstream>
#include <string>
#include <queue>
#include "EasyBMP/EasyBMP.h"

using namespace std;

class InfPlane : public SceneObject {
public:
    Vector normal;
    Vector up;
    Material material;
    BMP texture;

	explicit InfPlane(std::ifstream &f);
	InfPlane(Material m, Vector up, Vector normal, string textureName, Point origin);
	SceneObject* intersect(Ray* r, Point &intersect) override;
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	Color calculateTextureFromMaterial(Point intercept, bool diagnosticEnabled) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

private:
	Vector topLine;
	Vector leftLine;

	void sharedInit(Material m, Vector up, Vector normal, string textureName, Point origin);
};

#endif
