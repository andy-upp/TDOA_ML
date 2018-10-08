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

float MyPoint2D::norm(MyPoint2D A) {
    return sqrt(A.x*A.x + A.y*A.y);
}

float MyPoint2D::dist_square(MyPoint2D A, MyPoint2D B) {
	MyPoint2D diff = A - B;
    return diff.x*diff.x + diff.y*diff.y;
}
