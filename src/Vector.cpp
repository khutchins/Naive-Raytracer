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

void Vector::print() {
	printf("{%.2f,%.2f,%.2f}",x,y,z);
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

Point Vector::operator+(Point right)
{
    Point result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

Point Vector::operator-(Point right)
{
    Point result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

Vector Vector::operator *(double k) {
	Vector result;
	result.x = this->x * k;
	result.y = this->y * k;
	result.z = this->z * k;
	return result;
}

Vector operator * (double k, Vector v) {
	return v*k;
}

Vector Vector::VectorZero() {
	Vector zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	return zero;
}
