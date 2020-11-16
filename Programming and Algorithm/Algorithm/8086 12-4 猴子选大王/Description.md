# 12-4 猴子选大王

## 题目描述
猴子选大王，有N只猴子，从1～N进行编号。它们按照编号的顺时针方向，排成一个圆圈，然后从第一只猴子开始报数。第一只猴子报1，以后每只猴子报的数字都是它前面猴子所报数字加1。如果一只猴子报的数字是M，则该猴子出列，下一只猴子重新从1开始报数。剩下的猴子继续排成一个圆圈报数，直到全部的猴子都出列为止。最后一个出列的猴子胜出。
 

## 输入格式
The first line is an integer t, indicating the number of test cases. Then there are t lines and each line contains two positive integer N(0<N<=100) and M(0<M<=100).

## 输出格式
For each test case, print out the number of the Monkey King.

## 样例输入
2  
5 2  
4 3  

## 样例输出
3  
1  
