#include "Point.h"

Point Point::PointZero() {
	Point zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	return zero;
}

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

Point Point::operator+(Vector &right)
{
    Point result;
    result.x = this->x + right.x;
	result.y = this->y + right.y;
	result.z = this->z + right.z;
    return result;
}

Point Point::operator-(Vector &right)
{
    Point result;
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