#include "s21_matrix_oop.h"

/*
Contstrustors and destructor
*/
//===============================================================================================
// default constructor
S21Matrix::S21Matrix() noexcept : rows_(0), cols_(0), matrix_(nullptr) {}

// constructor with parameters
S21Matrix::S21Matrix(int rows, int cols)
    : rows_(rows), cols_(cols), matrix_(nullptr) {
  if (rows_ < 0 || cols < 0) {
    rows_ = cols_ = 0;
    throw std::length_error("Mitrix size must be greather or equal zero");
  }
  matrix_ = new double[rows_ * cols_]();
}

// copy constructor
S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_),
      cols_(other.cols_),
      matrix_(new double[rows_ * cols_]) {
  std::copy(other.matrix_, other.matrix_ + rows_ * cols_, matrix_);
}

// move constructor
S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

// destructor
S21Matrix::~S21Matrix() { delete[] matrix_; }

//===============================================================================================
/*
Accessors and mutators

*/
int S21Matrix::GetRows() const noexcept { return rows_; }

int S21Matrix::GetCols() const noexcept { return cols_; }

void S21Matrix::SetRows(int newRows) {
  if (newRows < 0) {
    throw std::length_error("Mitrix size must be greather or equal zero");
  }
  S21Matrix tmp(newRows, cols_);
  int min = std::min(rows_, newRows);
  for (int i = 0; i < min; i++) {
    for (int j = 0; j < cols_; j++) {
      tmp.matrix_[i * cols_ + j] = matrix_[i * cols_ + j];
    }
  }
  std::swap(tmp.matrix_, matrix_);
  std::swap(tmp.rows_, rows_);
  std::swap(tmp.cols_, cols_);
}

void S21Matrix::SetCols(const int newCols) {
  if (newCols < 0) {
    throw std::length_error("Mitrix size must be greather or equal zero");
  }
  S21Matrix tmp(rows_, newCols);
  int min = std::min(cols_, newCols);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < min; j++) {
      tmp.matrix_[i * cols_ + j] = matrix_[i * cols_ + j];
    }
  }
  std::swap(tmp.matrix_, matrix_);
  std::swap(tmp.rows_, rows_);
  std::swap(tmp.cols_, cols_);
}

//================================================================================================
/*
Task functions

*/
bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  if (!CheckSize(other)) {
    return false;
  }
  for (int i = 0; i < rows_ * cols_; i++) {
    if (std::abs(matrix_[i] - other.matrix_[i]) > 1e-7) return false;
  }
  return true;
}

void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_ * cols_; i++) {
    matrix_[i] *= num;
  }
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (!CheckSize(other) && other.matrix_) {
    throw std::runtime_error("Incorrect matrix in SumMatrix");
  }
  for (int i = 0; i < rows_ * cols_; i++) {
    matrix_[i] += other.matrix_[i];
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (!CheckSize(other) && other.matrix_) {
    throw std::runtime_error("Incorrect matrix in SubMatrix");
  }
  for (int i = 0; i < rows_ * cols_; i++) {
    matrix_[i] -= other.matrix_[i];
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::logic_error("Incorrect matrix size in MulMatrix");
  }
  S21Matrix tmp(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      tmp.matrix_[i * other.cols_ + j] = 0;
      for (int k = 0; k < cols_; k++) {
        tmp.matrix_[i * other.cols_ + j] +=
            matrix_[i * cols_ + k] * other.matrix_[k * other.cols_ + j];
      }
    }
  }
  *this = std::move(tmp);
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix tmp(cols_, rows_);
  for (int i = 0; i < cols_; i++) {
    for (int j = 0; j < rows_; j++) {
      tmp.matrix_[i * rows_ + j] = matrix_[j * cols_ + i];
    }
  }
  return tmp;
}

double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::logic_error("Incorrect matrix size for Determinant");
  }
  double result = 1.0;
  S21Matrix tmp{*this};
  int size = rows_;
  for (int i = 0; i < size; ++i) {
    int pivoting = i;
    for (int j = i + 1; j < size; ++j) {
      if (std::abs(tmp(j, i)) > std::abs(tmp(pivoting, i))) {
        pivoting = j;
      }
    }
    if (std::abs(tmp(pivoting, i)) < 1e-7) {
      return 0.0;
    }
    SwapRows(tmp, i, pivoting);
    result *= tmp(i, i);
    if (i != pivoting) {
      result = -result;
    }
    for (int j = i + 1; j < size; ++j) {
      double koef = tmp(j, i) / tmp(i, i);
      for (int k = i; k < size; ++k) {
        tmp(j, k) -= tmp(i, k) * koef;
      }
    }
  }

  return result;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) {
    std::logic_error("Non quadratic matrix in CalcComplements");
  }
  S21Matrix tmp(rows_, cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      tmp.matrix_[i * cols_ + j] = Addition(i, j);
    }
  }
  return tmp;
}

S21Matrix S21Matrix::InverseMatrix() const {
  if (rows_ != cols_) {
    throw std::logic_error("Non quadratic matrix in InverseMatrix");
  }
  double det = Determinant();
  if (std::abs(det) < 1e-7) {
    throw std::logic_error("Zero determinant in InverseMatrix");
  }
  return S21Matrix(CalcComplements().Transpose() * (1 / det));
}

//=================================================================================================
/*
Task operators
*/
double& S21Matrix::operator()(int i, int j) {
  if (0 <= i && i < rows_ && 0 <= j && j < cols_) {
    return matrix_[i * cols_ + j];
  } else {
    throw std::out_of_range("incorrect indexes in operator()");
  }
}

double S21Matrix::operator()(int i, int j) const {
  if (0 <= i && i < rows_ && 0 <= j && j < cols_) {
    return matrix_[i * cols_ + j];
  } else {
    throw std::out_of_range("incorrect indexes in operator()");
  }
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  this->SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  this->SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  this->MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(double num) {
  this->MulNumber(num);
  return *this;
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  S21Matrix tmp(other);
  *this = std::move(tmp);
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  std::swap(matrix_, other.matrix_);
  std::swap(cols_, other.cols_);
  std::swap(rows_, other.rows_);
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SumMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SubMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.MulMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator*(const double num) const {
  S21Matrix tmp(*this);
  tmp.MulNumber(num);
  return tmp;
}

bool S21Matrix::operator==(const S21Matrix& other) const {
  return this->EqMatrix(other);
}

//=================================================================================================
/*
Helps functions

*/

void S21Matrix::SwapRows(S21Matrix& tmp, int k, int m) const {
  for (int j = 0; j < cols_; j++) {
    std::swap(tmp.matrix_[m * cols_ + j], tmp.matrix_[k * cols_ + j]);
  }
}

bool S21Matrix::CheckSize(const S21Matrix& other) const {
  return cols_ == other.cols_ && rows_ == other.rows_;
}

double S21Matrix::Addition(int i, int j) const {
  S21Matrix tmp(rows_ - 1, cols_ - 1);
  for (int k = 0; k < rows_ - 1; k++) {
    for (int m = 0; m < cols_ - 1; m++) {
      tmp.matrix_[k * (cols_ - 1) + m] =
          matrix_[((k < i) ? k : k + 1) * cols_ + ((m < j) ? m : m + 1)];
    }
  }
  return ((i + j) % 2 ? -1 : 1) * tmp.Determinant();
}
//=================================================================================================

S21Matrix operator*(const double num, const S21Matrix& matrix) {
  S21Matrix tmp(matrix);
  tmp.MulNumber(num);
  return tmp;
}