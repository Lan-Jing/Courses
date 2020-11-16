#include <iostream>
#include "Matrix.hpp"

using namespace std;

int main() {
  {
    cout << "----- Case 1 -----" << endl;
    Matrix A(3,3);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        A[i][j] = 2;
      }
    }

    Matrix B = A;
    for (int i = 0; i < 3; i++) {
      B[2-i][i] = 1;
    }

    cout << "Matrix A" << endl;
    A.print();
    cout << "Matrix B" << endl;
    B.print();

    cout << "A+B" << endl;
    (A+B).print();
    cout << "A-B" << endl;
    (A-B).print();
    cout << "A.dotProduct(B)" << endl;
    (A.dotProduct(B)).print();
    cout << "A*B" << endl;
    (A*B).print();
    cout << "A.innerProduct(B)" << endl;
    (A.innerProduct(B)).print();
  }

  {
    cout << "----- Case 2 -----" << endl;
    Matrix A(3,3);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        A[i][j] = 2;
      }
    }

    Matrix B = A;

    cout << "Matrix A"  << endl;
    A.print();
    cout << "Matrix B: "
         << "(" <<B.size().first << ", " << B.size().second  << ")"
         << endl;
    B.print();
    B.resize(3,2);
    cout << "B.resize"
         << "(" <<B.size().first << ", " << B.size().second  << ")"
         << endl;
    B.print();

    try {
      cout << "A+B" << endl;
      (A+B).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }

    try {
      cout << "A-B" << endl;
      (A-B).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "A.dotProduct(B)" << endl;
      (A.dotProduct(B)).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "A*B" << endl;
      (A*B).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "A.innerProduct(B)" << endl;
      (A.innerProduct(B)).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "B[2][1]: " << B[2][1] << endl;
      cout << "B[3][1]: " ;
      cout.flush();
      cout << B[3][1] << endl;
      cout << "B[1][1]: " << B[1][1] << endl;
    } catch (out_of_range& e) {
      cout << "catch out_of_range exception" << endl;
    }
  }

  {
    cout << "----- Case 3 -----" << endl;
    Matrix A;
    Matrix B;
    cout << "Matrix A"
         << "(" <<A.size().first << ", " << A.size().second  << ")"
         << endl;
    A.print();
    cout << "Matrix B"
         << "(" <<B.size().first << ", " << B.size().second  << ")"
         << endl;
    B.print();

    cout << "A+B" << endl;
    (A+B).print();
    cout << "A-B" << endl;
    (A-B).print();
    cout << "A.dotProduct(B)" << endl;
    (A.dotProduct(B)).print();
    cout << "A*B" << endl;
    (A*B).print();
    cout << "A.innerProduct(B)" << endl;
    (A.innerProduct(B)).print();
  }

  {
    cout << "----- Case 4 -----" << endl;
    int row, col, temp;
    cin >> row >> col;
    Matrix A(row, col);
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        cin >> temp;
        A[i][j] = temp;
      }
    }

    cin >> row >> col;
    Matrix B(row, col);
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        cin >> temp;
        B[i][j] = temp;
      }
    }

    cout << "Matrix A" << endl;
    A.print();
    cout << "Matrix B" << endl;
    B.print();

    try {
      cout << "A+B" << endl;
      (A+B).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }

    try {
      cout << "A-B" << endl;
      (A-B).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "A.dotProduct(B)" << endl;
      (A.dotProduct(B)).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "A*B" << endl;
      (A*B).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
    try {
      cout << "A.innerProduct(B)" << endl;
      (A.innerProduct(B)).print();
    } catch (invalid_argument& e) {
      cout << "catch invalid_argument exception" << endl;
    }
  }

  return 0;
}