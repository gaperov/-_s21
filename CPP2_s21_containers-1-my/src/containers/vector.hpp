#ifndef S21_CONTAINERS_VECTOR
#define S21_CONTAINERS_VECTOR

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace s21 {

template <typename T>
class vector {
 public:
  class VectorIterator {
   private:
    using iterator_pointer = T *;
    using reference = T &;

    iterator_pointer ptr_;

   public:
    VectorIterator() = default;
    VectorIterator(iterator_pointer p) : ptr_(p) {}

    reference operator*() { return *ptr_; }
    VectorIterator operator++(int) {
      VectorIterator t(*this);
      ptr_++;
      return t;
    }

    VectorIterator operator--(int) {
      VectorIterator t(*this);
      ptr_--;
      return t;
    }

    VectorIterator &operator++() {
      ptr_++;
      return *this;
    }

    VectorIterator &operator--() {
      ptr_--;
      return *this;
    }

    VectorIterator operator+(int n) const {
      VectorIterator t(*this);
      for (int i = 0; i < n; i++) t++;
      return t;
    }

    VectorIterator operator-(int n) const {
      VectorIterator t(*this);
      for (int i = 0; i < n; i++) t--;
      return t;
    }

    bool operator==(const VectorIterator &other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const VectorIterator &other) const {
      return ptr_ != other.ptr_;
    }
    ptrdiff_t operator-(const VectorIterator &other) const {
      return ptr_ - other.ptr_;
    }
  };

 private:
  using value_type = T;
  using size_type = size_t;
  using reference = T &;
  using const_reference = const T &;
  using iterator_pointer = VectorIterator *;
  using const_iterator = const T *;
  using iterator = VectorIterator;
  // using const_iterator = VectorConstIterator;

  T *data_;
  size_type size_;
  size_type capacity_;

 public:
  vector() : data_(nullptr), size_(0), capacity_(0) {}

  vector(size_type n)
      : data_(n != 0 ? new value_type[n] : nullptr), size_(n), capacity_(n) {
    std::fill_n(data_, n, 0);
  }

  vector(const vector &v)
      : data_(v.size() == 0 ? nullptr : new T[v.capacity_]),
        size_(v.size_),
        capacity_(v.capacity_) {
    std::copy(v.data_, v.data_ + v.size_, data_);
  }

  vector(vector &&v) {
    size_ = std::exchange(v.size_, 0);
    capacity_ = std::exchange(v.capacity_, 0);
    data_ = std::exchange(v.data_, nullptr);
    std::cout << "\nThat was mave const\n";
  }

  vector(std::initializer_list<value_type> const &items) {
    size_ = capacity_ = items.size();
    data_ = new T[capacity_];
    std::copy(items.begin(), items.end(), data_);
  }

  ~vector() {
    delete[] data_;

    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }

  vector<T> &operator=(vector &&v) {
    if (this != &v) {
      std::swap(v.size_, size_);
      std::swap(v.capacity_, capacity_);
      std::swap(v.data_, data_);

      delete[] v.data_;
      v.size_ = 0;
      v.capacity_ = 0;
      v.data_ = nullptr;
    }
    return *this;
  }

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }
  size_type max_size() const {
    return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
  }
  void reserve(size_type size) {
    if (size > capacity_) {
      capacity_ = size;
      T *ndata = new T[capacity_];
      std::copy(data_, data_ + size_, ndata);
      ndata = std::exchange(data_, ndata);
      delete[] ndata;
    }
  }

  size_type capacity() const { return capacity_; }

  void shrink_to_fit() {
    if (capacity_ > size_) {
      T *t = new T[size_];
      std::copy(data_, data_ + size_, t);
      t = std::exchange(data_, t);
      capacity_ = size_;
      delete[] t;
    }
  }

  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("Incorrect index");
    return data_[pos];
  }

  reference operator[](size_type pos) {
    if (pos >= size_) throw std::out_of_range("Incorrect index");
    return data_[pos];
  }

  const_reference operator[](size_type pos) const {
    if (pos >= size_) throw std::out_of_range("Incorrect index");
    return data_[pos];
  }

  const_reference front() const {
    if (size_ == 0) throw std::out_of_range("Data is empty");
    return data_[0];
  }

  const_reference back() const {
    if (size_ == 0) throw std::out_of_range("Data is empty");
    return data_[size_ - 1];
  }

  iterator begin() { return VectorIterator(data_); }
  iterator end() { return VectorIterator(data_ + size_); }

  const_iterator cbegin() const { return data_; }
  const_iterator cend() const { return data_ + size_; }

  T *data() { return data_; }

  void clear() {
    size_ = 0;
    capacity_ = 0;
    delete[] data_;
    data_ = nullptr;
  }

  iterator insert(iterator pos, const_reference value) {
    size_type index = pos - begin();
    if (index > size_) throw std::invalid_argument("Incorrect pos");
    if (size_ == capacity_) capacity_ = (capacity_ ? capacity_ * 2 : 1);
    T *ndata = new T[capacity_];
    std::copy(data_, data_ + index, ndata);
    ndata[index] = value;
    std::copy(data_ + index, data_ + size_, ndata + index + 1);
    ndata = std::exchange(data_, ndata);
    delete[] ndata;
    size_++;
    return VectorIterator(data_ + index);
  }

  void erase(iterator pos) {
    size_type index = pos - begin();
    if (index >= size_) throw std::invalid_argument("Incorrect pos");
    T *ndata = new T[capacity_];
    std::copy(data_, data_ + index, ndata);
    std::copy(data_ + index + 1, data_ + size_, ndata + index);
    ndata = std::exchange(data_, ndata);
    size_--;
    delete[] ndata;
  }

  void push_back(const_reference value) {
    size_type index = end() - begin();
    if (size_ < capacity_) {
      data_[index] = value;
      size_++;
    } else {
      insert(end(), value);
    }
  }
  void pop_back() {
    if (size_ != 0) size_--;
  }

  void swap(vector &other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }

  template <typename... Args>
  VectorIterator insert_many(const VectorIterator pos, Args &&...args) {
    vector<value_type> tv = {args...};
    if (pos == end()) {
      for (size_type i = 0; i < tv.size(); i++)
        push_back(tv[tv.size() - i - 1]);
    } else {
      int t = pos - begin();
      for (size_type i = 0; i < tv.size(); i++) {
        VectorIterator npos = begin() + t;
        insert(npos, tv[tv.size() - i - 1]);
      }
    }
    return pos;
  }
  template <typename... Args>
  void insert_many_back(Args &&...args) {
    vector<value_type> tv = {args...};
    for (size_type i = 0; i < tv.size(); i++) push_back(tv[i]);
  }
};
}  // namespace s21

#endif