#include "Vector.h"

Vector::Vector() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector Vector::operator+(Vector right)
{
    Vector result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

Vector Vector::operator-(Vector right)
{
    Vector result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}