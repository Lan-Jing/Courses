#include<iostream>
using namespace std;
#include<cstring>

class Int{
    public:
        Int(){
            num++;
            data = 0;
            printf("%d objects of Int has been constructed.\n",num);
        }
        Int(int n){
            num++;
            data = n;
            printf("%d objects of Int has been constructed.\n",num);
        }
        ~Int(){
            num--;
        }
        static int isodd(int i){
            return i%2;
        }
        static int num;
    private:
        int data;
};