#include <time.h>
#include <stdlib.h>
#include "../include/helper.hpp"

void init_rand()
{
    srand(time(0));
}

// construct a float number in [0, rbound]
float rand_float(float rbound) 
{
    return (float)((double)rand()/(double)(RAND_MAX/rbound));
}

int Max(int a, int b)
{
    return a > b ? a : b;
}

// construct left and right padding for both dimensions.
// return size of the result matrix as well.
void get_padding(int &rpl, int &rpr, int &cpl, int &cpr, 
                 int &om, int &on,
                 int im, int in, int km, int kn, int s)
{
    for(int rPadding = 0;rPadding < s;rPadding++) {
        if((im-km+rPadding) % s == 0) {
            rpl = rPadding / 2;
            rpr = rPadding - rpl;
            om = (im-km+rPadding) / s + 1;
            break;
        }
    }
    for(int cPadding = 0;cPadding < s;cPadding++) {
        if((in-kn+cPadding) % s == 0) {
            cpl = cPadding / 2;
            cpr = cPadding - cpl;
            on = (in-kn+cPadding) / s + 1;
            break;
        }
    }
}

double omp_avgTime, omp_stime;
double cuda_avgTime, cuda_stime;
double col_avgTime, col_stime;
double cudnn_avgTime, cudnn_stime;