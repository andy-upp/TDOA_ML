#include <iostream>
#include <cmath>
#include "../include/MyPoint2D.h"
#include "../include/Setting1.h"

using namespace std;

int main() {
	// Test for class MyPoint2D
	MyPoint2D Init;
    MyPoint2D A(5.0f, 4.0f);
    MyPoint2D B(3.0f, 2.0f);
    MyPoint2D C = A - B;
    MyPoint2D D = A + B;
    float a = A.norm();
    float d = A.dist_square(B);
    if (C.x != 2.0f || C.y != 2.0f) cout << "Operator - error" << endl;
    if (D.x != 8.0f || D.y != 6.0f) cout << "Operator + error" << endl;
    if (a != sqrt(41.0f)) cout << "Norm error" << endl;
    if (d != 8.0f) {
    	cout << "d = " << d << endl;
    	cout << "sqrt(8) = " << sqrt(8) << endl;
    	cout << "Distance error" << endl;
    } 
    else {
    	cout << "MyPoint2D clear" << endl;
    }

    cout << (A-B).norm() << endl;
    
    return 0;
}
