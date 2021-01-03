void init_rand();
float rand_float(float rbound);
int Max(int a, int b);

void get_padding(int &rpl, int &rpr, int &cpl, int &cpr, 
                 int &om, int &on,
                 int im, int in, int km, int kn, int s);

extern double omp_avgTime, omp_stime;
extern double cuda_avgTime, cuda_stime;
extern double col_avgTime, col_stime;
extern double cudnn_avgTime, cudnn_stime;