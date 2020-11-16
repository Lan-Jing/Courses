#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <map>
#include <stdexcept>
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


#endif