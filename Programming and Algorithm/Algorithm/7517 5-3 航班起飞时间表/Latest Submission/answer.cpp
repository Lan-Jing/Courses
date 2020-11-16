#include <iostream>
using namespace std;
#include <algorithm>
#include <queue>

struct node{
	int originalT,cost;
	bool operator < (const node &other) const {
		if(other.cost == this->cost)
			return this->originalT < other.originalT;
		else return this->cost > other.cost;
	}
}list[30010]; int n,k;
long long int totCost, scheme[60010];

int main(){
	cin>>n>>k;
	for(int i = 0;i < n;i++){
		cin>>list[i].cost; list[i].originalT = i+1;
	}
	sort(list,list + n);
//	for(int i = 0;i < n;i++) cout<<list[i].cost<<' '<<list[i].originalT<<endl;
	for(int i = 0;i < n;i++){
		int j = list[i].originalT;
		while(j <= k || (scheme[j] && j <= k+n)) j++;
		scheme[j] = i + 1;
		
		totCost += list[i].cost * (j - list[i].originalT);
//		for(int g = k + 1;g <= k + n;g++) cout<<scheme[g]<<' '; cout<<endl;
//		cout<<totCost<<endl;
	}
	cout<<totCost;
	return 0;
}