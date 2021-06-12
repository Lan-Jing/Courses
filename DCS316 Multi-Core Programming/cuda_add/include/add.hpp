#define MAT_ENTRY_T float
#define BLOCKSIZE 32
#define WORKLOAD 2

typedef enum add_type {
    cpu_add,
    gpu_add_1D,
    gpu_add_2D
} add_type ;

class matrix
{
public:
    int height, width;

    matrix();
    matrix(int _height, int _width, int _random=1);
    matrix(const matrix& other);
    ~matrix();
    
    matrix& operator = (const matrix& other);
    matrix& operator += (const matrix& other);
    bool operator == (const matrix& other) const ;
    MAT_ENTRY_T& operator () (const int& y, const int& x) const ;
    int size() const ;
    void print() const ;

    MAT_ENTRY_T* ents;
};

void matrix_add(      matrix& c,
                const matrix& a,
                const matrix& b,
                add_type mode = cpu_add);

template<int workload>
__global__ void add_kernel_1D(MAT_ENTRY_T* d_C, 
                              MAT_ENTRY_T* d_A, 
                              MAT_ENTRY_T* d_B, 
                              const int height, const int width);
template<int workload>
__global__ void add_kernel_2D(MAT_ENTRY_T* d_C,
							  MAT_ENTRY_T* d_A,
							  MAT_ENTRY_T* d_B,
							  const int height, const int width);