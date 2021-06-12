#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include "../include/mat.hpp"

void init_rand() 
{
    srand(time(0));
}

char rand_char() 
{
    return (char)(rand() % 16);
}

int divup(int a, int b)
{
    return (a+b-1)/b;
}

timer recorder(10);
timer::timer(int _l)
{
    if(_l <= 0) return ;
    l = _l;
    timeArr = (double*)malloc(sizeof(double)*_l);
    init_time();
}

timer::~timer()
{
    free(timeArr);
}

void timer::init_time()
{
    for(int i = 0;i < l;i++) timeArr[i] = 0x3f3f3f3f;
}

void timer::update_time(int idx, double t)
{
    if(idx < 0 || idx >= l) return ;
    timeArr[idx] = min(timeArr[idx], t);
}

double timer::fetch_time(int idx)
{
    if(idx < 0 || idx >= l) return -1;
    return timeArr[idx];
}

mat::mat(int _m, int _n, int _mode) 
{
    m = _m, n = _n;
    val = (char*)malloc(sizeof(char)*size());
    if(_mode) {
        for(int i = 0;i < size();i++) {
            val[i] = rand_char();
        }
    } else {
        memset(val, 0, sizeof(char)*size());
    }
}

mat::~mat()
{
    free(val);
}

int mat::size() const 
{
    return m * n;
}

void mat::print() const 
{
    for(int i = 0;i < size();i++) {
        printf("%2d ", val[i]);
        if(i % n == n-1)
            printf("\n");
    }
    printf("\n");
}

float* mat::entropy_omp() 
{
    float* res = (float*)malloc(sizeof(float)*size());
    double t = omp_get_wtime();

    int i;
#pragma omp parallel for
    for(i = 0;i < m;i++) {
        for(int j = 0;j < n;j++) {
            int count[16], eleCount = 0;
            memset(count, 0, sizeof(int)*16);

            for(int dy = -2;dy <= 2;dy++) {
                for(int dx = -2;dx <= 2;dx++) {
                    int y = i + dy,
                        x = j + dx;

                    if(y >= 0 && x >= 0 && y < m && x < n) {
                        count[val[y*n+x]]++;
                        eleCount++;
                    }
                }
            }

            float entropy = 0;
            for(int k = 0;k <= 15;k++) {
                double p = (double)count[k]/eleCount;
                entropy -= p*log2(p);
            }
            res[i*n+j] = entropy;
        }
    }

    t = omp_get_wtime() - t;
    recorder.update_time(0, 1000*t);
    return res;
}

__constant__ float cd_logTable[30];
float* mat::entropy_cu(int mode)
{
    float* res = (float*)malloc(sizeof(float)*size());
    float logTable[30], logTable_1[30];
    for(int i = 0;i <= 25;i++) logTable[i]   = log2(i);
    for(int i = 0;i <= 25;i++) logTable_1[i] = i <= 1 ? 0 : i*log2(i);
    
    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);

    char* d_val;
    float* d_entropy, *d_logTable, *d_logTable_1;
    gpuCheck( cudaMalloc(&d_val, sizeof(char)*size()) );
    gpuCheck( cudaMalloc(&d_entropy, sizeof(float)*size()) );
    gpuCheck( cudaMalloc(&d_logTable,   sizeof(float)*30) );
    gpuCheck( cudaMalloc(&d_logTable_1, sizeof(float)*30) );

    gpuCheck( cudaMemcpy(d_val, val, sizeof(char)*size(), cudaMemcpyHostToDevice) );
    gpuCheck( cudaMemcpy(d_logTable,   logTable,   sizeof(float)*26, cudaMemcpyHostToDevice) );
    gpuCheck( cudaMemcpy(d_logTable_1, logTable_1, sizeof(float)*26, cudaMemcpyHostToDevice) );
    gpuCheck( cudaMemcpyToSymbol(cd_logTable, logTable_1, sizeof(float)*26) );

    dim3 gridDim(divup(m, BLOCKSIZE), divup(n, BLOCKSIZE)),
         blockDim(BLOCKSIZE, BLOCKSIZE);

    cudaEventRecord(start);
    switch(mode) {
        case 1: {
            entropy_kernel1<<<gridDim, blockDim>>>(d_entropy, d_val, m, n);
            break;
        }
        case 2: {  
            entropy_kernel2<<<gridDim, blockDim>>>(d_entropy, d_val, d_logTable, m, n);
            break;
        }
        case 3: {
            gridDim = dim3(divup(m, BLOCKSIZE-4), divup(n, BLOCKSIZE-4));
            entropy_kernel3<BLOCKSIZE-4><<<gridDim, blockDim>>>(d_entropy, d_val, m, n);
			break;
		}
        case 4: {
            gridDim = dim3(divup(m, BLOCKSIZE-4), divup(n, BLOCKSIZE-4));
            entropy_kernel4<BLOCKSIZE-4><<<gridDim, blockDim>>>(d_entropy, d_val, m, n);
			break;
		}
		case 5: {
            gridDim = dim3(divup(m, BLOCKSIZE/2), divup(n, BLOCKSIZE/2)),
            blockDim = dim3(BLOCKSIZE/2, BLOCKSIZE/2);
            entropy_kernel5<BLOCKSIZE/2, BLOCKSIZE/2><<<gridDim, blockDim>>>(d_entropy, d_val, d_logTable_1, m, n);
			break;
		}
    }
    cudaEventRecord(end);
    cudaEventSynchronize(end);

    gpuCheck( cudaMemcpy(res, d_entropy, sizeof(float)*size(), cudaMemcpyDeviceToHost) );
    gpuCheck( cudaFree(d_val) );
    gpuCheck( cudaFree(d_entropy) );
    gpuCheck( cudaFree(d_logTable) );
    
    float t = 0;
    cudaEventElapsedTime(&t, start, end);
    recorder.update_time(mode, t);

    cudaEventDestroy(start);
    cudaEventDestroy(end);    
    return res;
}

void print_entropy(float* entropy, int m, int n)
{
    for(int i = 0;i < m;i++) {
        for(int j = 0;j < n;j++) {
            printf("%6lf ", entropy[i*n+j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool check_entropy(float* entropy_a, float* entropy_b, int size)
{
    for(int i = 0;i < size*size;i++) {
        if(abs(entropy_a[i]-entropy_b[i]) > 1e-5) {
            printf("Check failed at (%d, %d): %lf, %lf\n", i/size, i%size, 
                                                           entropy_a[i], entropy_b[i]);
			return false;
        }
    }
	return true;
}

__global__ 
void entropy_kernel1(float* d_entropy, char* d_val, 
                     int m, int n)
{
    const int x = threadIdx.x + blockIdx.x * blockDim.x,
              y = threadIdx.y + blockIdx.y * blockDim.y;

    int count[16], eleCount = 0;
    memset(count, 0, sizeof(int)*16);
    for(int dx = -2;dx <= 2;dx++) {
        for(int dy = -2;dy <= 2;dy++) {
            int xx = x + dx,
                yy = y + dy;
            
            if(xx >= 0 && yy >= 0 && yy < m && xx < n) {
                count[d_val[yy*n+xx]]++;
                eleCount++;
            }
        }
    }
    eleCount = max(1, eleCount);

    float entropy = 0;
    for(int k = 0;k <= 15;k++) {
        float p = (float)count[k]/eleCount;
        entropy -= p*log2(p);
    }
    if(y < m && x < n) {
        d_entropy[y*n+x] = entropy;
    }
}

__global__ 
void entropy_kernel2(float* d_entropy, char* d_val, float* d_logTable,
                     int m, int n)
{
    const int x = threadIdx.x + blockIdx.x * blockDim.x,
              y = threadIdx.y + blockIdx.y * blockDim.y;

    char count[16], eleCount = 0;
    memset(count, 0, sizeof(char)*16);
    for(int dx = -2;dx <= 2;dx++) {
        for(int dy = -2;dy <= 2;dy++) {
            int xx = x + dx,
                yy = y + dy;
            
            if(xx >= 0 && yy >= 0 && yy < m && xx < n) {
                count[d_val[yy*n+xx]]++;
                eleCount++;
            }
        }
    }
    eleCount = max(1, eleCount);

    float entropy = 0;
    for(int k = 0;k <= 15;k++) {
        float p = (float)count[k]/eleCount;
        entropy -= p*(__ldg(&d_logTable[count[k]])-__ldg(&d_logTable[eleCount]));
	}
    if(y < m && x < n) {
        d_entropy[y*n+x] = entropy;
    }
}

template<int bsize>
__global__ void entropy_kernel3(float* d_entropy, char* d_val,
                                int m, int n)
{
    __shared__ int sd_val[32][32];
    const int x = threadIdx.x-2 + blockIdx.x * bsize,
              y = threadIdx.y-2 + blockIdx.y * bsize;
    if(x >= 0 && y >= 0 && y < m && x < n) {
        sd_val[threadIdx.y][threadIdx.x] = d_val[y*n+x];
    } else {
        sd_val[threadIdx.y][threadIdx.x] = 16;
    }
    __syncthreads();

    char count[17], eleCount = 0;
    memset(count, 0, sizeof(char)*17);

    if(threadIdx.x >= 2 && threadIdx.x <= 29 && threadIdx.y >= 2 && threadIdx.y <= 29) {
        for(int dx = -2;dx <= 2;dx++) {
            for(int dy = -2;dy <= 2;dy++) {
                char nowVal = sd_val[threadIdx.y+dy][threadIdx.x+dx];
                count[nowVal]++;
                eleCount += min(1, 16-nowVal);
            }
        }
    }

    float entropy = 0;
    for(int k = 0;k <= 15;k++) {
        float p = (float)count[k]/eleCount;
        entropy -= p*log2(p);
   }

    if(threadIdx.x >= 2 && threadIdx.x <= 29 && threadIdx.y >= 2 && threadIdx.y <= 29 && 
       y < m && x < n) {
        d_entropy[y*n+x] = entropy;
    }
}

template<int bsize>
__global__ void entropy_kernel4(float* d_entropy, char* d_val,
                                int m, int n)
{
    __shared__ int sd_val[32][32];
    const int x = threadIdx.x-2 + blockIdx.x * bsize,
              y = threadIdx.y-2 + blockIdx.y * bsize;
    if(x >= 0 && y >= 0 && y < m && x < n) {
        sd_val[threadIdx.y][threadIdx.x] = d_val[y*n+x];
    } else {
        sd_val[threadIdx.y][threadIdx.x] = 16;
    }

    char count[17], eleCount = 0;
    memset(count, 0, sizeof(char)*17);

    __syncthreads();
    if(threadIdx.x >= 2 && threadIdx.x <= 29 && threadIdx.y >= 2 && threadIdx.y <= 29) {
        for(int dx = -2;dx <= 2;dx++) {
            for(int dy = -2;dy <= 2;dy++) {
                char nowVal = sd_val[threadIdx.y+dy][threadIdx.x+dx];
                switch(nowVal) {
                    case 0:  count[0]++;  break;
                    case 1:  count[1]++;  break;
                    case 2:  count[2]++;  break;
                    case 3:  count[3]++;  break;
                    case 4:  count[4]++;  break;
                    case 5:  count[5]++;  break;
                    case 6:  count[6]++;  break;
                    case 7:  count[7]++;  break;
                    case 8:  count[8]++;  break;
                    case 9:  count[9]++;  break;
                    case 10: count[10]++; break;
                    case 11: count[11]++; break;
                    case 12: count[12]++; break;
                    case 13: count[13]++; break;
                    case 14: count[14]++; break;
                    case 15: count[15]++; break;
                    case 16: count[16]++; break;
                }
                eleCount += min(1, 16-nowVal);
            }
        }
    }

    float entropy = 0;
    for(int k = 0;k <= 15;k++) {
        float p = (float)count[k]/eleCount;
        entropy -= p*log2(p);
   }

    if(threadIdx.x >= 2 && threadIdx.x <= 29 && threadIdx.y >= 2 && threadIdx.y <= 29 && 
       y < m && x < n) {
        d_entropy[y*n+x] = entropy;
    }
}

template<int bsize_x, int bsize_y>
__global__ void entropy_kernel5(float* d_entropy, char* d_val, float* d_logTable,
                    			int m, int n)
{
    const int x = threadIdx.x + blockIdx.x * blockDim.x,
              y = threadIdx.y + blockIdx.y * blockDim.y,
			  idx = threadIdx.y*bsize_x + threadIdx.x;

    char eleCount = 0;
	// __shared__ char sd_count[16][bsize*bsize];
	// for(int i = 0;i <= 15;i++) sd_count[i][((threadIdx.y>>2)<<7)+(threadIdx.x<<2)+(threadIdx.y&3)] = 0;
    __shared__ int sd_count[16][bsize_y*bsize_x];
	for(int i = 0;i <= 15;i++) sd_count[i][idx] = 0;
    for(int dy = -2;dy <= 2;dy++) {
        for(int dx = -2;dx <= 2;dx++) {
            int xx = x + dx,
                yy = y + dy;
            
            if(xx >= 0 && yy >= 0 && yy < m && xx < n) {
				sd_count[d_val[yy*n+xx]][idx]++;
                eleCount++;
            }
        }
    }

    float entropy = 0;
    for(int k = 0;k <= 15;k++) {
		// float p = (float)sd_count[k][idx] / eleCount;
		// float p = (float)sd_count[k][idx] * (1.0 / eleCount);
        // entropy -= p*log2(p);
        // entropy -= p*(__ldg(&d_logTable[sd_count[k][idx]])-__ldg(&d_logTable[eleCount]));
		// entropy -= cd_logTable[sd_count[k][idx]];
        entropy -= __ldg(&d_logTable[sd_count[k][idx]]);
    }
    entropy = entropy / eleCount + log2(eleCount);
    if(y < m && x < n) {
        d_entropy[y*n+x] = entropy;
    }
}