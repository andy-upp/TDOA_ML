#include "../include/Setting1.h"
#include <vector>
#include "../include/MyPoint2D.h"

using namespace std;

Setting1::Setting1(const vector<MyPoint2D>& receivers, const MyPoint2D& target) {
    r = receivers;
    real_tgt = target;
}

void Setting1::addReceiver(MyPoint2D& new_r) {
	r.push_back(new_r);
}

void Setting1::setTransmiter(MyPoint2D& transmiter) {
	t = transmiter;
}

void Setting1::setInitPoint(MyPoint2D& init_p) {
	p = init_p;
}

MyPoint2D Setting1::localize() {
	MyPoint2D A;
	return A;
}