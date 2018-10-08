#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct MyPoint2d {
    float x, y;
    
    MyPoint2d(): x(0), y(0) {}
    MyPoint2d(float _x, float _y): x(_x), y(_y) {}
    
    MyPoint2d operator+(const MyPoint2d &A) {
        MyPoint2d C;
        C.x = x + A.x;
        C.y = y + A.y;
        return C;
    }
    
    MyPoint2d operator-(const MyPoint2d &A) {
        MyPoint2d C;
        C.x = x - A.x;
        C.y = y - A.y;
        return C;
    }
};

float dist_square(MyPoint2d, MyPoint2d);
float g_func(MyPoint2d, MyPoint2d, MyPoint2d, float, float);
void newton_method_setup1();
int test();

int main() {
    /*
    * "s_t": the coordinate of transmiter S;
    * "s_ri": the coordinate of receiver i;
    * "tgt": the true coordinate of target;
    * "iter": the coordinate of initial point and the iteration results.
    */
    // test();
    
    MyPoint2d s_t(0.0, 0.0), s_r1(4.0, 0.0), s_r2(0.0, 4.0), s_r3(4, 4), tgt(1, 1.6);
    MyPoint2d iter(3, 3);
    
    // "receivers": the list of receivers;
    // "tds": the list of time difference;
    vector<MyPoint2d> receivers;
    vector<float> tds;
    receivers.push_back(s_r1);
    receivers.push_back(s_r2);
    receivers.push_back(s_r3);
    
    float d_time1 = sqrt(dist_square(s_r1, tgt)) 
                    + sqrt(dist_square(s_t, tgt))
                    - sqrt(dist_square(s_t, s_r1));
                    
    float d_time2 = sqrt(dist_square(s_r2, tgt))
                    + sqrt(dist_square(s_t, tgt))
                    - sqrt(dist_square(s_t, s_r2));
                    
    float d_time3 = sqrt(dist_square(s_r3, tgt))
                    + sqrt(dist_square(s_t, tgt))
                    - sqrt(dist_square(s_t, s_r3));
    
    tds.push_back(d_time1);
    tds.push_back(d_time2);
    tds.push_back(d_time3);
    
    // "iteration": limit number of iteration;
    // "speed": signal travel speed;
    int iteration = 10;
    float speed = 1.0;
    
    
    while (iteration > 0) {
        float grad_x = 0.0, grad_y = 0.0;
        float H_00 = 0.0, H_01 = 0.0, H_10 = 0.0, H_11 = 0.0;
        for (int i = 0; i < receivers.size(); ++i) {
            float s2t = sqrt(dist_square(s_t, iter));
            float r2t = sqrt(dist_square(receivers[i], iter));
            float s2r = sqrt(dist_square(s_t, receivers[i]));
            float memo1 = iter.x - s_t.x, memo2 = iter.x - receivers[i].x;
            float memo3 = iter.y - s_t.y, memo4 = iter.y - receivers[i].y;
            grad_x += ((s2t + r2t - tds[i] - s2r) * (memo1/s2t + memo2/r2t));
            grad_y += ((s2t + r2t - tds[i] - s2r) * (memo3/s2t + memo4/r2t));
            H_00 += ((memo1/s2t + memo2/r2t)*(memo1/s2t + memo2/r2t) + 
                        (s2t + r2t - tds[i] - s2r)*(1/s2t + 1/r2t - memo1*memo1/(s2t*s2t*s2t) - memo2*memo2/(r2t*r2t*r2t)));
            H_01 += ((memo3/s2t + memo4/r2t)*(memo1/s2t + memo2/r2t) + 
                        (s2t + r2t - tds[i] - s2r)*(-memo1*memo3/(s2t*s2t*s2t) - memo2*memo4/(r2t*r2t*r2t)));
            H_10 = H_01;
            H_11 += ((memo3/s2t + memo4/r2t)*(memo3/s2t + memo4/r2t) + 
                        (s2t + r2t - tds[i] - s2r)*(1/s2t + 1/r2t - memo3*memo3/(s2t*s2t*s2t) - memo4*memo4/(r2t*r2t*r2t)));
        }
        grad_x *= 2;
        grad_y *= 2;
        H_00 *= 2;
        H_01 *= 2;
        H_10 *= 2;
        H_11 *= 2;
        float frac = 1/(H_00*H_11 - H_10*H_01);
        iter.x -= frac*(H_11*grad_x - H_01*grad_y);
        iter.y -= frac*(H_00*grad_y - H_10*grad_x);
        iteration --;
    }
    
    printf("The real target is at (%f, %f)\n", tgt.x, tgt.y);
    printf("THe searched target is at (%f, %f)\n", iter.x, iter.y);
    
    return 0;
    
}

float dist_square(MyPoint2d A, MyPoint2d B) {
    MyPoint2d diff = A - B;
    return diff.x*diff.x + diff.y*diff.y;
}

float g_function(MyPoint2d s_r, MyPoint2d s_t, MyPoint2d tgt, float td, float speed) {
    float norm_sr_st = sqrt(dist_square(s_r, s_t));
    float norm_sr_tgt = sqrt(dist_square(s_r, tgt));
    float norm_st_tgt = sqrt(dist_square(s_t, tgt));
    return norm_sr_tgt + norm_st_tgt - td*speed - norm_sr_st;
}

void newton_method_setup1() {
    

}

int test() {
    MyPoint2d A(3, 5);
    MyPoint2d B(4, 3);
    A = A - B;
    printf("%f, %f\n", A.x, A.y);
}




