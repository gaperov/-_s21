#ifndef S21_QUEUE
#define S21_QUEUE

#include <algorithm>
#include <iostream>

namespace s21 {
template <typename T>
class queue {
 private:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  value_type *data_;
  size_type size_, capacity_, begin_;

 public:
  queue() : data_(nullptr), size_(0), capacity_(0), begin_(0) {}
  queue(std::initializer_list<value_type> const &items) {
    size_ = items.size();
    capacity_ = size_;
    data_ = new value_type[capacity_];
    begin_ = 0;
    std::copy(items.begin(), items.end(), data_);
  }
  queue(const queue &q)
      : data_(q.capacity_ > 0 ? new value_type[q.capacity_] : nullptr),
        size_(q.size_),
        capacity_(q.capacity_),
        begin_(q.begin_) {
    std::copy(q.data_, q.data_ + q.size_ + q.begin_, data_);
  }
  queue(queue &&q) {
    size_ = std::exchange(q.size_, 0);
    capacity_ = std::exchange(q.capacity_, 0);
    begin_ = std::exchange(q.begin_, 0);
    data_ = std::exchange(q.data_, nullptr);
  }
  ~queue() {
    delete[] data_;
    begin_ = 0;
    size_ = 0;
    capacity_ = 0;
  }
  queue<value_type> &operator=(queue &&q) {
    if (this != &q) {
      std::swap(q.data_, data_);
      size_ = q.size_;
      capacity_ = q.capacity_;
      begin_ = q.begin_;

      delete[] q.data_;
      q.size_ = 0;
      q.capacity_ = 0;
      q.data_ = nullptr;
      q.begin_ = 0;
    }
    return *this;
  }

  const_reference front() {
    if (size_ == 0) throw std::out_of_range("Queue is empty!");
    return data_[begin_];
  }
  const_reference back() {
    if (size_ == 0) throw std::out_of_range("Queue is empty!");
    return data_[begin_ + size_ - 1];
  }

  bool empty() { return size_ ? false : true; }
  size_type size() { return size_; }

  void push(const_reference value) {
    if (capacity_ == 0) {
      capacity_ = 1;
      data_ = new value_type[capacity_];
    }
    if (size_ + begin_ == capacity_) {
      capacity_ = capacity_ << 1;
      value_type *new_data = new value_type[capacity_];
      std::copy(data_, data_ + size_ + begin_, new_data);
      delete[] data_;
      data_ = new_data;
    }

    data_[size_ + begin_] = value;
    size_++;
  }
  void pop() {
    begin_++;
    size_ ? size_-- : 0;
    if (size_ == 0) {
      delete[] data_;
      data_ = nullptr;
      capacity_ = 0;
      begin_ = 0;
    }
  }
  void swap(queue &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(begin_, other.begin_);
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    (this->push(args), ...);
  }

  void print() {
    std::cout << "Size: " << size_ << std::endl;
    std::cout << "Capacity: " << capacity_ << std::endl;
    std::cout << "Begin: " << begin_ << std::endl;
    std::cout << "Queue: ";
    for (int i = 0; i < size_; i++) std::cout << data_[begin_ + i] << " ";
    std::cout << std::endl << std::endl;
  }
};
}  // namespace s21

#endif  // S21_QUEUE