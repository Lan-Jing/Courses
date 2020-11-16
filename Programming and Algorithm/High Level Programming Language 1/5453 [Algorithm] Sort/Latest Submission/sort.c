#include<stdio.h>
#include<string.h>

int n,a[100010];
void merge_sort(int l,int r){
	if(r == l) return ;
	int mid = (l+r)/2;
	merge_sort(l,mid); merge_sort(mid+1,r);
	
	int p1=l,p2=mid+1,p3=l,temp[100010];
	memset(temp,0,sizeof(temp));
	
	while(p1 <= mid && p2 <= r){
		if(a[p1] < a[p2])
			temp[p3++] = a[p1++];
		else 
			temp[p3++] = a[p2++];
	}
	while(p1 <= mid) temp[p3++] = a[p1++];
	while(p2 <= r) temp[p3++] = a[p2++];
	for(p3 = l;p3 <= r;p3++)
		a[p3] = temp[p3];
}

int main(){
	scanf("%d",&n);
	int i;
	for(i = 1;i <= n;i++) scanf("%d",&a[i]);
	merge_sort(1,n);
	for(i = 1;i <= n;i++){
		if(i < n) printf("%d ",a[i]);
		else printf("%d",a[i]);
	}
	printf("\n");
	return 0;
}