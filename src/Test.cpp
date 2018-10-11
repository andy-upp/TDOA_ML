#include "../include/Test.h"
#include "../include/MyPoint2D.h"
#include "../include/Setting1.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

Test::Test() {}

string Test::test_MyPoint2D() {
	MyPoint2D Init;
    MyPoint2D A(5.0f, 4.0f);
    MyPoint2D B(3.0f, 2.0f);
    MyPoint2D C = A - B;
    MyPoint2D D = A + B;
    float a = A.norm();
    float d = A.dist_square(B);
    if (C.x != 2.0f || C.y != 2.0f) return "Operator - error";
    if (D.x != 8.0f || D.y != 6.0f) return "Operator + error";
    if (a != sqrt(41.0f)) return "Norm error";
    if (d != 8.0f) {
    	cout << "d = " << d << endl;
    	cout << "sqrt(8) = " << sqrt(8) << endl;
    	return "Distance error";
    } 
    return "MyPoint2D pass!";
}

string Test::test_Setting1() {
	MyPoint2D init(2, 2), A(3, 4), B(5, 1), tgt(-2, 0), trans(10, 10);
	vector<MyPoint2D> receivers;
	receivers.push_back(A);
	receivers.push_back(B);

	Setting1 setup(receivers, tgt);
	setup.setTransmiter(trans);
	setup.setInitPoint(init);
	if (setup.real_tgt.x != -2 || setup.real_tgt.y != 0)
		return "Real target set error";
	if (setup.t.x != 10 || setup.t.x != 10)
		return "Transmiter set error";
	if (setup.p.x != 2 || setup.p.y != 2)
		return "Initial Point set error";
	if (setup.r[0].x != 3)
		return "Receivers set error";
	return "Setting1 pass.";
}
