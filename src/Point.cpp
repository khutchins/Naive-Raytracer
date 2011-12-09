#include "Point.h"

Vector Point::operator+(Point &right)
{
    Vector result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

Vector Point::operator-(Point &right)
{
    Vector result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

Vector Point::operator+(Vector &right)
{
    Vector result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

Vector Point::operator-(Vector &right)
{
    Vector result;
    result.x = this->x - right.x;
	result.y = this->y - right.y;
	result.z = this->z - right.z;
    return result;
}

Point Point::operator *(double k) {
	Point result;
	result.x = this->x * k;
	result.y = this->y * k;
	result.z = this->z * k;
	return result;
}

Point operator * (double k, Point& p) {
	return p*k;
}