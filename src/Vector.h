#ifndef _VECTOR_H_
#define _VECTOR_H_

struct Vector {
	double x,y,z;

	Vector();
	Vector(double x, double y, double z);

	Vector operator+(Vector right);
	Vector operator-(Vector right);
};

#endif