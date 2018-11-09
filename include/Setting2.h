#ifndef SETTING2_H
#define SETTING2_H

#include <vector>
#include "MyPoint2D.h"

using namespace std;

class Setting2 {
public:
	vector<MyPoint2D> sensors;
	vector<double> timeDiffs;
	MyPoint2D realTarget; // Real target position
	MyPoint2D startPoint; // Iteration start point
	MyPoint2D p; // Iteration point
	double c; // signal speed
	double noise; // noise
	int iterTimes; // record the iteration times
	Setting2();
	Setting2(vector<MyPoint2D>&, MyPoint2D&, double&);
	int iterLimit;
	double distLimit;
	void setLimits(const int&, const double&);
	MyPoint2D localize();
	void timeCompute();
	void setStart(MyPoint2D&);
	double objectFunc(MyPoint2D&);
	void printSensorsPosition();

};

#endif // Ending of setting2
