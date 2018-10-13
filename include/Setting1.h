#ifndef SETTING1_H
#define SETTING1_H

#include <vector>
#include <unordered_map>
#include "MyPoint2D.h"

using namespace std;

class Setting1 {
public:
	vector<MyPoint2D> rcvrs; //receivers
	vector<float> timediffs;
	MyPoint2D t; // transmiter
	MyPoint2D p; // iteration point
	MyPoint2D real_tgt; // real position of target
	float c; // signal speed
	int iter_limit; // iteration number limit
	float norm_limit; // judge when to stop the iteration

	Setting1(const MyPoint2D&, const float&);
	void addReceiver(MyPoint2D&, float&);
	void setTransmiter(MyPoint2D*);
	void setInitPoint(MyPoint2D*);
	void setIterLimit(const int&);
	void setNormLimit(const float&);
	float timeCompute(MyPoint2D&);
	MyPoint2D localize();
};

#endif // Finsh SETTING1_H
