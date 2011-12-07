#ifndef _POINT_H_
#define _POINT_H_

typedef struct _Point {
    double x,y,z;
	_Point operator+(_Point right);
	_Point operator-(_Point right);
} Point;

#endif