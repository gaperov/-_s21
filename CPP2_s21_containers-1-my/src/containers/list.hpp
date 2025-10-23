#ifndef S21LIST_H
#define S21LIST_H

#include <algorithm>
#include <iostream>
#include <limits>

namespace s21 {
template <typename T>
class list {
 private:
  using value_type = T;
  typedef struct Node {
    value_type data;
    struct Node *next;
    struct Node *back;
  } Node_;

 public:
  class ListIterator {
    friend class list;

   private:
    using iterator_pointer = Node_ *;
    using reference = value_type &;
    iterator_pointer ptr_;

   public:
    ListIterator() = default;
    ListIterator(iterator_pointer p) : ptr_(p) {}

    reference operator*() {
      if (!ptr_) throw std::out_of_range("Pointer is NULL");
      return ptr_->data;
    }
    ListIterator operator++(int) {
      ListIterator copy(*this);
      ptr_ = ptr_ ? ptr_->next : nullptr;
      return copy;
    }
    ListIterator operator--(int) {
      ListIterator copy(*this);
      ptr_ = ptr_ ? ptr_->back : nullptr;
      return copy;
    }
    ListIterator &operator++() {
      ptr_ = ptr_ ? ptr_->next : nullptr;
      return *this;
    }
    ListIterator &operator--() {
      ptr_ = ptr_ ? ptr_->back : nullptr;
      return *this;
    }
    ListIterator operator+(int n) const {
      ListIterator copy(*this);
      for (int i = 0; i < n; i++) copy++;
      return copy;
    }
    ListIterator operator-(int n) const {
      ListIterator copy(*this);
      for (int i = 0; i < n; i++) copy--;
      return copy;
    }

    bool operator==(const ListIterator &other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const ListIterator &other) const {
      return ptr_ != other.ptr_;
    }
    bool isNULL() const { return ptr_ ? false : true; }

   private:
    Node *getNode() { return ptr_; }
    bool lastNode() { return ptr_->next ? false : true; }
    bool inRange(Node *end) {
      ListIterator temp(end), copy(*this);
      while (copy != temp) {
        try {
          *copy;
        } catch (const std::out_of_range &e) {
          return false;
        }
        copy++;
      }

      return true;
    }
  };

 private:
  using reference = T &;
  using const_reference = const T &;
  using iterator = ListIterator;
  using size_type = size_t;

  Node_ *begin_;
  Node_ *end_;
  size_type size_, quantity_;

 public:
  list() : begin_(nullptr), end_(nullptr), size_(0), quantity_(0) {}
  list(size_type n) : begin_(nullptr), end_(nullptr), size_(0), quantity_(0) {
    for (size_type i = 0; i < n; i++) this->push_back(0);
    end_ = nullptr;
    quantity_ = 0;
  }
  list(std::initializer_list<value_type> const &items)
      : begin_(nullptr), end_(nullptr), size_(0), quantity_(0) {
    for (auto current = items.begin(); current != items.end(); current++) {
      this->push_back(*current);
    }
  }
  list(const list &l) : begin_(nullptr), end_(nullptr), size_(0), quantity_(0) {
    ListIterator current = ListIterator(l.begin_);
    for (size_type i = 0; i < l.size_; current++, i++) {
      this->push_back(*current);
    }
    this->end_ = this->begin_;
    for (size_type i = 1; i < l.quantity_; i++) {
      this->end_ = this->end_->next;
    }
    this->quantity_ = l.quantity_;
  }
  list(list &&l) {
    begin_ = std::exchange(l.begin_, nullptr);
    end_ = std::exchange(l.end_, nullptr);
    size_ = std::exchange(l.size_, 0);
    quantity_ = std::exchange(l.quantity_, 0);
  }
  list<value_type> &operator=(list &&l) {
    Node_ *temp = begin_ ? begin_->next : nullptr;
    while (temp) {
      delete[] begin_;
      begin_ = temp;
      temp = temp->next;
    }
    delete[] begin_;
    begin_ = std::exchange(l.begin_, nullptr);
    end_ = std::exchange(l.end_, nullptr);
    size_ = std::exchange(l.size_, 0);
    quantity_ = std::exchange(l.quantity_, 0);

    return *this;
  }
  ~list() {
    Node_ *temp = begin_ ? begin_->next : nullptr;
    while (temp) {
      delete[] begin_;
      begin_ = temp;
      temp = temp->next;
    }
    delete[] begin_;
    begin_ = nullptr;
    end_ = nullptr;
    size_ = 0;
    quantity_ = 0;
  }

  const_reference front() { return *ListIterator(begin_); }
  const_reference back() { return *ListIterator(end_); }

  iterator begin() { return ListIterator(begin_); }
  iterator end() { return ListIterator(end_); }

  bool empty() { return !size_ ? true : false; }
  size_type size() { return size_; }
  size_type quantity() { return quantity_; }
  size_type max_size() {
    return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
  }

  void clear() {
    Node_ *temp = begin_ ? begin_->next : nullptr;
    while (temp) {
      delete[] begin_;
      begin_ = temp;
      temp = temp->next;
    }
    delete[] begin_;
    begin_ = nullptr;
    end_ = nullptr;
    size_ = 0;
    quantity_ = 0;
  }
  iterator insert(iterator pos, const_reference value) {
    if (!end_) {
      this->push_back(value);
      return iterator(begin_);
    }
    if (pos == iterator(begin_)) {
      this->push_front(value);
      return iterator(begin_);
    }
    if (pos.isNULL()) throw std::out_of_range("Iterator is NULL!");
    if (!pos.inRange(end_)) throw std::out_of_range("Iterator out of range!");

    Node *new_node = new Node_[1];
    new_node->data = value;
    new_node->next = pos.getNode()->next;
    new_node->back = pos.getNode();
    if (pos.getNode()->next) pos.getNode()->next->back = new_node;
    pos.getNode()->next = new_node;

    size_++;
    quantity_++;

    return ListIterator(pos + 1);
  }
  void erase(iterator pos) {
    if (pos.isNULL()) throw std::out_of_range("Iterator is NULL!");
    size_--;
    if (pos.inRange(end_)) quantity_--;

    Node *current = pos.getNode();
    if (current->back) {
      current->back->next = current->next;
      if (current->next) {
        current->next->back = current->back;
      } else {
        end_ = end_->back;
      }
    } else {
      begin_ = begin_->next;
    }

    delete[] current;
    if (!quantity_) end_ = nullptr;
  }
  void push_back(const_reference value) {
    quantity_++;
    if (size_ == 0) {
      Node *new_node = new Node[1];
      new_node->data = value;
      new_node->next = nullptr;
      new_node->back = nullptr;
      begin_ = new_node;
      end_ = new_node;
      size_++;
    } else {
      if (end_ == nullptr) {
        end_ = begin_;
        end_->data = value;
      } else {
        if (end_->next == nullptr) {
          Node *new_node = new Node[1];
          new_node->data = value;
          new_node->next = nullptr;
          new_node->back = end_;
          end_->next = new_node;
          end_ = new_node;
          size_++;
        } else {
          end_ = end_->next;
          end_->data = value;
        }
      }
    }
  }
  void pop_back() {
    if (end_) {
      end_ = end_->back;
      quantity_--;
    }
  }
  void push_front(const_reference value) {
    Node *new_node = new Node[1];
    new_node->data = value;
    new_node->next = nullptr;
    new_node->back = nullptr;

    if (size_ == 0) {
      begin_ = new_node;
      end_ = new_node;
    } else {
      begin_->back = new_node;
      new_node->next = begin_;
      begin_ = new_node;
    }
    !quantity_ ? end_ = begin_ : 0;
    quantity_++;
    size_++;
  }
  void pop_front() {
    Node *temp = begin_ ? begin_->next : nullptr;
    delete[] begin_;
    begin_ = temp;
    size_ ? size_-- : 0;
    quantity_ ? quantity_-- : 0;
  }
  void swap(list &other) {
    std::swap(begin_, other.begin_);
    std::swap(end_, other.end_);
    std::swap(size_, other.size_);
    std::swap(quantity_, other.quantity_);
  }
  void splice(iterator pos, list &other) {
    if (!begin_) {
      *this = std::move(other);
      return;
    }
    if (!other.begin_) return;
    if (pos.isNULL()) throw std::out_of_range("Iterator is NULL!");

    size_t temp = 0;
    iterator copy_end(end_);
    if (!pos.inRange(end_)) {
      while (copy_end != pos) {
        copy_end++;
        temp++;
        try {
          *copy_end;
        } catch (std::out_of_range &e) {
          throw std::logic_error("The iterator does not apply to this list!");
        }
      }
    }
    Node *current = pos.getNode();
    Node *currentBack = current->back;
    Node *otherBegin = other.begin().getNode();
    Node *otherEnd = other.end().getNode();

    if (currentBack)
      currentBack->next = otherBegin;
    else
      begin_ = otherBegin;
    otherBegin->back = currentBack;
    current->back = otherEnd;
    otherEnd->next = current;

    size_ += other.size_;
    quantity_ += other.size_ + temp;
    end_ = (iterator(begin_) + (quantity_ - 1)).getNode();

    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.size_ = 0;
    other.quantity_ = 0;
  }
  void reverse() {
    iterator begin(begin_);
    iterator end(end_);

    for (size_type i = 0; i < quantity_ / 2; i++) {
      std::swap(*begin, *end);
      begin++;
      end--;
    }
  }
  void unique() {
    if (!quantity_) return;

    iterator current(begin_);
    iterator next(begin_ ? begin_->next : nullptr);
    while (next != iterator(end_) + 1) {
      if (*current == *next) {
        this->erase(next);
        next = current + 1;
        continue;
      }
      current++;
      next++;
    }
  }
  void sort() {
    for (iterator min_iter(begin_); min_iter != iterator(end_); min_iter++) {
      iterator temp = min_iter;
      for (iterator current = min_iter + 1; current != iterator(end_) + 1;
           current++) {
        if (*min_iter > *current) min_iter = current;
      }
      if (temp != min_iter) std::swap(*min_iter, *temp);
      min_iter = temp;
    }
  }
  void merge(list &other) {
    if (this == &other)
      throw std::logic_error("You can't merge list in itself!");
    if (!other.begin_) return;
    if (!begin_) {
      *this = std::move(other);
      return;
    }

    iterator thisBegin(begin_);
    iterator otherBegin(other.begin_);
    Node *temp;

    while (!thisBegin.isNULL() && !otherBegin.isNULL()) {
      if (*thisBegin >= *otherBegin) {
        temp = otherBegin.getNode();
        otherBegin++;
        temp->next = thisBegin.getNode();
        temp->back = thisBegin.getNode()->back;
        if (thisBegin.getNode()->back)
          thisBegin.getNode()->back->next = temp;
        else
          begin_ = temp;
        thisBegin.getNode()->back = temp;
        quantity_++;
      } else
        thisBegin++;
    }

    thisBegin = iterator(end_);
    if (!otherBegin.isNULL()) {
      temp = thisBegin.getNode();
      thisBegin++;
      temp->next = otherBegin.getNode();
      otherBegin.getNode()->back = temp;

      while (otherBegin != iterator(other.end_)) {
        quantity_++;
        otherBegin++;
      }
      quantity_++;

      otherBegin.getNode()->next = thisBegin.getNode();
      if (thisBegin.getNode()) {
        thisBegin.getNode()->back = otherBegin.getNode();
      }
    }

    size_ += other.size_;
    end_ = (iterator(begin_) + (quantity_ - 1)).getNode();
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.size_ = 0;
    other.quantity_ = 0;
  }

  template <typename... Args>
  iterator insert_many(iterator pos, Args &&...args) {
    if (pos.isNULL() && end_) throw std::out_of_range("Iterator is NULL!");
    if (!pos.inRange(end_)) throw std::out_of_range("Iterator out of range!");

    list temp;
    ((temp.push_back(args)), ...);
    this->splice(pos.isNULL() ? ListIterator(begin_) : pos, temp);

    return ListIterator(pos - sizeof...(Args));
  }
  template <typename... Args>
  void insert_many_back(Args &&...args) {
    (this->push_back(args), ...);
  }
  template <typename... Args>
  void insert_many_front(Args &&...args) {
    list temp;
    ((temp.push_back(args)), ...);
    this->splice(ListIterator(begin_), temp);
  }

  void print() {
    ListIterator currentFront = ListIterator(begin_);
    ListIterator currentBack = ListIterator(end_);
    std::cout << "Size: " << size_ << std::endl;
    std::cout << "Quantity: " << quantity_ << std::endl;
    std::cout << "Begin: " << (!currentFront.isNULL() ? *currentFront : 0)
              << std::endl;
    std::cout << "End: " << (!currentBack.isNULL() ? *currentBack : 0)
              << std::endl;
    std::cout << "List Front: ";
    for (int i = 0; i < quantity_; i++) {
      std::cout << *currentFront << ' ';
      currentFront++;
    }
    std::cout << std::endl;
    std::cout << "List Back: ";
    for (int i = 0; i < quantity_; i++) {
      std::cout << *currentBack << ' ';
      currentBack--;
    }
    std::cout << std::endl << std::endl;
  }
};
}  // namespace s21

#endif  // S21LIST_H