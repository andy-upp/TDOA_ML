#include <iostream>
#include <stdio.h>
#include <vector>
#include "include/MyPoint2D.h"
#include "include/Setting1.h"
#include <cmath>

using namespace std;

int main() {

    MyPoint2D R1(8.45f, 6.27f), R2(4.3f, 9.86f), R3(6.66f, 2.33f);
    MyPoint2D tgt(-3.52f, -8.22f);
    MyPoint2D* init = new MyPoint2D(-5, 5);
    MyPoint2D* transmiter = new MyPoint2D(0.0f, 0.0f);
    float speed = 340;

    Setting1 setup1(tgt, speed);
    setup1.setTransmiter(transmiter);
    setup1.setInitPoint(init);
    setup1.setIterLimit(50);
    setup1.setNormLimit(1e-6);

    float d_time1 = setup1.timeCompute(R1);
    float d_time2 = setup1.timeCompute(R2);
    float d_time3 = setup1.timeCompute(R3);

    setup1.addReceiver(R1, d_time1);
    setup1.addReceiver(R2, d_time2);
    setup1.addReceiver(R3, d_time3);

    cout << "Real target is at (" << tgt.x << ", " << tgt.y << ")" << endl;
    cout << "Initial point is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
    setup1.localize();
    cout << "Located target is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;

    return 0;
}