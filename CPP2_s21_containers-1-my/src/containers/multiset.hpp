#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace s21 {
using namespace std;
// Implementing Red-Black Tree in C++
template <typename Key>
class multiset {
 public:
  class multisetIterator;

 private:
  using key_type = Key;
  using mapped_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = multisetIterator;
  // using const_iterator = multisetConstIterator<key_type, mapped_type>;
  using size_type = size_t;

  struct Node {
    key_type key;
    Node* parent;
    Node* left;
    Node* right;
    int color;
  };
  typedef Node* NodePtr;
  // iterator temp = multisetIterator(nullptr, nullptr, nullptr);
  NodePtr root;
  NodePtr TNULL;
  size_type size_;

  // class multisetIterator<key_type, mapped_type> {};

  void initializeNULLNode(NodePtr node, NodePtr parent) {
    node->key = 0;
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = 0;
  }

  // Preorder
  void preOrderHelper(NodePtr node) {
    if (node != TNULL) {
      std::cout << node->key << " ";
      preOrderHelper(node->left);
      preOrderHelper(node->right);
    }
  }

  // Inorder
  void inOrderHelper(NodePtr node) {
    if (node != TNULL) {
      inOrderHelper(node->left);
      std::cout << node->key << " ";
      inOrderHelper(node->right);
    }
  }

  // Post order
  void postOrderHelper(NodePtr node) {
    if (node != TNULL) {
      postOrderHelper(node->left);
      postOrderHelper(node->right);
      cout << node->key << " ";
    }
  }

  NodePtr searchTreeHelper(NodePtr node, key_type key) {
    if (node == TNULL) {
      return node;
    }

    if (key == node->key) {
      while (node->left != TNULL && node->left->key == key) node = node->left;
      return node;
    }

    if (key < node->key) {
      return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
  }

  // For balancing the tree after deletion
  void deleteFix(NodePtr x) {
    NodePtr s;
    while (x != root && x->color == 0) {
      if (x == x->parent->left) {
        s = x->parent->right;
        if (s->color == 1) {
          s->color = 0;
          x->parent->color = 1;
          leftRotate(x->parent);
          s = x->parent->right;
        }

        if (s->left->color == 0 && s->right->color == 0) {
          s->color = 1;
          x = x->parent;
        } else {
          if (s->right->color == 0) {
            s->left->color = 0;
            s->color = 1;
            rightRotate(s);
            s = x->parent->right;
          }

          s->color = x->parent->color;
          x->parent->color = 0;
          s->right->color = 0;
          leftRotate(x->parent);
          x = root;
        }
      } else {
        s = x->parent->left;
        if (s->color == 1) {
          s->color = 0;
          x->parent->color = 1;
          rightRotate(x->parent);
          s = x->parent->left;
        }

        if (s->right->color == 0 && s->right->color == 0) {
          s->color = 1;
          x = x->parent;
        } else {
          if (s->left->color == 0) {
            s->right->color = 0;
            s->color = 1;
            leftRotate(s);
            s = x->parent->left;
          }

          s->color = x->parent->color;
          x->parent->color = 0;
          s->left->color = 0;
          rightRotate(x->parent);
          x = root;
        }
      }
    }
    x->color = 0;
  }

  void rbTransplant(NodePtr u, NodePtr v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }

  NodePtr minimum(NodePtr node) {
    while (node->left != TNULL) {
      node = node->left;
    }
    return node;
  }

  NodePtr maximum(NodePtr node) {
    while (node->right != TNULL) {
      node = node->right;
    }
    return node;
  }

  void deleteNodeHelper(NodePtr node, Node* n) {
    NodePtr z = TNULL;
    NodePtr x, y;
    while (node != TNULL) {
      if (node->key == n->key) {
        z = node;
      }

      if (node->key <= n->key) {
        node = node->right;
      } else {
        node = node->left;
      }
    }

    if (z == TNULL) {
      std::cout << "Key not found in the tree\n";
      return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
      x = z->right;
      rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
      x = z->left;
      rbTransplant(z, z->left);
    } else {
      y = minimum(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z) {
        x->parent = y;
      } else {
        rbTransplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }

      rbTransplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    delete z;
    if (y_original_color == 0) {
      deleteFix(x);
    }
  }

  // For balancing the tree after insertion
  void insertFix(NodePtr k) {
    NodePtr u;
    while (k->parent->color == 1) {
      if (k->parent == k->parent->parent->right) {
        u = k->parent->parent->left;
        if (u->color == 1) {
          u->color = 0;
          k->parent->color = 0;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->left) {
            k = k->parent;
            rightRotate(k);
          }
          k->parent->color = 0;
          k->parent->parent->color = 1;
          leftRotate(k->parent->parent);
        }
      } else {
        u = k->parent->parent->right;

        if (u->color == 1) {
          u->color = 0;
          k->parent->color = 0;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->right) {
            k = k->parent;
            leftRotate(k);
          }
          k->parent->color = 0;
          k->parent->parent->color = 1;
          rightRotate(k->parent->parent);
        }
      }
      if (k == root) {
        break;
      }
    }
    root->color = 0;
  }

  void printHelper(NodePtr root, string indent, bool last) {
    if (root != TNULL) {
      cout << indent;
      if (last) {
        cout << "R----";
        indent += "   ";
      } else {
        cout << "L----";
        indent += "|  ";
      }

      string sColor = root->color ? "RED" : "BLACK";
      cout << root->key << "(" << sColor << ")" << endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

  Node* insertNode(Node* new_node) {
    NodePtr y = nullptr;
    NodePtr x = this->root;

    while (x != TNULL) {
      y = x;
      if (new_node->key < x->key) {
        x = x->left;
      } else {
        x = x->right;
      }
    }

    new_node->parent = y;
    if (y == nullptr) {
      root = new_node;
    } else if (new_node->key < y->key) {
      y->left = new_node;
    } else {
      y->right = new_node;
    }

    if (new_node->parent == nullptr) {
      new_node->color = 0;
      return new_node;
    }

    if (new_node->parent->parent == nullptr) {
      return new_node;
    }

    insertFix(new_node);
    return new_node;
  }

  NodePtr searchTree(key_type key) { return searchTreeHelper(this->root, key); }

  NodePtr successor(NodePtr x) {
    if (x->right != TNULL) {
      return minimum(x->right);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  NodePtr predecessor(NodePtr x) {
    if (x->left != TNULL) {
      return maximum(x->left);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->left) {
      x = y;
      y = y->parent;
    }

    return y;
  }

  void leftRotate(NodePtr x) {
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
  }

  void rightRotate(NodePtr x) {
    NodePtr y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
      y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
  }

  Node* copy(Node* n, Node* tn, Node* otn) {
    if (n == otn || n == nullptr) return TNULL;
    Node* t = new Node;
    t->parent = tn;
    t->color = n->color;
    t->key = n->key;
    t->right = copy(n->right, t, otn);
    t->left = copy(n->left, t, otn);
    return t;
  }

 public:
  multiset() {
    TNULL = new Node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
    size_ = 0;
  }

  multiset(std::initializer_list<value_type> const& items) {
    TNULL = new Node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
    size_ = 0;

    for (value_type p : items) {
      insert(p);
    }
  }

  multiset(const multiset& m) {
    size_ = m.size_;
    TNULL = new Node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = copy(m.root, nullptr, m.TNULL);
  }

  multiset(multiset&& m) {
    TNULL = std::exchange(m.TNULL, nullptr);
    root = std::exchange(m.root, nullptr);
    size_ = std::exchange(m.size_, 0);
    m.clear();
  }

  multiset operator=(multiset&& s) {
    TNULL = std::exchange(s.TNULL, nullptr);
    root = std::exchange(s.root, nullptr);
    size_ = std::exchange(s.size_, 0);
    s.clear();
  }

  ~multiset() {
    if (root != TNULL) {
      Node* m = maximum(root);
      while (m != root) {
        deleteNodeHelper(this->root, m);
        m = maximum(root);
      }

      m = minimum(root);
      while (m != root) {
        deleteNodeHelper(this->root, m);
        m = minimum(root);
      }
    }

    if (root == TNULL)
      delete root;
    else {
      delete root;
      delete TNULL;
    }
  }

  void preorder() { preOrderHelper(this->root); }

  void inorder() { inOrderHelper(this->root); }

  void postorder() { postOrderHelper(this->root); }

  // Inserting a node
  pair<multisetIterator, bool> insert(const value_type& value) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->key = value;
    node->left = TNULL;
    node->right = TNULL;
    node->color = 1;

    bool s = this->contains(value);
    Node* n;
    size_++;
    n = insertNode(node);
    return make_pair(multisetIterator(this->root, n, this->TNULL), !s);
  }

  NodePtr getRoot() { return this->root; }

  void erase(iterator n) {
    deleteNodeHelper(this->root, n.ptr_);
    size_--;
  }

  void printTree() {
    if (root) {
      printHelper(this->root, "", true);
    }
  }

  bool empty() { return root == TNULL; }

  size_type size() { return size_; }

  size_type max_size() {
    return numeric_limits<size_type>::max() /
           sizeof(typename multiset<key_type>::Node);
  }

  void clear() {
    if (root != TNULL) {
      Node* m = maximum(root);
      while (m != root) {
        deleteNodeHelper(this->root, m);
        m = maximum(root);
      }

      m = minimum(root);
      while (m != root) {
        deleteNodeHelper(this->root, m);
        m = minimum(root);
      }

      size_ = 0;

      delete root;
      root = TNULL;
    }
  }

  bool contains(const Key& key) { return searchTree(key) != TNULL; }

  iterator lower_bound(key_type key) {
    multisetIterator t1 = multisetIterator(root, searchTree(key), TNULL);
    multisetIterator t = t1--;
    while (*t1 == *t) {
      t--;
      t1--;
    }
    return t;
  }

  iterator upper_bound(key_type key) {
    iterator t = lower_bound(key);
    while (*t == key) {
      ++t;
    }
    return t;
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  void swap(multiset& other) {
    Node* t = other.root;
    other.root = root;
    root = t;

    t = other.TNULL;
    other.TNULL = TNULL;
    TNULL = t;

    int s = other.size_;
    other.size_ = size_;
    size_ = s;
  }

  void merge(multiset& other) {
    for (auto b = other.begin(); b != other.end(); ++b) {
      this->insert(*b);
    }
  }

  class multisetIterator {
   private:
    Node* root_;
    Node* TNULL_;
    value_type fake;

    NodePtr minimum(NodePtr node) {
      while (node->left != TNULL_) {
        node = node->left;
      }
      return node;
    }

    NodePtr maximum(NodePtr node) {
      while (node->right != TNULL_) {
        node = node->right;
      }
      return node;
    }

    NodePtr successor(NodePtr x) {
      if (x->right != TNULL_) {
        return minimum(x->right);
      }

      NodePtr y = x->parent;
      while (y != TNULL_ && x == y->right) {
        x = y;
        y = y->parent;
      }
      if (y == nullptr) {
        std::cout << "nulp\n";
      } else if (y == TNULL_) {
        std::cout << "TNULL\n";
      }
      return y;
    }

    NodePtr predecessor(NodePtr x) {
      if (x->left != TNULL_) {
        return maximum(x->left);
      }

      NodePtr y = x->parent;
      while (y != TNULL_ && x == y->left) {
        x = y;
        y = y->parent;
      }

      return y;
    }

   public:
    Node* ptr_;
    multisetIterator(Node* r, Node* n, Node* tn) {
      ptr_ = n;
      root_ = r;
      TNULL_ = tn;
      // fake = std::make_pair((key_type)0, (mapped_type)0);
    }

    multisetIterator& operator++() {
      if (root_ != TNULL_ && ptr_ == maximum(root_))
        ptr_ = TNULL_;
      else
        ptr_ = successor(ptr_);
      return *this;
    }

    multisetIterator& operator--() {
      if (root_ != TNULL_ && ptr_ == TNULL_) {
        ptr_ = maximum(root_);
      } else {
        ptr_ = predecessor(ptr_);
      }
      return *this;
    }

    multisetIterator operator++(int) {
      multisetIterator t = *this;
      ++*this;
      return t;
    }

    multisetIterator operator--(int) {
      multisetIterator t = *this;
      --*this;
      return t;
    }

    value_type& operator*() {
      fake = ptr_->key;
      value_type& r = fake;
      return r;
    }

    bool operator==(const multisetIterator& other) {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const multisetIterator& other) {
      return ptr_ != other.ptr_;
    }
  };

  iterator begin() {
    // std::cout << "\nbegin state " << (root == TNULL) << (root == nullptr) <<
    // (TNULL == nullptr) << "\n"; minimum(root);

    // return multisetIterator(nullptr, nullptr, nullptr);
    return multisetIterator(root, root == TNULL ? TNULL : minimum(root), TNULL);
  }

  iterator end() {
    if (root == TNULL) return multisetIterator(root, TNULL, TNULL);
    multisetIterator r(root, TNULL, TNULL);
    TNULL->key = maximum(root)->key;
    return r;
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::vector<std::pair<iterator, bool>> res;
    std::vector<key_type> d = {args...};
    for (int i = 0; i < d.size(); i++) {
      res.push_back(insert(d[i]));
    }
    return res;
  }
};
}  // namespace s21