#include "Vector.h"

/*
====================
Vector::Vector
	returns the empty vector
====================
*/
Vector::Vector() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

/*
====================
Vector::Vector
	returns a vector with the provided (x,y,z) values
====================
*/
Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/*
====================
Vector::VectorZero
	returns the empty vector
====================
*/
Vector Vector::VectorZero() {
	Vector zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	return zero;
}


/*
====================
Vector::print
	outputs the vector information in a readable format
====================
*/
void Vector::print() {
	printf("{%.2f,%.2f,%.2f}",x,y,z);
}

/*
====================
Vector::normalize
	normalizes the vector, if necessary
====================
*/
void Vector::normalize() {
	double n = sqrt(x*x+y*y+z*z);
	x /= n;
	y /= n;
	z /= n;
}

/*
====================
perpendicularVector
	computes one of the vectors perpendicular to the provided vector.  If the 
	vector provided is the zero vector, the zero vector is returned
====================
*/
Vector Vector::perpendicularVector() {
	if(x == 0 && y == 0 && z == 0) {
		return VectorZero();
	}
	if(x <= y && x <= z) {
		return this->cross(1,0,0);
	}
	else if(y <= x && y <= z) {
		return this->cross(0,1,0);
	}
	else return this->cross(0,0,1);
}

/*
====================
cross
	computes the cross product of self with provided vector values
====================
*/
Vector Vector::cross(const double x2, const double y2, const double z2) {
	Vector v;
	v.x = y*z2 - z*y2;
	v.y = z*x2 - x*z2;
	v.z = x*y2 - y*x2;
	return v;
}

/*
====================
cross
	computes the cross product of self with provided vector
====================
*/
Vector Vector::cross(const Vector other) {
	return this->cross(other.x,other.y,other.z);
}

//Vector with vector addition
Vector Vector::operator+(Vector right)
{
    Vector result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

//Vector with vector subtraction
Vector Vector::operator-(Vector right)
{
    Vector result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

//Vector with point addition (returns point)
Point Vector::operator+(Point right)
{
    Point result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

//Vector with point subtraction (returns point)
Point Vector::operator-(Point right)
{
    Point result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

//Vector with double multiplication
Vector Vector::operator *(double k) {
	Vector result;
	result.x = this->x * k;
	result.y = this->y * k;
	result.z = this->z * k;
	return result;
}

//Double with vector multiplication
Vector operator * (double k, Vector v) {
	return v*k;
}