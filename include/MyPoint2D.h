#ifndef MYPOINT2D_H
#define MYPOINT2D_H

class MyPoint2D {
	public:
	    double x;
	    double y;
	    MyPoint2D();
	    MyPoint2D(double, double);
	    MyPoint2D operator+(const MyPoint2D&);
	    MyPoint2D operator-(const MyPoint2D&);
	    double norm();
	    double dist_square(MyPoint2D);
	    void randomGenerate(const double&, const double&);
	    void randomGenerate(const double&, const double&, const double&, const double&);
};

#endif // MYPOINT2D_H
