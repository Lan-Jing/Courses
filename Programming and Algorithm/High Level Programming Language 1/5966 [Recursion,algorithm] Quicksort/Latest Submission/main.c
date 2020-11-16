#include<stdio.h>
#include<stdlib.h>

#define maxn 100010

void merge_sort(int *numbers,int left,int right){
	if(right <= left) return ;
	
	int mid = (left + right)/2;
	
	merge_sort(numbers,left,mid);
	merge_sort(numbers,mid+1,right);
	
	int *temp = (int *)malloc(sizeof(int) * (right-left+2));//注意malloc 返回一个void指针，每次都要强制转换到需要的类型 
	int p1 = left,p2 = mid+1,p = 1;
	
	while(p1 <= mid && p2 <= right){
		if(numbers[p1] < numbers[p2]) temp[p++] = numbers[p1++];
		else temp[p++] = numbers[p2++];
	}
	while(p1 <= mid) temp[p++] = numbers[p1++];
	while(p2 <= right) temp[p++] = numbers[p2++];
	
	for(int i = 1;i < p;i++)
		numbers[i-1+left] = temp[i];
	
	free(temp);
}

int main(){
	int n,numbers[maxn];
	
	scanf("%d",&n);
	for(int i = 1;i <= n;i++) scanf("%d",&numbers[i]);
	
	merge_sort(numbers,1,n);
	
	for(int i = 1;i <= n;i++)
		if(i < n) printf("%d ",numbers[i]);
		else printf("%d\n",numbers[i]);
}