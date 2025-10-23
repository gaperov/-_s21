#ifndef STACK_H
#define STACK_H

#include <algorithm>
#include <iostream>

namespace s21 {
template <typename T>
class stack {
 private:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  value_type *data_;
  size_type size_, capacity_;

 public:
  stack() : data_(new value_type[1]), size_(0), capacity_(1) {}
  stack(std::initializer_list<value_type> const &items) {
    size_ = items.size();
    capacity_ = size_ > 0 ? size_ : 1;
    data_ = new value_type[capacity_];
    std::copy(items.begin(), items.end(), data_);
  }
  stack(const stack &s)
      : data_(s.capacity_ > 0 ? new value_type[s.capacity_] : nullptr),
        size_(s.size_),
        capacity_(s.capacity_) {
    std::copy(s.data_, s.data_ + s.size_, data_);
  }
  stack(stack &&s) {
    size_ = std::exchange(s.size_, 0);
    capacity_ = std::exchange(s.capacity_, 0);
    data_ = std::exchange(s.data_, nullptr);
  }
  ~stack() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
  stack<value_type> &operator=(stack &&s) {
    if (this != &s) {
      std::swap(s.size_, size_);
      std::swap(s.capacity_, capacity_);
      std::swap(s.data_, data_);

      delete[] s.data_;
      s.size_ = 0;
      s.capacity_ = 0;
      s.data_ = nullptr;
    }
    return *this;
  }

  const_reference top() {
    if (size_ == 0) throw std::out_of_range("Stack is empty");
    return data_[size_ - 1];
  }

  bool empty() { return size_ == 0 ? true : false; }
  size_type size() { return size_; }

  void push(const_reference value) {
    if (size_ == capacity_) {
      capacity_ = capacity_ << 1;
      value_type *new_data = new value_type[capacity_];
      std::copy(data_, data_ + size_, new_data);
      delete[] data_;
      data_ = new_data;
    }
    if (capacity_ == 0) {
      capacity_ = 1;
      data_ = new value_type[capacity_];
    }

    data_[size_] = value;
    size_++;
  }
  void pop() {
    if (size_ > 0) size_--;
  }
  void swap(stack &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template <typename... Args>
  void insert_many_front(Args &&...args) {
    (this->push(args), ...);
  }

  void print() {
    std::cout << "Stack: ";
    for (size_type i = 0; i < size_; i++) std::cout << data_[i] << ' ';
    std::cout << "\nSize: " << size_ << std::endl;
    std::cout << "Capacity: " << capacity_ << std::endl << std::endl;
  }
};
}  // namespace s21

#endif  // STACK_H