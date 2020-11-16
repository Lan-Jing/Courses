# [STL] Matrix

# 描述
实现一个Matrix类，完成下面方法。
其中：
1. `dotProduct` 是矩阵点乘，`innerProduct`是矩阵乘法
2. `out_of_range`当访问越界时抛出
3. `invalid_argument`当参与矩阵运算的两个矩阵大小不匹配时抛出
4. `print`将矩阵打印出来，每个数后面接一个空格，每列最后一个数后没有空格

```c++
class Matrix {
  std::vector<std::vector<int> > data;
public:
  Matrix() = default;
  Matrix(int row, int col) { resize(row, col); }

  ~Matrix() = default;

  void resize(int nrow, int ncol);

  // (row, col)
  std::pair<int, int> size() const;

  std::vector<int>& operator[](int r) throw(std::out_of_range);
  const std::vector<int>& operator[](int r) const throw(std::out_of_range) ;

  
  Matrix operator+(const Matrix& other) throw(std::invalid_argument);
  Matrix operator-(const Matrix& other) throw(std::invalid_argument);
  // innerProduct
  Matrix operator*(const Matrix& other) throw(std::invalid_argument);

  // dot products
  Matrix dotProduct(const Matrix& other) throw(std::invalid_argument);
  // inner products
  Matrix innerProduct(const Matrix& other) throw(std::invalid_argument);

  /**
   * For example: 3x3 identity matrix
   *   1 0 0
   *   0 1 0
   *   0 0 1
   */
  void print();
};
```

# 样例输入
```
2 8
-3 -14 -42 39 33 8 -20 35 
-15 -50 -48 13 -49 12 -42 -50 
9 10
1 -10 -50 -39 46 -5 -21 -26 -44 42 
-19 -23 20 12 -9 -13 21 11 -34 -2 
20 -24 -43 2 -2 12 -34 -9 -11 -48 
-41 -13 -46 19 -38 1 -20 22 18 22 
-42 32 50 -24 14 10 -2 12 44 46 
-5 7 -41 -22 -1 45 29 -35 -13 -41 
-9 41 -46 -17 -8 -39 -7 -42 -32 -28 
-27 -45 40 -33 -41 36 -15 21 -42 -18 
-28 35 48 36 -47 4 24 -47 46 7 
```

# 样例输出
```
----- Case 1 -----
Matrix A
2 2 2
2 2 2
2 2 2
Matrix B
2 2 1
2 1 2
1 2 2
A+B
4 4 3
4 3 4
3 4 4
A-B
0 0 1
0 1 0
1 0 0
A.dotProduct(B)
4 4 2
4 2 4
2 4 4
A*B
10 10 10
10 10 10
10 10 10
A.innerProduct(B)
10 10 10
10 10 10
10 10 10
----- Case 2 -----
Matrix A
2 2 2
2 2 2
2 2 2
Matrix B: (3, 3)
2 2 2
2 2 2
2 2 2
B.resize(3, 2)
2 2
2 2
2 2
A+B
catch invalid_argument exception
A-B
catch invalid_argument exception
A.dotProduct(B)
catch invalid_argument exception
A*B
12 12
12 12
12 12
A.innerProduct(B)
12 12
12 12
12 12
B[2][1]: 2
B[3][1]: catch out_of_range exception
----- Case 3 -----
Matrix A(0, 0)
Matrix B(0, 0)
A+B
A-B
A.dotProduct(B)
A*B
A.innerProduct(B)
----- Case 4 -----
Matrix A
-3 -14 -42 39 33 8 -20 35
-15 -50 -48 13 -49 12 -42 -50
Matrix B
1 -10 -50 -39 46 -5 -21 -26 -44 42
-19 -23 20 12 -9 -13 21 11 -34 -2
20 -24 -43 2 -2 12 -34 -9 -11 -48
-41 -13 -46 19 -38 1 -20 22 18 22
-42 32 50 -24 14 10 -2 12 44 46
-5 7 -41 -22 -1 45 29 -35 -13 -41
-9 41 -46 -17 -8 -39 -7 -42 -32 -28
-27 -45 40 -33 -41 36 -15 21 -42 -18
-28 35 48 36 -47 4 24 -47 46 7
A+B
catch invalid_argument exception
A-B
catch invalid_argument exception
A.dotProduct(B)
catch invalid_argument exception
A*B
catch invalid_argument exception
A.innerProduct(B)
catch invalid_argument exception
```