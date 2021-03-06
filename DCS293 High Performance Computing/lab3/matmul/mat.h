
#define dtype double
#define vec_t dtype*

#define rand_rightBound 10

typedef struct mat_t 
{
    int m, n;
    vec_t* mat;
} mat_t;

int Max(int a, int b);
double float_rand(int r);

mat_t* new_mat(int m, int n, int mode);
void free_mat(mat_t* matp);
void print_mat(mat_t* matp);

mat_t* testmul(mat_t *mata, mat_t *matb);
int mat_is_equal(mat_t *mata, mat_t *matb);
