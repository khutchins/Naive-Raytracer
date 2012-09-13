#include "Point.h"

/*
====================
Point::PointZero()
	returns a point with all of its values set to zero
====================
*/
Point Point::PointZero() {
	Point zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	return zero;
}

/*
====================
Point::Point
	Returns a point with zero coordinates
====================
*/
Point::Point() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

/*
====================
Point::Point
	Returns a point with the following coordinates
====================
*/

Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

//Subtracts a point from a point and returns a vector
Vector Point::operator-(Point right)
{
    Vector result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

//Adds a vector to a point and returns a point
Point Point::operator+(Vector right)
{
    Point result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

//Subtracts a vector from a point and returns a point
Point Point::operator-(Vector right)
{
    Point result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

//Multiplies a point by a double
Point Point::operator *(double k) {
	Point result;
	result.x = this->x * k;
	result.y = this->y * k;
	result.z = this->z * k;
	return result;
}

//Multiplies a double by a point
Point operator * (double k, Point p) {
	return p*k;
}
