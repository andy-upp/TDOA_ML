#ifndef SETTING1_H
#define SETTING1_H

#include <vector>
#include "MyPoint2D.h"

class Setting1 {
private:
	vector<MyPoint2D> r; //receivers
	MyPoint2D t; // transmiter
	MyPoint2D p; // iteration point
	MyPoint2D real_tgt; // real position of target
public:
	Setting1(vector<MyPoint2D>, MyPoint2D);
	void addReceiver(MyPoint2D);
	void setTransmiter(MyPoint2D);
	void setInitPoint(MyPoint2D);
	MyPoint2D localize();
};

#endif // Finsh SETTING1_H
