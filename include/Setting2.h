#ifndef SETTING2_H
#define SETTING2_H

#include <vector>
#include "MyPoint2D.h"

using namespace std;

class Setting2 {
public:
	// Vector of MyPoint2D, the sensors
	vector<MyPoint2D> sensors; 

	// Vector of double, the time differences of arrival
	vector<double> timeDiffs;

	vector<MyPoint2D> iterations;
	MyPoint2D realTarget; // Real target position
	MyPoint2D startPoint; // Iteration start point
	MyPoint2D p; // Iteration point
	double c; // signal speed
	double noise; // noise
	int iterTimes; // record the iteration times
	Setting2(); // Constructor, do nothing

	// Constructor initialize sensors, realTarget and c
	Setting2(vector<MyPoint2D>&, MyPoint2D&, double&);

	// Iteration limit
	int iterLimit;

	// The threshold of norm difference between adjecent iterations
	double distLimit;

	// The threshold of objective function value
	double funcValLimit;

	// Set those limits
	void setLimits(const int&, const double&, const double&);

	// Pure Newton Method to find the local minimum
	MyPoint2D locate();

	// Compute the TDOA, no need for real data
	void timeCompute();

	// Set timeDiffs
	void setTimeDiffs(const vector<double>&);

	// Set the startPoint
	void setStart(MyPoint2D&);

	// Objective function
	double objectFunc(const MyPoint2D);

	// The standard quadratic model function for the trust region method
	double modelFunc(const MyPoint2D, const double&, const double&, 
		const double&, const double&, const double&, const double&);

	// Print sensors info
	void printSensorsPosition();

	// Check if the Hessian is positive definite
	bool isPosDef(const double&, const double&, const double&);

	// Conjugate gradient (steihaug) method to solve the trust region sub-problem
	MyPoint2D cg_steihaug(const double&, const double&, const double&, 
		const double&, const double&, const int&, const double&, const double&);

	// Trust region Newton method to find the local minimum
	MyPoint2D locate_TR(const double&, const double&, const double&, 
		const double&, const double&);

	// Cauchy point method to solve the trust region sub-problem
	MyPoint2D cauchy_point(const double&, const double&, const double&, 
		const double&, const double&, const double&);
};

#endif // Ending of setting2
