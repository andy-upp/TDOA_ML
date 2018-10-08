#ifndef MYPOINT2D_H
#define MYPOINT2D_H

class MyPoint2D {
	public:
	    float x;
	    float y;
	    MyPoint2D();
	    MyPoint2D(float, float);
	    MyPoint2D operator+(const MyPoint2D&);
	    MyPoint2D operator-(const MyPoint2D&);
	    float norm(MyPoint2D);
	    float dist_square(MyPoint2D, MyPoint2D);
};

#endif // MYPOINT2D_H
