#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "../include/helper.hpp"

// Generate a random float number, ranging from 0 to randMax
void init_rand()
{
	srand(time(0));
}

float rand(float randMax)
{
	return (float)((double)rand()/(double)(RAND_MAX/randMax));
}

// Create a timer array, hosting separated counters.
float* timerArr;
int timerArrSize;

void create_timer(int timerCnt) 
{ 
	timerArr = nullptr;
	if(timerCnt <= 0) return ;
	timerArrSize = timerCnt;
	timerArr = (float*)malloc(sizeof(float)*timerCnt);
	clear_timer();
}

void destroy_timer()
{
	if(timerArr == nullptr) return ;
	free(timerArr);
}

void clear_timer(int idx) 
{ 
	if(idx < 0 || idx >= timerArrSize) return ;
	timerArr[idx] = 0x3f3f3f3f;
}

void clear_timer()
{
	for(int i = 0;i < timerArrSize;i++) timerArr[i] = 0x3f3f3f3f;
}

void add_to_timer(int idx, float duration) 
{
	if(idx < 0 || idx >= timerArrSize) return ;
	timerArr[idx] = timerArr[idx] > duration ? duration : timerArr[idx];
}

float check_timer(int idx)
{
	if(idx < 0 || idx >= timerArrSize) return -1;
	return timerArr[idx];
}