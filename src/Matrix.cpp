//#include "Matrix.h"
//#include <math.h>
//
//Matrix Matrix::MatrixRotation(double yaw, double pitch, double roll) {
//	double cosYaw = cos(yaw);
//	double cosPit = cos(pitch);
//	double cosRol = cos(roll);
//	double sinYaw = sin(yaw);
//	double sinPit = sin(pitch);
//	double sinRol = sin(roll);
//	double mat[4][4] = 
//	{{cosYaw * cosPit, cosYaw * sinPit * sinRol - sinYaw * cosRol, cosYaw * sinPit * cosRol + sinYaw * sinRol, 0},
//	 {sinYaw * cosPit, sinYaw * sinPit * sinRol + cosYaw * cosRol, sinYaw * sinPit * cosRol - cosYaw * sinRol, 0},
//	 {-sinPit, cosPit * sinRol, cosPit * cosRol, 0},
//	 {0, 0, 0, 1}};
//}