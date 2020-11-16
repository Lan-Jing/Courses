# 【Array】Matrix multiplication

## Description
Given two matrix $A$ and $B$, the program should output $C=AB$. The size of A is (n,m) and the size of B is (m, r).

## Input
The are (1 + n + m) lines

In first line, there are there number $n, m, r$. 1 <= n <= 100, 1 <= m <= 100, 1 <= r <= 100

In the follow n lines, there are $m$ numbers in each line.

In the last m lines, there are $r$ numbers in each line.

### Output
Output $C=AB$
#### Sample Input
```
3 3 3
1 1 1
1 1 1
1 1 1
1 1 1
1 1 1
1 1 1
```
#### Sample Output
```
3 3 3
3 3 3
3 3 3
```

## Hint
[Matrix multiplication](https://en.wikipedia.org/wiki/Matrix_multiplication)


[Strassen algorithm](https://en.wikipedia.org/wiki/Strassen_algorithm)

目前最快的矩阵乘法算法：
[Coppersmith–Winograd algorithm](https://en.wikipedia.org/wiki/Coppersmith%E2%80%93Winograd_algorithm)