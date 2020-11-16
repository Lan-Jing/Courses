#include <iostream>
using namespace std;
#include <vector>

int main() {                             
    vector<int> ivec(10, 2);             // 创建含10个值为2的元素的vector容器
    vector<int>::iterator iter;          // 声明迭代器对象
    vector<int>::reverse_iterator riter;          // 声明迭代器对象

    // your code
    //用 iter 将第0个元素的值加10
    //用 riter 将第0个元素的值加10
    //用 iter 将最后一个元素的值改为100
    //用 riter 将最后一个元素的值减20
    //用 iter 正序输出所有元素
    //用 riter 反向输出所有元素
    iter = ivec.begin();
    riter = ivec.rend() - 1;
    (*iter) += 10; (*riter) += 10;
    iter = ivec.end() - 1;
    (*iter) = 100;
    riter = ivec.rbegin();
    (*riter) -= 20;
    for(iter = ivec.begin();iter != ivec.end();iter++)
        cout<<(*iter)<<' ';
    cout<<endl;
    for(riter = ivec.rbegin();riter != ivec.rend();riter++)
        cout<<(*riter)<<' ';
    return 0;
}