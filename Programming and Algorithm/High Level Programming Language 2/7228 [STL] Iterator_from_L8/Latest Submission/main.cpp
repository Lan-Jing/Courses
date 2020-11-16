#include <iostream>
using namespace std;
#include <vector>

int main(){
    vector<int> vec(10);
    vector<int>::iterator iter;
    for(int i = 0; i <= 9; i++){
        vec[i] = i;
    }


    // 用iter将每一个元素加1
    // your code
    for(vector<int>::iterator iter = vec.begin();iter != vec.end();iter++) (*iter) += 1;

    // 在第1个元素前面插入3个100
    // your code
   	for(int i = 0;i < 3;i++) vec.insert(vec.begin()+1,100);


    // 删除最后一个元素
    // your code
   	vec.erase(vec.end()-1);

    // 在最后的位置插入元素200 
    // your code
    vec.insert(vec.end(),200);

    // 利用iter输出vec
    // your code
    for(vector<int>::iterator iter = vec.begin();iter != vec.end();iter++) cout<<(*iter)<<' '; //cout<<endl;


    return 0;
}