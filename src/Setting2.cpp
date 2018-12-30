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

void Setting2::setLimits(const int& iteration, const double& distance, const double& funcVal) {
	iterLimit = iteration;
	distLimit = distance;
	funcValLimit = funcVal;
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

void Setting2::setTimeDiffs(const vector<double>& tds) {
	timeDiffs = tds;
	return;
}

void Setting2::printSensorsPosition() {
	int n = sensors.size();
	printf("Sensors number: %d\n", n);
	for (auto pp:sensors) {
		printf("(%f, %f)\n", pp.x, pp.y);
	}
}

MyPoint2D Setting2::locate() {
	p = startPoint;
	// printf("Start at (%f, %f)\n", p.x, p.y);
	// printf("Function value before iteration: %f\n", objectFunc(p));
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
		// printf("Positive Definite Hessian? : %s\n", isPosDef(H_00, H_01, H_11) ? "true" : "false");
		double frac = 1/(H_00*H_11-H_01*H_01);
		p.x -= frac*(H_11*grad_x - H_01*grad_y);
		p.y -= frac*(H_00*grad_y - H_01*grad_x);
		distOfTwoIter = sqrt(p.dist_square(tmp));
		iterId++;
	}
	iterTimes = iterId;
	// printf("Iteration: %d\n", iterTimes);
	// printf("Function value after iteration: %f\n", objectFunc(p));
	// printf("-------------------------------\n");

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
	double lastFuncVal = objectFunc(p);

	// Print some info
	// printf("Start at (%f, %f)\n", p.x, p.y);
	// printf("Function value before iteration: %f\n", lastFuncVal);

	int iterId = 0;
	double distOfTwoIter = 1;
	double diffFuncVal = 1;
	int n = sensors.size();
	double rho = 0;
	while (iterId < iterLimit && distOfTwoIter > distLimit && diffFuncVal > funcValLimit) {
		// Save the iterator's last position
		MyPoint2D tmp = p;
		iterations.push_back(tmp);

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

		// printf("Gradient: (%f, %f)\n", grad_x, grad_y);
		// printf("Hessian: \n[[%f, %f],\n [%f, %f]]\n", H_00, H_01, H_01, H_11);

		// Modify the Hessian
		double eigen1 = ((H_00 + H_11) + sqrt(pow(H_00 + H_11, 2) - 4*(H_00*H_11 - pow(H_01, 2))))/2;
		double eigen2 = ((H_00 + H_11) - sqrt(pow(H_00 + H_11, 2) - 4*(H_00*H_11 - pow(H_01, 2))))/2;
		// cout << "Eigen values: " << eigen1 << ", " << eigen2 << endl;
		if (eigen1 > 0 && eigen2 < 0) {
			H_00 += (-eigen2 + 1.0);
			H_11 += (-eigen2 + 1.0);
		}
		else if (eigen1 < 0 && eigen2 > 0) {
			H_00 += (-eigen1 + 1.0);
			H_11 += (-eigen1 + 1.0);
		}
		else if (eigen1 < 0 && eigen2 < 0) {
			double toAdd = eigen1 < eigen2 ? eigen1 : eigen2;
			H_00 += (-toAdd + 1.0);
			H_11 += (-toAdd + 1.0);
		}

		// Check whether the Hessian is positive definite
		// printf("Modified Hessian: \n[[%f, %f],\n [%f, %f]]\n", H_00, H_01, H_01, H_11);
		// printf("Positive Definite Hessian? : %s\n", isPosDef(H_00, H_01, H_11) ? "true" : "false");

		// Solver of the trust region subproblem, return a step size
		MyPoint2D step = cauchy_point(H_00, H_01, H_11, grad_x, grad_y, radiusTR);
		// printf("Step: [%f, %f]\n", step.x, step.y);

		// Rho is the evaluation coeff of our trust region
		double obj_func_val = objectFunc(p), _obj_func_val = objectFunc(p + step);
		rho = (obj_func_val - _obj_func_val) / (obj_func_val - modelFunc(step, obj_func_val, grad_x, grad_y, H_00, H_01, H_11));
		// cout << "rho: " << rho << endl;

		if (rho >= ita_v) {
			// This means our trust region is SUPER good, update iterator p and expand our trust region
			p = p + step; // I do not overload +=, so just do the old school thing
			radiusTR = min(tr_upper, miu_xpnd*radiusTR); // Do not exceed the upper bound of trust region
			distOfTwoIter = step.norm(); // And update the distance between two iterations
			diffFuncVal = abs(_obj_func_val - lastFuncVal)/lastFuncVal;
			lastFuncVal = _obj_func_val;
		}
		else if (rho >= ita_s) {
			// This means our trust region is good, update iterator p but keep the size of trust region
			p = p + step;
			distOfTwoIter = step.norm();
			diffFuncVal = abs(_obj_func_val - lastFuncVal)/lastFuncVal;
			lastFuncVal = _obj_func_val;
		}
		else {
			// Bad trust region, DO NOT update iterator p and shrink the trust region
			radiusTR = miu_shrk*radiusTR;
		}
		// cout << "Trust Region Radius: " << radiusTR << endl;
		// cout << "-------" << endl;

		// DO NOT forget to increase your iteration ID!!!!!!
		iterId++;
		
	}
	iterTimes = iterId;
	// printf("Iteration: %d\n", iterTimes);
	// printf("Function value after iteration: %f\n", objectFunc(p));
	// printf("-------------------------------\n");

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

bool Setting2::isPosDef(const double& H00, const double& H01, const double& H11) {
	// printf("The det of Hessian: %f\n", H00*H11 - H01*H01);
	return H00 > 0 && H11 > 0 && (H00*H11 - H01*H01) > 0;
}

MyPoint2D Setting2::cg_steihaug(const double& H00, const double& H01, const double& H11, 
	const double& g0, const double& g1, const int& maxit, const double& radiusTR, const double& errtol) {
	/**
	* Conjugate Gradient Trust Region Method
	* double H00, H01, H11: Hessian
	* double g0, g1: gradient
	* int maxit: maximum iteration
	* double radiusTR: trust region radius
	* double errtol: the tolerance of error
	*/

	MyPoint2D pp;
	MyPoint2D r(-g0, -g1);
	if (r.norm() < errtol) 
		return pp;
	MyPoint2D d = r;
	double grad_norm = sqrt(g0*g0 + g1*g1);
	int i = 0;
	double gamma = H00*d.x*d.x + 2*H01*d.x*d.y + H11*d.y*d.y;
	double alpha = 0, beta = 0;

	while (i < maxit) {
		if (gamma <= 0) {
			// Compute tau > 0, s.t. ||pp + tau*d|| = delta
			double tau = sqrt(pow(radiusTR, 2)/(pow(pp.x + d.x, 2) + pow(pp.y + d.y, 2)));
			pp.x = pp.x + tau*d.x;
			pp.y = pp.y + tau*d.y;
			// cout << "1" << endl;
			return pp;
		}
		else {
			alpha = (r.x*r.x + r.y*r.y)/gamma;
			pp.x = pp.x + alpha*d.x;
			pp.y = pp.y + alpha*d.y;
			if (pp.norm() < radiusTR) {
				MyPoint2D tmp = r;
				r.x = r.x - alpha*(H00*d.x + H01*d.y);
				r.y = r.y - alpha*(H01*d.x + H11*d.y);
				if (r.norm()/grad_norm <= errtol) {
					// cout << "2" << endl;
					return pp;
				}
				else {
					beta = (r.x*r.x + r.y*r.y)/(tmp.x*tmp.x + tmp.y*tmp.y);
					d.x = r.x + beta*d.x;
					d.y = r.y + beta*d.y;
					i ++;
				}
			}
			else {
				double tau = sqrt(pow(radiusTR, 2)/(pow(pp.x + d.x, 2) + pow(pp.y + d.y, 2)));
				pp.x = pp.x + tau*d.x;
				pp.y = pp.y + tau*d.y;
				// cout << "tau: " << tau << endl;
				// cout << "3" << endl;
				return pp;
			}
		}
		gamma = H00*d.x*d.x + 2*H01*d.x*d.y + H11*d.y*d.y;
	}
	
}

MyPoint2D Setting2::cauchy_point(const double& h00, const double& h01, const double& h11, 
		const double& gx, const double& gy, const double& radius) {
	double grad_norm = sqrt(gx*gx + gy*gy);
	double param = h00*gx*gx + 2*h01*gx*gy + h11*gy*gy;
	MyPoint2D d = MyPoint2D(-radius*(gx/grad_norm), -radius*(gy/grad_norm));
	if (param <= 0) 
		return d;
	else {
		double tmp = (pow(grad_norm, 3))/(radius*param);
		double t = min((double) 1, tmp);
		return d*t;
	}
}