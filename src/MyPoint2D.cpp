#include <cmath>
#include "../include/MyPoint2D.h"

MyPoint2D::MyPoint2D() {
	x = 0.f;
	y = 0.f;
}

MyPoint2D::MyPoint2D(float _x, float _y) {
	x = _x;
	y = _y;
}

MyPoint2D MyPoint2D::operator +(const MyPoint2D& A) {
	MyPoint2D C;
	C.x = x + A.x;
	C.y = y + A.y;
	return C;
}

MyPoint2D MyPoint2D::operator -(const MyPoint2D& A) {
	MyPoint2D C;
	C.x = x - A.x;
	C.y = y - A.y;
	return C;
}

float MyPoint2D::norm() {
    return sqrt(x*x + y*y);
}

float MyPoint2D::dist_square(MyPoint2D point) {
	float diff_x = x - point.x;
	float diff_y = y - point.y;
    return diff_x*diff_x + diff_y*diff_y;
}
