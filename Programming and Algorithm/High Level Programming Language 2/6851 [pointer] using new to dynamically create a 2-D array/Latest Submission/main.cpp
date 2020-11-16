#include<iostream>
using namespace std;

int main(){
    int m,n;
    cin>>m>>n;
    int** ptr_row = new int*[m];
    for(int i = 0;i < m;i++){
        ptr_row[i] = new int[n];
        for(int j = 0;j < n;j++) cin>>ptr_row[i][j];
    }
    for(int i = 0;i < m;i++){
        for(int j = 0;j < n;j++)
            if(j < n-1) cout<<ptr_row[i][j]<<" ";
            else cout<<ptr_row[i][j]<<endl;
    }
    for(int j = 0;j < m;j++) delete [] ptr_row[j];
    delete [] ptr_row;
    return 0;
}