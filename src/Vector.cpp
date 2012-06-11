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