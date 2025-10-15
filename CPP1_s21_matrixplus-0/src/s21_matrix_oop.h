#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <cmath>
#include <cstring>
#include <iostream>

class S21Matrix {
 public:
  S21Matrix() noexcept;
  explicit S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;

  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(double num);
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  ~S21Matrix() noexcept;

  int GetRows() const noexcept;
  int GetCols() const noexcept;

  void SetRows(int newRows);
  void SetCols(int newCols);
  void MulNumber(double num) noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulMatrix(const S21Matrix& other);

  bool EqMatrix(const S21Matrix& other) const;
  bool operator==(const S21Matrix& other) const;

  double Determinant() const;
  double& operator()(int i, int j);
  double operator()(int i, int j) const;
  S21Matrix Transpose() const;
  S21Matrix InverseMatrix() const;
  S21Matrix CalcComplements() const;
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(double num) const;
  friend S21Matrix operator*(double num, const S21Matrix& matrix);

 private:
  int rows_, cols_;
  double* matrix_;

  void SwapRows(S21Matrix& tmp, int k, int m) const;

  bool CheckSize(const S21Matrix& other) const;

  double Addition(int i, int j) const;
};

#endif  // S21_MATRIX_OOP_H