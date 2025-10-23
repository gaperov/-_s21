#ifndef S21_CONTAINERS_ARRAY
#define S21_CONTAINERS_ARRAY

#include <stddef.h>

#include <algorithm>
#include <stdexcept>

namespace s21 {
template <typename T, size_t N>
class array {
 private:
  using const_iterator = const T *;
  using iterator = T *;
  using const_reference = const T &;
  using size_type = size_t;
  using reference = T &;
  using value_type = T;

  T *data_;

 public:
  array() : data_(N > 0 ? new value_type[N] : nullptr) {
    if (N > 0) std::fill_n(data_, N, 0);
  }

  array(std::initializer_list<value_type> const &items)
      : data_(N > 0 ? new value_type[N] : nullptr) {
    if (N > 0) std::copy(items.begin(), items.end(), data_);
  }
  array(const array<T, N> &a) : data_(N > 0 ? new value_type[N] : nullptr) {
    if (N > 0) std::copy(a.data_, a.data_ + N, data_);
  }

  array(array<T, N> &&a) : data_(N > 0 ? new value_type[N] : nullptr) {
    if (N > 0) std::move(a.data_, a.data_ + N, data_);
  }

  ~array() {
    if (data_ != nullptr) delete[] data_;
    data_ = nullptr;
  }

  array<T, N> &operator=(const array<T, N> &a) {
    if (this != &a) std::copy(a.cbegin(), a.cend(), data_);
    return *this;
  }

  array<T, N> &operator=(array<T, N> &&a) {
    if (this != &a) data_ = std::exchange(a.data_, nullptr);
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= N) throw std::out_of_range("Incorrect pos");
    return (*this)[pos];
  }

  reference at(size_type pos) const {
    if (pos >= N) throw std::out_of_range("Incorrect pos");
    return *(data_ + pos);
  }

  reference operator[](size_type pos) {
    if (pos >= N) throw std::out_of_range("Incorrect pos");
    return data_[pos];
  }

  const_reference front() const { return *data_; }

  const_reference back() const { return *(data_ + N - 1); }

  T *data() { return data_; }

  iterator begin() { return data_; }
  iterator end() { return data_ + N; }

  iterator cbegin() const { return data_; }
  iterator cend() const { return data_ + N; }

  bool empty() { return N == 0; }
  size_type size() { return N; }
  size_type max_size() { return N; }

  void swap(array<T, N> &other) { std::swap(data_, other.data_); }

  void fill(const_reference value) {
    for (size_type i = 0; i < N; i++) data_[i] = value;
  }
};
}  // namespace s21

#endif