#include <iostream>
#include <stdio.h>
#include <vector>
#include "include/MyPoint2D.h"

using namespace std;

int test();

int main() {
    /*
    * "s_t": the coordinate of transmiter S;
    * "s_ri": the coordinate of receiver i;
    * "tgt": the true coordinate of target;
    * "iter": the coordinate of initial point and the iteration results.
    */
    
    test();
    
    /*
     * MyPoint2D s_t(0.0, 0.0), s_r1(300.6, 860.7), s_r2(548.2, 102.0), s_r3(59, 67.5), tgt(100, 22.7);
    MyPoint2D iter(105.2, 18.3);

    printf("Real target: (%f, %f)\n", tgt.x, tgt.y);
    printf("Init point: (%f, %f)\n", iter.x, iter.y);
    
    // "receivers": the list of receivers;
    // "tds": the list of time difference;
    vector<MyPoint2D> receivers;
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
    int iteration = 50;
    float speed = 1.0;

    // "norm_limit": if the norm between two iteration results is close, we say convergence.
    float norm_limit = 1e-6;
    float norm_diff = dist_square(iter, tgt);
    
    while (iteration > 0 && norm_diff > norm_limit) {
        MyPoint2D tmp = iter;
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
        norm_diff = dist_square(iter, tmp);
        iteration --;
    }
    
    
    printf("Found target: (%f, %f)\n", iter.x, iter.y);
    printf("Steps: %d\n", 50-iteration);
    printf("Distance Error: %f\n", dist_square(iter, tgt));
    
    
     * 
     * */
    
    return 0;
}

int test() {
    MyPoint2D A(3.f, 5.f);
    MyPoint2D B(4.f, 3.f);
    A = A - B;
    printf("%f, %f\n", A.x, A.y);
}




