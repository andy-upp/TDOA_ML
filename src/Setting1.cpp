#include <vector>
#include <iostream>
#include <cmath>
#include "../include/Setting1.h"
#include "../include/MyPoint2D.h"

using namespace std;

Setting1::Setting1(const MyPoint2D& target, const float& speed) {
    real_tgt = target;
    c = speed;
}

void Setting1::addReceiver(MyPoint2D& new_r, float& time_difference) {
	rcvrs.push_back(new_r);
	timediffs.push_back(time_difference);
}

void Setting1::setTransmiter(MyPoint2D* transmiter) {
	t = *transmiter;
}

void Setting1::setInitPoint(MyPoint2D* init_p) {
	p = *init_p;
}

void Setting1::setIterLimit(const int& limit) {
	iter_limit = limit;
}

void Setting1::setNormLimit(const float& limit) {
	norm_limit = limit;
}

float Setting1::timeCompute(MyPoint2D& p1) {
	float dist = sqrt(t.dist_square(real_tgt)) 
					+ sqrt(p1.dist_square(real_tgt))
					- sqrt(t.dist_square(p1));
	return dist/c;
}

MyPoint2D Setting1::localize() {
	float norm_diff = sqrt(p.dist_square(real_tgt));
	int iter_id = iter_limit;
	int n_rcvrs = rcvrs.size();
	
	while (iter_id > 0 && norm_diff > norm_limit) {
		MyPoint2D tmp = p;
		float grad_x = 0.0, grad_y = 0.0;
		float H_00 = 0.0, H_01 = 0.0, H_11 = 0.0;
		for (int i = 0; i < n_rcvrs; ++i) {
			MyPoint2D cur_rcvr = rcvrs[i];
			float cur_timediff = timediffs[i];
			float t2p = sqrt(t.dist_square(p));
			float p2r = sqrt(p.dist_square(cur_rcvr));
			float t2r = sqrt(t.dist_square(cur_rcvr));
			float memo0 = t2p + p2r - cur_timediff*c - t2r;
			float memo1 = p.x - t.x;
			float memo2 = p.x - cur_rcvr.x;
			float memo3 = p.y - t.y;
			float memo4 = p.y - cur_rcvr.y;
			float memo5 = memo1/t2p + memo2/p2r;
			float memo6 = memo3/t2p + memo4/p2r;
			float memo7 = 1/t2p + 1/p2r;
			float memo8 = t2p*t2p*t2p;
			float memo9 = p2r*p2r*p2r;
			
			grad_x += memo0*memo5;
			grad_y += memo0*memo6;
			H_00 += memo5*memo5 + memo0*(memo7 - 
				memo1*memo1/memo8 - memo2*memo2/memo9);
			H_01 += memo5*memo6 + memo0*(-memo1*memo3/memo8 - 
				memo2*memo4/memo9); 
			H_11 += memo6*memo6 + memo0*(memo7 - 
				memo3*memo3/memo8 - memo4*memo4/memo9);

		}
		grad_x *= 2;
		grad_y *= 2;
		H_00 *= 2;
		H_01 *= 2;
		H_11 *= 2;
		float frac = 1/(H_00*H_11 - H_01*H_01);
		p.x -= frac*(H_11*grad_x - H_01*grad_y);
		p.y -= frac*(H_00*grad_y - H_01*grad_x);
		norm_diff = sqrt(p.dist_square(tmp));
        iter_id --;
	}
	cout << "Iteration used: " << iter_limit - iter_id << endl;
	cout << "End up norm between iteration: " << norm_diff << endl;
	return p;
}