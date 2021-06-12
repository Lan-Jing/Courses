#define EPS 1e-4

// generate random numbers
void init_rand();
float rand(float randMax);

// a timer array
extern float* timerArr;
extern int timerArrSize;
void create_timer(int timerCnt);
void destroy_timer();
void clear_timer(int idx);
void clear_timer();
void add_to_timer(int idx, float duration);
float check_timer(int idx);