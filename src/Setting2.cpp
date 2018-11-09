#include "../include/Setting2.h"
#include "../include/MyPoint2D.h"
#include <cmath>
#include <iostream>

Setting2::Setting2(vector<MyPoint2D>& arr_sensor, MyPoint2D& rt, double& speed) {
	sensors = arr_sensor;
	realTarget = rt;
	c = speed;
}

void Setting2::setStart(MyPoint2D& sp) {
	startPoint = sp;
}

void Setting2::setLimits(const int& iteration, const double& distance) {
	iterLimit = iteration;
	distLimit = distance;
	return;
}

void Setting2::timeCompute() {
	int n = sensors.size();
	double referenceTime = sqrt(sensors[0].dist_square(realTarget))/c;
	for (int i = 1; i < n; ++i) {
		double t = sqrt(sensors[i].dist_square(realTarget))/c;
		printf("%f\n", t);
		timeDiffs.push_back(referenceTime-t);
	}
	return;
}

void Setting2::printSensorsPosition() {
	int n = sensors.size();
	printf("Sensors number: %d\n", n);
	for (auto pp:sensors) {
		printf("(%f, %f)\n", pp.x, pp.y);
	}

}

MyPoint2D Setting2::localize() {
	p = startPoint;
	printf("Start at (%f, %f)\n", p.x, p.y);
	printf("Function value before iteration: %f\n", objectFunc(p));
	// if (objectFunc(p) < 0.1) return p;
	int iterId = 0;
	double distOfTwoIter = 1;
	int n = sensors.size();
	while (iterId < iterLimit && distOfTwoIter > distLimit) {
		MyPoint2D tmp = p;
		double grad_x = 0.0, grad_y = 0.0;
		double H_00 = 0.0, H_01 = 0.0, H_11 = 0.0;
		double distP_S0 = (p-sensors[0]).norm();
		double xDistP_S0 = p.x - sensors[0].x;
		double yDistP_S0 = p.y - sensors[0].y;
		for (int i = 1; i < n; ++i) {
			double distP_Si = (p-sensors[i]).norm();
			double distTime = timeDiffs[i-1]*c;
			double g = distP_S0 - distP_Si - distTime;
			double xDistP_Si = p.x - sensors[i].x;
			double yDistP_Si = p.y - sensors[i].y;
			double memo1 = xDistP_S0/distP_S0 - xDistP_Si/distP_Si;
			double memo2 = yDistP_S0/distP_S0 - yDistP_Si/distP_Si;
			grad_x += 2*g*memo1;
			grad_y += 2*g*memo2;
			H_00 += 2*(pow(memo1, 2.0)+g*(pow(yDistP_S0, 2.0)/pow(distP_S0, 3.0)-pow(yDistP_Si, 2.0)/pow(distP_Si, 3.0)));
			H_11 += 2*(pow(memo2, 2.0)+g*(pow(xDistP_S0, 2.0)/pow(distP_S0, 3.0)-pow(xDistP_Si, 2.0)/pow(distP_Si, 3.0)));
			H_01 += 2*(memo2*memo1-g*((xDistP_S0*yDistP_S0)/pow(distP_S0, 3.0)-(xDistP_Si*yDistP_Si)/pow(distP_Si, 3.0)));
		}
		// cout << H_00 << ", " << H_01 << ", " << H_11 << endl;
		// cout << grad_x << ", " << grad_y << ", " << endl;
		double frac = 1/(H_00*H_11-H_01*H_01);
		p.x -= frac*(H_11*grad_x - H_01*grad_y);
		p.y -= frac*(H_00*grad_y - H_01*grad_x);
		distOfTwoIter = sqrt(p.dist_square(tmp));
		iterId++;
	}
	iterTimes = iterId;
	printf("Function value after iteration: %f\n", objectFunc(p));

	return p;
}

double Setting2::objectFunc(MyPoint2D& point) {
	double ans = 0;
	int n = sensors.size();
	for (int i = 1; i < n; ++i) {
		double dist1 = (point-sensors[0]).norm();
		double dist2 = (point-sensors[i]).norm();
		double dist3 = timeDiffs[i-1]*c;
		ans += pow(dist1 - dist2 - dist3, 2);
	}
	return ans;
}