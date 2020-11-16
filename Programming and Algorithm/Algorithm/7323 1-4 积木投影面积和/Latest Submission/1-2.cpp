#include <iostream>
using namespace std;

int grid[3][3];

int main(){
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++) cin>>grid[i][j];
	int shade1 = 0,shade2 = 0,shade3 = 0;
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			if(grid[i][j]) shade1++;
	for(int i = 0;i < 3;i++){
		int maxHeight = 0;
		for(int j = 0;j < 3;j++)
			maxHeight = max(maxHeight,grid[i][j]);
		shade2 += maxHeight;
	}
	for(int j = 0;j < 3;j++){
		int maxHeight = 0;
		for(int i = 0;i < 3;i++)
			maxHeight = max(maxHeight,grid[i][j]);
		shade3 += maxHeight;
	}
	cout<<shade1+shade2+shade3;
	return 0;
}