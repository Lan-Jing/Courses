# [Classes and Objects]Static Member

## Description
/* 
测试static member
*/
完成类Int，可以加入你觉得需要的member value or function.
```
class Int
{
    int data;
public:
    Int(int n); //将n的值赋给data
};
```
```
使得函数f()输出为
num 1 is odd? 1
num 2 is odd? 0
1 objects of Int has been constructed.
2 objects of Int has been constructed.
3 objects of Int has been constructed.
2 objects of Int has been constructed.
```
```
void f()
{
    int i1=1, i2=2;
    cout << "num " << i1 << " is odd? " << Int::isodd(1) << endl;    
    cout << "num " << i2 << " is odd? " << Int::isodd(2) << endl;    
    Int ii1; 
    { Int ii1(i1); }
    Int ii2(i2);
}
```