#include <iostream>
#include <stdio.h>
#include <random>
#include <ctime>
#include <vector>
#include <fstream>
#include "include/MyPoint2D.h"
#include "include/Setting1.h"
#include <cmath>

using namespace std;

int main() {
	// generate figure of unconvergency
	// record the final point which are not converge to tgt
	int epoch = 10;
	double noise_level[4] = {1e-15, 1e-3, 1e-2, 0.1};
	ofstream myfile;
	myfile.open("../10_receiver.csv");
	int seed = time(0);// should be fixed
	for (int nl_i = 0; nl_i < 4; ++nl_i) {
		for (int j = 0; j < epoch; ++j) {
			int i = 0;
			vector<double> res(1000, 0.0f);
			while (i < 1000) {
				srand(seed);
				seed += 1;
		    	MyPoint2D R1, R2, R3, R4, R5, R6, R7, R8, R9, R10;
		    	MyPoint2D tgt;
		    	tgt.randomGenerate(0.0f, 10.0f);

			    R1.randomGenerate(0.0f, 10.0f);
			    // cout << "R1: " << R1.x << ", " << R1.y << endl;
			    R2.randomGenerate(0.0f, 10.0f);
			    // cout << "R2: " << R2.x << ", " << R2.y << endl;
			    R3.randomGenerate(0.0f, 10.0f);
			    // cout << "R3: " << R3.x << ", " << R3.y << endl;
			    R4.randomGenerate(0.0f, 10.0f);
			    // cout << "R4: " << R4.x << ", " << R4.y << endl;
			    R5.randomGenerate(0.0f, 10.0f);
			    // cout << "R5: " << R5.x << ", " << R5.y << endl;
			    R6.randomGenerate(0.0f, 10.0f);
			    // cout << "R6: " << R6.x << ", " << R6.y << endl;
			    R7.randomGenerate(0.0f, 10.0f);
			    // cout << "R7: " << R7.x << ", " << R7.y << endl;
			    R8.randomGenerate(0.0f, 10.0f);
			    // cout << "R8: " << R8.x << ", " << R8.y << endl;
			    R9.randomGenerate(0.0f, 10.0f);
			    // cout << "R9: " << R9.x << ", " << R9.y << endl;
			    R10.randomGenerate(0.0f, 10.0f);
			    // cout << "R10: " << R10.x << ", " << R10.y << endl;
		    
			    MyPoint2D init;
			    init.randomGenerate(tgt.x-2, tgt.x+2, tgt.y-2, tgt.y+2);
			    MyPoint2D transmiter;
			    double speed = 340;

			    Setting1 setup1(tgt, speed);
			    setup1.setTransmiter(transmiter);
			    setup1.setInitPoint(init);
			    setup1.setIterLimit(50);
			    setup1.setNormLimit(1e-7);

			    double d_time1 = setup1.timeCompute(R1, noise_level[nl_i]);
			    double d_time2 = setup1.timeCompute(R2, noise_level[nl_i]);
			    double d_time3 = setup1.timeCompute(R3, noise_level[nl_i]);
			    double d_time4 = setup1.timeCompute(R4, noise_level[nl_i]);
			    double d_time5 = setup1.timeCompute(R5, noise_level[nl_i]);
			    double d_time6 = setup1.timeCompute(R6, noise_level[nl_i]);
			    double d_time7 = setup1.timeCompute(R7, noise_level[nl_i]);
			    double d_time8 = setup1.timeCompute(R8, noise_level[nl_i]);
			    double d_time9 = setup1.timeCompute(R9, noise_level[nl_i]);
			    double d_time10 = setup1.timeCompute(R10, noise_level[nl_i]);


			    setup1.addReceiver(R1, d_time1);
			    setup1.addReceiver(R2, d_time2);
			    setup1.addReceiver(R3, d_time3);
			    setup1.addReceiver(R4, d_time4);
			    setup1.addReceiver(R5, d_time5);
			    setup1.addReceiver(R6, d_time6);
			    setup1.addReceiver(R7, d_time7);
			    setup1.addReceiver(R8, d_time8);
			    setup1.addReceiver(R9, d_time9);
			    setup1.addReceiver(R10, d_time10);


			    // cout << "Real target is at (" << tgt.x << ", " << tgt.y << ")" << endl;
			    // cout << "Initial point is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
			    setup1.localize();
			    // cout << "Located target is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
			   	// cout << "Error: " << sqrt(setup1.p.dist_square(tgt)) / tgt.norm() << endl;
			    res[i] = sqrt(setup1.p.dist_square(tgt)) / tgt.norm();
			    // cout << setup1.p.x << ", " << setup1.p.y << endl;
			    i++;
			}

			// double avg_err = 0.0f;
			// double max_err = 0;
			int count = 0;
			for (int k = 0; k < 1000; ++k) {
				// avg_err += res[k];
				// if (res[k] >= max_err) max_err = res[k];
				if (res[k] > 0.0000008) count++;
			}
			// cout << avg_err << endl;
			// cout << max_err << endl;
			// cout << count << endl;
			myfile << j+1 << ", " << 10 << ", " << 
			noise_level[nl_i] << ", " << count << endl;
		}
	}
	myfile.close();
	// srand(time(0));
 //    MyPoint2D R1, R2, R3, R4, R5, R6, R7, R8, R9, R10;
 //    MyPoint2D tgt;
 //    tgt.randomGenerate(0.0f, 10.0f);

 //    R1.randomGenerate(0.0f, 10.0f);
 //    cout << "R1: " << R1.x << ", " << R1.y << endl;
 //    R2.randomGenerate(0.0f, 10.0f);
 //    cout << "R2: " << R2.x << ", " << R2.y << endl;
 //    R3.randomGenerate(0.0f, 10.0f);
 //    cout << "R3: " << R3.x << ", " << R3.y << endl;
 //    R4.randomGenerate(0.0f, 10.0f);
 //    cout << "R4: " << R4.x << ", " << R4.y << endl;
 //    R5.randomGenerate(0.0f, 10.0f);
 //    cout << "R5: " << R5.x << ", " << R5.y << endl;
 //    R6.randomGenerate(0.0f, 10.0f);
 //    cout << "R6: " << R6.x << ", " << R6.y << endl;
 //    R7.randomGenerate(0.0f, 10.0f);
 //    cout << "R7: " << R7.x << ", " << R7.y << endl;
 //    R8.randomGenerate(0.0f, 10.0f);
 //    cout << "R8: " << R8.x << ", " << R8.y << endl;
 //    R9.randomGenerate(0.0f, 10.0f);
 //    cout << "R9: " << R9.x << ", " << R9.y << endl;
 //    R10.randomGenerate(0.0f, 10.0f);
 //    cout << "R10: " << R10.x << ", " << R10.y << endl;
    
 //    MyPoint2D* init = new MyPoint2D(1.0, 1.0);
 //    MyPoint2D* transmiter = new MyPoint2D(0.0f, 0.0f);
 //    float speed = 340;

 //    Setting1 setup1(tgt, speed);
 //    setup1.setTransmiter(transmiter);
 //    setup1.setInitPoint(init);
 //    setup1.setIterLimit(50);
 //    setup1.setNormLimit(1e-7);

 //    float d_time1 = setup1.timeCompute(R1, 1e-10);
 //    float d_time2 = setup1.timeCompute(R2, 1e-10);
 //    float d_time3 = setup1.timeCompute(R3, 1e-10);

 //    setup1.addReceiver(R1, d_time1);
 //    setup1.addReceiver(R2, d_time2);
 //    setup1.addReceiver(R3, d_time3);

 //    cout << "Real target is at (" << tgt.x << ", " << tgt.y << ")" << endl;
 //    cout << "Initial point is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
 //    setup1.localize();
 //    cout << "Located target is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
 //    cout << "Error: " << sqrt(setup1.p.dist_square(tgt)) / tgt.norm() << endl;

    return 0;
}