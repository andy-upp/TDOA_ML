#include <vector>
#include <iostream>
#include <cmath>
#include "../include/Setting1.h"
#include "../include/MyPoint2D.h"

using namespace std;

Setting1::Setting1(const MyPoint2D& target, const double& speed) {
    real_tgt = target;
    c = speed;
}

void Setting1::addReceiver(MyPoint2D& new_r, double& time_difference) {
	rcvrs.push_back(new_r);
	timediffs.push_back(time_difference);
}

void Setting1::setTransmiter(const MyPoint2D& transmiter) {
	t = transmiter;
}

void Setting1::setInitPoint(const MyPoint2D& init_p) {
	p = init_p;
}

void Setting1::setIterLimit(const int& limit) {
	iter_limit = limit;
}

void Setting1::setNormLimit(const double& limit) {
	norm_limit = limit;
}

double Setting1::timeCompute(MyPoint2D& p1) {
	double dist = sqrt(t.dist_square(real_tgt)) 
					+ sqrt(p1.dist_square(real_tgt))
					- sqrt(t.dist_square(p1));
	return dist/c;
}

double Setting1::timeCompute(MyPoint2D& point, const double& noise) {
	double dist = sqrt(t.dist_square(real_tgt)) 
					+ sqrt(point.dist_square(real_tgt))
					- sqrt(t.dist_square(point));
	dist += (noise*dist);
	return dist/c;
}

MyPoint2D Setting1::localize() {
	double norm_diff = sqrt(p.dist_square(real_tgt));
	int iter_id = iter_limit;
	int n_rcvrs = rcvrs.size();
	
	while (iter_id > 0 && norm_diff > norm_limit) {
		MyPoint2D tmp = p;
		double grad_x = 0.0, grad_y = 0.0;
		double H_00 = 0.0, H_01 = 0.0, H_11 = 0.0;
		for (int i = 0; i < n_rcvrs; ++i) {
			MyPoint2D cur_rcvr = rcvrs[i];
			double cur_timediff = timediffs[i];
			double t2p = sqrt(t.dist_square(p));
			double p2r = sqrt(p.dist_square(cur_rcvr));
			double t2r = sqrt(t.dist_square(cur_rcvr));
			double memo0 = t2p + p2r - cur_timediff*c - t2r;
			double memo1 = p.x - t.x;
			double memo2 = p.x - cur_rcvr.x;
			double memo3 = p.y - t.y;
			double memo4 = p.y - cur_rcvr.y;
			double memo5 = memo1/t2p + memo2/p2r;
			double memo6 = memo3/t2p + memo4/p2r;
			double memo7 = 1/t2p + 1/p2r;
			double memo8 = t2p*t2p*t2p;
			double memo9 = p2r*p2r*p2r;
			
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
		Hessian[0] = H_00;
		Hessian[1] = H_01;
		Hessian[2] = H_11;
		double frac = 1/(H_00*H_11 - H_01*H_01);
		p.x -= frac*(H_11*grad_x - H_01*grad_y);
		p.y -= frac*(H_00*grad_y - H_01*grad_x);
		norm_diff = sqrt(p.dist_square(tmp));
        iter_id --;
	}
	// cout << "Iteration used: " << iter_limit - iter_id << endl;
	// cout << "End up norm between iteration: " << norm_diff << endl;
	iter_times = iter_limit - iter_id;
	return p;
}

int Setting1::getIterTimes() {
	return iter_times;
}

double* Setting1::getHessian() {
	return Hessian;
}