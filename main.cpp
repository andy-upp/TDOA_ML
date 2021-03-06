#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <climits>
#include <vector>
#include <fstream>
#include "include/MyPoint2D.h"
#include "include/Setting1.h"
#include "include/Setting2.h"
#include <cmath>
#define PI 3.14159265
using namespace std;

void func_10_19();
void func_10_26();
void func_11_03(const string& = "line", const bool& = false);

int main() {
	cout << "***********************************" << endl;
	cout << "* Newton Method Based TDOA Solver *" << endl;
	cout << "* Author: Hao Wu                  *" << endl;
	cout << "* Date: 12/27/2018                *" << endl;
	cout << "***********************************" << endl;
	func_11_03("circle", false);
	// func_10_26();
    return 0;
}

void func_11_03(const string& sensorsDistribution, const bool& isTest) {
	MyPoint2D realTarget(-1.0, -1.0);
	vector<MyPoint2D> sensors;
	srand(200);
	if (sensorsDistribution == "circle") {
		double degree = 0.0;
		double radius = 7.0;
		double deltaDegree = 70.0;
		for (int i = 0; i < 4; ++i) {
			double xPolar = radius*cos((degree + deltaDegree*i) * PI / 180.0);
			double yPolar = radius*sin((degree + deltaDegree*i) * PI / 180.0);
			MyPoint2D sensor(xPolar, yPolar);
			sensors.push_back(sensor);
		}
	}
	else if (sensorsDistribution == "line") {
		for (int i = 0; i < 4; ++i) {
			MyPoint2D sensor(2*i+1, 10-2*i);
			sensors.push_back(sensor);
		}
	}
	
	double speed = 340;
	int iterLimit = 300;
	double distLimit = 1e-7;
	double funcLimit = 1e-7;
	Setting2 config(sensors, realTarget, speed);
	config.printSensorsPosition();
	config.setLimits(iterLimit, distLimit, funcLimit);
	config.timeCompute();
	// MyPoint2D start(2.0, 3.0);
	// config.setStart(start);
	// MyPoint2D found = config.locate();
	// printf("%s\n", "----------------------");
	// printf("Found target at (%f, %f) \n", found.x, found.y);
	// printf("Iteration times: %d\n", config.iterTimes);

	if (!isTest) {
		int n_sample = 100;
		int blocks_x = 20, blocks_y = 20;
		double b_width = 40/blocks_x, b_height = 40/blocks_y;
		MyPoint2D start;
		vector<MyPoint2D> conv; // Record starting point of converging to real target
		vector<MyPoint2D> conv_other; // Record convergence point other than real target
		vector<MyPoint2D> unconv_norm; // Record the diverge starting point
		vector<MyPoint2D> unconv_other; // Record the starting point of converging to other point
		for (int i = -10; i < blocks_x-10; ++i) {
			for (int j = -10; j < blocks_y-10; ++j) {
				for (int k = 0; k < n_sample; ++k) {
					start.randomGenerate(i*b_width, i*b_width+b_width, 
						j*b_height, j*b_height+b_height);
					config.setStart(start);
					double rhoLower = 0.25, rhoUpper = 0.75, shrinkRatio = 0.25, expandRatio = 2.0, trSize = 6.0;
					// MyPoint2D found = config.locate();
    				MyPoint2D found = config.locate_TR(rhoLower, rhoUpper, shrinkRatio, expandRatio, trSize);
					// cout << config.iterTimes << endl;
					double error = sqrt(found.dist_square(realTarget))/realTarget.norm();
					if (error > 0.1) {
						if (config.iterTimes < iterLimit) {
							// Converge to other point
							unconv_other.push_back(start);
							conv_other.push_back(found);
						} else {
							// Diverge
							unconv_norm.push_back(start);
						}
					} else {
						// Converge to real target
						conv.push_back(start);
					}
				}
			}
		}
		ofstream conv_record;
		conv_record.open("py/converge.csv");
		for (auto pp:conv) {
			conv_record << pp.x << ", " << pp.y << endl;
		}
		conv_record.close();

		ofstream unconv_norm_record;
		unconv_norm_record.open("py/unconv_norm.csv");
		for (auto pp:unconv_norm) {
			unconv_norm_record << pp.x << ", " << pp.y << endl;
		}
		unconv_norm_record.close();

		ofstream unconv_other_record;
		unconv_other_record.open("py/unconv_other.csv");
		for (auto pp:unconv_other) {
			unconv_other_record << pp.x << ", " << pp.y << endl;
		}
		unconv_other_record.close();

		ofstream convToOtherRecord;
		convToOtherRecord.open("py/convToOtherRecord.csv");
		for (auto pp:conv_other) {
			convToOtherRecord << pp.x << ", " << pp.y << endl;
		}
		convToOtherRecord.close();

		ofstream configRecord;
		configRecord.open("py/config_record.csv");
		configRecord << realTarget.x << ", " << realTarget.y << endl;
		for (auto pp:sensors) {
			configRecord << pp.x << ", " << pp.y << endl;
		}
		configRecord.close();
    }
    else {
    	MyPoint2D start(-7.5, -7.5);
    	// srand(time(0));
    	// start.randomGenerate(0, 1, 8, 9);
    	config.setStart(start);
    	double rhoLower = 0.25, rhoUpper = 0.75, shrinkRatio = 0.25, expandRatio = 2.0, trSize = 5.0;
    	MyPoint2D found = config.locate_TR(rhoLower, rhoUpper, shrinkRatio, expandRatio, trSize);
    	ofstream iterRecord;
    	iterRecord.open("py/iterRecord.csv");
    	for (auto pp : config.iterations) {
    		iterRecord << pp.x << ", " << pp.y << endl;
    	}
    	iterRecord.close();
    	printf("Target locked at (%f, %f)\n", found.x, found.y);
    	printf("Iteration times: %d\n", config.iterTimes);
    }
}

void func_10_19() {
	// 10/19/2018
	// - generate figure of unconvergency
	// - record the final point which are not converge to tgt
	/********************************************************/
	int epoch = 10;
	double noise_level[4] = {1e-15, 1e-3, 1e-2, 0.1};
	ofstream myfile;
	myfile.open("py/nousage.csv");
	int seed = 200;// should be fixed
	// vector<double> unconv_tgt_x, unconv_tgt_y;
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
			    // // cout << "R4: " << R4.x << ", " << R4.y << endl;
			    R5.randomGenerate(0.0f, 10.0f);
			    // // cout << "R5: " << R5.x << ", " << R5.y << endl;
			    R6.randomGenerate(0.0f, 10.0f);
			    // // cout << "R6: " << R6.x << ", " << R6.y << endl;
			    // R7.randomGenerate(0.0f, 10.0f);
			    // // cout << "R7: " << R7.x << ", " << R7.y << endl;
			    // R8.randomGenerate(0.0f, 10.0f);
			    // // cout << "R8: " << R8.x << ", " << R8.y << endl;
			    // R9.randomGenerate(0.0f, 10.0f);
			    // // cout << "R9: " << R9.x << ", " << R9.y << endl;
			    // R10.randomGenerate(0.0f, 10.0f);
			    // // cout << "R10: " << R10.x << ", " << R10.y << endl;
		    
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
			    // double d_time7 = setup1.timeCompute(R7, noise_level[nl_i]);
			    // double d_time8 = setup1.timeCompute(R8, noise_level[nl_i]);
			    // double d_time9 = setup1.timeCompute(R9, noise_level[nl_i]);
			    // double d_time10 = setup1.timeCompute(R10, noise_level[nl_i]);


			    setup1.addReceiver(R1, d_time1);
			    setup1.addReceiver(R2, d_time2);
			    setup1.addReceiver(R3, d_time3);
			    setup1.addReceiver(R4, d_time4);
			    setup1.addReceiver(R5, d_time5);
			    setup1.addReceiver(R6, d_time6);
			    // setup1.addReceiver(R7, d_time7);
			    // setup1.addReceiver(R8, d_time8);
			    // setup1.addReceiver(R9, d_time9);
			    // setup1.addReceiver(R10, d_time10);


			    // cout << "Real target is at (" << tgt.x << ", " << tgt.y << ")" << endl;
			    // cout << "Initial point is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
			    setup1.locate();
			    // cout << "Located target is at (" << setup1.p.x << ", " << setup1.p.y << ")" << endl;
			   	// cout << "Error: " << sqrt(setup1.p.dist_square(tgt)) / tgt.norm() << endl;
			    res[i] = sqrt(setup1.p.dist_square(tgt)) / tgt.norm();

			    if (res[i] > 0.0000008 && noise_level[nl_i] < 1e-7) {
			    	// To be clear, the order of writing to the csv is
			    	// real_tgt x, y
			    	// searched_p x, y
			    	// R1, R2, ..., R10
			    	myfile << tgt.x << ", " << tgt.y << endl;
			    	myfile << init.x << ", " << init.y << endl;
			    	myfile << setup1.p.x << ", " << setup1.p.y << endl;
			    	myfile << R1.x << ", " << R1.y << endl;
			    	myfile << R2.x << ", " << R2.y << endl;
			    	myfile << R3.x << ", " << R3.y << endl;
			    	myfile << R4.x << ", " << R4.y << endl;
			    	myfile << R5.x << ", " << R5.y << endl;
			    	myfile << R6.x << ", " << R6.y << endl;
			    	// myfile << R7.x << ", " << R7.y << endl;
			    	// myfile << R8.x << ", " << R8.y << endl;
			    	// myfile << R9.x << ", " << R9.y << endl;
			    	// myfile << R10.x << ", " << R10.y << endl;
			    }
			    // cout << setup1.p.x << ", " << setup1.p.y << endl;
			    i++;
			}

			// double avg_err = 0.0f;
			// double max_err = 0;
			// int count = 0;
			// for (int k = 0; k < 1000; ++k) {
			// 	// avg_err += res[k];
			// 	// if (res[k] >= max_err) max_err = res[k];
			// 	if (res[k] > 0.0000008) count++;
			// }
			// // cout << avg_err << endl;
			// // cout << max_err << endl;
			// // cout << count << endl;
			// myfile << j+1 << ", " << 10 << ", " << 
			// noise_level[nl_i] << ", " << count << endl;
		}
	}
	myfile.close();
	cout << "draw? [Y/N]" << endl;
	char c = getchar();
	if (c == 'Y' || c == 'y') {
		system("python py/draw.py 6");
	}
	/********************************************************/
}

void func_10_26() {
	/**
	 * 10/26/2018
	 * - Let's fix the positions of FOUR receivers
	 * - Divide the 10x10 area into 25 blocks
	 * - Pick 100 different random starting points in each block
	 * - Record convergency and unconvergency w.r.t starting points
	 * - Use python (best choice is OpenGL) to plot the result.
	 * Track:
	 * seed1: time(0), 35, 200, 268
	 * Start from figrue31, using noise level 1e-05, before are 1e-10
	*/
	// int seed1 = 268;
	// srand(seed1);
	MyPoint2D T(0.0, 0.0);
	MyPoint2D real_tgt(2, 2), r1(3, 8), r2(5.5, 8), r3(8, 8), r4(10.5, 8);
	// real_tgt.randomGenerate(0, 10);
	// srand(time(0));

	int seed2 = 17;
	srand(seed2);
	double speed = 340;
	int iterLimit = 50;
	Setting1 config(real_tgt, speed);
	config.setTransmiter(T);
	config.setNormLimit(1e-03);
	config.setIterLimit(iterLimit);
	double noise = 1e-05;
	double d_time1 = config.timeCompute(r1, noise);
	double d_time2 = config.timeCompute(r2, noise);
	double d_time3 = config.timeCompute(r3, noise);
	double d_time4 = config.timeCompute(r4, noise);

	config.addReceiver(r1, d_time1);
	config.addReceiver(r2, d_time2);
	config.addReceiver(r3, d_time3);
	config.addReceiver(r4, d_time4);
	
	int n_sample = 100;
	int blocks_x = 5, blocks_y = 5;
	double b_width = 10/blocks_x, b_height = 10/blocks_y;
	MyPoint2D start;
	vector<MyPoint2D> conv;
	vector<MyPoint2D> conv_other;
	vector<MyPoint2D> unconv_norm; // Unconverge due to bad norm
	vector<MyPoint2D> unconv_other; // Unconverge due to converging to other point
	for (int i = 0; i < blocks_x; ++i) {
		for (int j = 0; j < blocks_y; ++j) {
			for (int k = 0; k < n_sample; ++k) {
				start.randomGenerate(i*b_width, i*b_width+b_width, 
					j*b_height, j*b_height+b_height);
				config.setInitPoint(start);
				MyPoint2D found = config.locate();
				double error = sqrt(found.dist_square(real_tgt))/real_tgt.norm();
				if (error > 0.001) {
					int n_iter = config.getIterTimes();
					if (n_iter < iterLimit) {
						unconv_other.push_back(start);
						conv_other.push_back(found);
						cout << *(config.getHessian()) << endl;
						cout << *(config.getHessian()+1) << endl;
						cout << *(config.getHessian()+2) << endl;
					} else {
						unconv_norm.push_back(start);

					}
				} else {
					conv.push_back(start);
				}

			}
		}
	}
	ofstream conv_record;
	conv_record.open("py/converge.csv");
	for (auto pp:conv) {
		conv_record << pp.x << ", " << pp.y << endl;
	}
	conv_record.close();

	ofstream unconv_norm_record;
	unconv_norm_record.open("py/unconv_norm.csv");
	for (auto pp:unconv_norm) {
		unconv_norm_record << pp.x << ", " << pp.y << endl;
	}
	unconv_norm_record.close();

	ofstream unconv_other_record;
	unconv_other_record.open("py/unconv_other.csv");
	for (auto pp:unconv_other) {
		unconv_other_record << pp.x << ", " << pp.y << endl;
	}
	unconv_other_record.close();

	ofstream convToOtherRecord;
	convToOtherRecord.open("py/convToOtherRecord.csv");
	for (auto pp:conv_other) {
		convToOtherRecord << pp.x << ", " << pp.y << endl;
	}
	convToOtherRecord.close();

	ofstream configRecord;
	configRecord.open("py/config_record.csv");
	configRecord << real_tgt.x << ", " << real_tgt.y << endl;
	configRecord << r1.x << ", " << r1.y << endl;
	configRecord << r2.x << ", " << r2.y << endl;
	configRecord << r3.x << ", " << r3.y << endl;
	configRecord << r4.x << ", " << r4.y << endl;
	configRecord.close();
}