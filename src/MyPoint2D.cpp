#include <cmath>
#include <random>
#include <ctime>
#include "../include/MyPoint2D.h"

MyPoint2D::MyPoint2D() {
	x = 0.f;
	y = 0.f;
}

MyPoint2D::MyPoint2D(const double _x, const double _y) {
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

MyPoint2D MyPoint2D::operator *(const double& num) {
	MyPoint2D C;
	C.x = num*x;
	C.y = num*y;
	return C;
}

double MyPoint2D::norm() {
    return sqrt(x*x + y*y);
}

double MyPoint2D::dist_square(MyPoint2D point) {
	double diff_x = x - point.x;
	double diff_y = y - point.y;
    return diff_x*diff_x + diff_y*diff_y;
}

void MyPoint2D::randomGenerate(const double& lower, const double& upper) {
	double fractor1 = ((double) rand())/(double) RAND_MAX;
	double fractor2 = ((double) rand())/(double) RAND_MAX;
	double dist = upper - lower;
	x = lower + dist*fractor1;
	y = lower + dist*fractor2;
}

void MyPoint2D::randomGenerate(const double& x_lo, const double& x_up, const double& y_lo, const double& y_up) {
	double fractor1 = ((double) rand())/(double) RAND_MAX;
	double fractor2 = ((double) rand())/(double) RAND_MAX;
	double dist_x = x_up - x_lo;
	double dist_y = y_up - y_lo;
	x = x_lo + dist_x*fractor1;
	y = y_lo + dist_y*fractor2;
}
