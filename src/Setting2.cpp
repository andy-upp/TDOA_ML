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
		printf("Positive Definite Hessian? : %s\n", isPosDef(H_00, H_01, H_11) ? "true" : "false");
		double frac = 1/(H_00*H_11-H_01*H_01);
		p.x -= frac*(H_11*grad_x - H_01*grad_y);
		p.y -= frac*(H_00*grad_y - H_01*grad_x);
		distOfTwoIter = sqrt(p.dist_square(tmp));
		iterId++;
	}
	iterTimes = iterId;
	printf("Iteration: %d\n", iterTimes);
	printf("Function value after iteration: %f\n", objectFunc(p));
	printf("-------------------------------\n");

	return p;
}

MyPoint2D Setting2::locate_TR(const double& ita_s, const double& ita_v, 
	const double& miu_shrk, const double& miu_xpnd, const double& tr_upper) {
	/**
	* double ita_s: the lower bound of rho which is satisfied
	* double ita_v: the lower bound of rho which is super-satisfied
	* double miu_shrk: the ratio we want to shrink our trust region
	* double miu_xpnd: the ratio we want to expand our trust region
	* double tr_upper: the upper bound of the size of our trust region
	*/
	p = startPoint; // Set the iterator p as our starting point
	double radiusTR = 0.5*tr_upper; // Give the inital trust region radius

	// Print some info
	printf("Start at (%f, %f)\n", p.x, p.y);
	printf("Function value before iteration: %f\n", objectFunc(p));

	int iterId = 0;
	double distOfTwoIter = 1;
	int n = sensors.size();
	double rho = 0;
	while (iterId < iterLimit && distOfTwoIter > distLimit) {
		// Save the iterator's last position
		// MyPoint2D tmp = p;

		// Compute the gradient and Hessian
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

		// Check whether the Hessian is positive definite
		printf("Positive Definite Hessian? : %s\n", isPosDef(H_00, H_01, H_11) ? "true" : "false");

		// Solver of the trust region subproblem, return a step size
		MyPoint2D step = cg_steihaug(H_00, H_01, H_11, grad_x, grad_y, p, radiusTR);

		// Rho is the evaluation coeff of our trust region
		double obj_func_val = objectFunc(p), _obj_func_val = objectFunc(p + step);
		rho = (obj_func_val - _obj_func_val) / (obj_func_val - modelFunc(step, obj_func_val, grad_x, grad_y, H_00, H_01, H_11));
		cout << "rho: " << rho << endl;

		if (rho >= ita_v) {
			// This means our trust region is SUPER good, update iterator p and expand our trust region
			p = p + step; // I do not overload +=, so just do the old school thing
			radiusTR = min(tr_upper, miu_xpnd*radiusTR); // Do not exceed the upper bound of trust region
			distOfTwoIter = step.norm(); // And update the distance between two iterations
			iterId++;
		}
		else if (rho >= ita_s) {
			// This means our trust region is good, update iterator p but keep the size of trust region
			p = p + step;
			distOfTwoIter = step.norm();
			iterId++;
		}
		else {
			// Bad trust region, DO NOT update iterator p and shrink the trust region
			radiusTR = miu_shrk*radiusTR;
		}

		// DO NOT forget to increase your iteration ID!!!!!!
		// iterId++;
	}
	iterTimes = iterId;
	printf("Iteration: %d\n", iterTimes);
	printf("Function value after iteration: %f\n", objectFunc(p));
	printf("-------------------------------\n");

	return p; 
}

double Setting2::objectFunc(MyPoint2D point) {
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

double Setting2::modelFunc(MyPoint2D point, const double& obj_func_val, 
	const double& grad_x, const double& grad_y, const double& H00, const double& H01, const double& H11) {
	return obj_func_val + grad_x*point.x + grad_y*point.y + 0.5*(H00*point.x*point.x + 2*H01*point.x*point.y + H11*point.y*point.y);
}

bool Setting2::isPosDef(const int& H00, const int& H01, const int& H11) {
	printf("The det of Hessian: %d\n", H00*H11 - H01*H01);
	return H00 > 0 && H11 > 0 && (H00*H11 - H01*H01) > 0;
}

MyPoint2D Setting2::cg_steihaug(const double& H00, const double& H01, const double& H11, 
	const double& g0, const double& g1, const MyPoint2D& curX, const double& radius) {
	double errtol = 1e-5;
	double d0 = -g0, d1 = -g1;
	MyPoint2D z = curX;
	if (g0*g0 + g1*g1 < errtol) return z;
	int maxit = 50;
	double r = 0;
	for (int i = 0; i < maxit; ++i) {
		double dBd = d0*d0*H00 + 2*d0*d1*H01 + d1*d1*H11;
		if (dBd <= 0) {
			double L = sqrt(z.dist_square(curX));
			r = radius/L;
			return z + MyPoint2D(d0*r, d1*r);
		}
		else {
			double grad_m = g0*d0 + g1*d1;
			r = dummy_gradient_descent(5, grad_m, r);
			MyPoint2D lastRealGradM(g0+2*H00*z.x+2*H01*z.y, g1+2*H01*z.x+2*H11*z.y);
			z = z + MyPoint2D(r*d0, r*d1);
			if (sqrt(z.dist_square(curX)) >= radius) {
				double L = sqrt(z.dist_square(curX));
				r = radius/L;
				return z + MyPoint2D(d0*r, d1*r);
			}
			MyPoint2D realGradM(g0+2*H00*z.x+2*H01*z.y, g1+2*H01*z.x+2*H11*z.y);
			if (realGradM.norm() <= errtol) {
				return z;
			}
			double factor = pow(realGradM.norm(), 2)/pow(lastRealGradM.norm(), 2);
			d0 = -realGradM.x + factor*d0;
			d1 = -realGradM.y + factor*d1;

		}
	}
	return z;
}

double Setting2::dummy_gradient_descent(const int& maxit, const double& grad, const double& start) {
	// double breakdown = 1e-4;
	double rs = start;
	for (int i = 0; i < maxit; ++i) {
		rs -= grad;
	}
	return rs;
}