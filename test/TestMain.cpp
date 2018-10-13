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

    // Test for class Setting1
    MyPoint2D* init = new MyPoint2D(2, 2);
    MyPoint2D* trans = new MyPoint2D(10, 10);
    MyPoint2D tgt(-2, 0);

	Setting1 setup(tgt, 1.0f);
	setup.setTransmiter(trans);
	setup.setInitPoint(init);
	setup.setIterLimit(50);
	if (setup.iter_limit != 50) {
		cout << "iter_limit set error: " << setup.iter_limit << endl;
	}

	setup.setNormLimit(1e-06);

	float d_time1 = setup.timeCompute(A);
	float d_time2 = setup.timeCompute(B);
	float d_time3 = setup.timeCompute(D);

    setup.addReceiver(A, d_time1);
    setup.addReceiver(B, d_time2);
    setup.addReceiver(D, d_time3);

    if (setup.real_tgt.x != -2 || setup.real_tgt.y != 0)
		cout << "Real target set error" << endl;
	if (setup.t.x != 10 || setup.t.y != 10)
		cout << "Transmiter set error" << endl;
	if (setup.p.x != 2 || setup.p.y != 2) {
		cout << "Initial Point set error" << endl;
		cout << setup.p.x << ", " << setup.p.y << endl;
	}
	if (setup.rcvrs[0].x != 5.0f || setup.rcvrs[0].y != 4.0f) {
		cout << "Receiver add error" << endl;
		cout << setup.rcvrs[0].x << ", " << setup.rcvrs[0].y << endl;
	}
	else {
		cout << "Setting1 clear" << endl;
	}

    MyPoint2D localized_p = setup.localize();

	cout << "Localized position is at " << localized_p.x 
	<< ", " << localized_p.y << endl;
	
    return 0;
}
