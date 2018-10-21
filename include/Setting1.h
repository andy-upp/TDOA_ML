#ifndef SETTING1_H
#define SETTING1_H

#include <vector>
#include <unordered_map>
#include "MyPoint2D.h"

using namespace std;

class Setting1 {
public:
	vector<MyPoint2D> rcvrs; //receivers
	vector<double> timediffs;
	MyPoint2D t; // transmiter
	MyPoint2D p; // iteration point
	MyPoint2D real_tgt; // real position of target
	double c; // signal speed
	int iter_limit; // iteration number limit
	double norm_limit; // judge when to stop the iteration

	Setting1(const MyPoint2D&, const double&);
	void addReceiver(MyPoint2D&, double&);
	void setTransmiter(const MyPoint2D&);
	void setInitPoint(const MyPoint2D&);
	void setIterLimit(const int&);
	void setNormLimit(const double&);
	double timeCompute(MyPoint2D&);
	double timeCompute(MyPoint2D&, const double&);
	MyPoint2D localize();
};

#endif // Finsh SETTING1_H
