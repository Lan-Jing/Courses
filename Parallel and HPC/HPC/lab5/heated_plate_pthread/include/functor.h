#define MM 2000
#define NN 2000

// declaration of caller functions
void initialization(int num_threads);
void compute_mean(int num_threads);
void set_mean(int num_threads);
void copy_temp(int num_threads);
void iteration(int num_threads);
void compute_diff(int num_threads);

// declaration of loop functors.
void* init_loop1(void *args);
void* init_loop2(void *args);
void* compute_mean_loop1(void* args);
void* compute_mean_loop2(void* args);
void* set_mean_loop(void* args);
void* copy_temp_loop(void *args);
void* iteration_loop(void *args);
void* compute_diff_loop(void *args);