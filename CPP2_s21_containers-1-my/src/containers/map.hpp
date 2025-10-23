#ifndef S21_CONTAINERS_MAP
#define S21_CONTAINERS_MAP

#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace s21 {
using namespace std;
// Implementing Red-Black Tree in C++
template <typename Key, typename T>
class map {
 public:
  class MapIterator;

 private:
  using key_type = Key;
  using mapped_type = T;
  using value_type = pair<key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = MapIterator;
  // using const_iterator = MapConstIterator<key_type, mapped_type>;
  using size_type = size_t;

  struct Node {
    key_type key;
    mapped_type data;
    Node* parent;
    Node* left;
    Node* right;
    int color;
  };
  typedef Node* NodePtr;
  // iterator temp = MapIterator(nullptr, nullptr, nullptr);
  NodePtr root;
  NodePtr TNULL;

  // class MapIterator<key_type, mapped_type> {};

  void initializeNULLNode(NodePtr node, NodePtr parent) {
    node->key = 0;
    node->data = 0;
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
    if (node == TNULL || key == node->key) {
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
      cout << root->key << ": " << root->data << "(" << sColor << ")" << endl;
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

  size_type size_;

  Node* copy(Node* n, Node* tn, Node* otn) {
    if (n == otn || n == nullptr) return TNULL;
    Node* t = new Node;
    t->parent = tn;
    t->color = n->color;
    t->data = n->data;
    t->key = n->key;
    t->right = copy(n->right, t, otn);
    t->left = copy(n->left, t, otn);
    return t;
  }

 public:
  map() {
    TNULL = new Node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
    size_ = 0;
  }

  map(std::initializer_list<value_type> const& items) {
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

  map(const map& m) {
    size_ = m.size_;
    TNULL = new Node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = copy(m.root, nullptr, m.TNULL);
  }

  map(map&& m) {
    TNULL = std::exchange(m.TNULL, nullptr);
    root = std::exchange(m.root, nullptr);
    size_ = std::exchange(m.size_, 0);
    m.clear();
  }

  ~map() {
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
  pair<MapIterator, bool> insert(const value_type& value) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->key = value.first;
    node->data = value.second;
    node->left = TNULL;
    node->right = TNULL;
    node->color = 1;

    bool s = this->contains(value.first);
    Node* n;
    if (!s) {
      size_++;
      n = insertNode(node);
    } else {
      n = searchTree(value.first);
      delete node;
    }
    return make_pair(MapIterator(this->root, n, this->TNULL), !s);
  }

  pair<MapIterator, bool> insert(const Key& key, const T& obj) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->key = key;
    node->data = obj;
    node->left = TNULL;
    node->right = TNULL;
    node->color = 1;

    bool s = this->contains(key);
    Node* n;
    if (!s) {
      size_++;
      n = insertNode(node);
    } else {
      n = searchTree(key);
      delete node;
    }
    return make_pair(MapIterator(this->root, n, this->TNULL), !s);
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
           sizeof(typename map<key_type, mapped_type>::Node);
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

  pair<MapIterator, bool> insert_or_assign(const Key& key, const T& obj) {
    if (contains(key)) {
      searchTree(key)->data = obj;
    }

    return insert(key, obj);
  }

  T& at(const Key& key) {
    Node* r = searchTree(key);

    if (r == TNULL) throw std::out_of_range("No key in a tree");
    return r->data;
  }

  T& operator[](const Key& key) { return searchTree(key)->data; }

  bool contains(const Key& key) { return searchTree(key) != TNULL; }

  void swap(map& other) {
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

  void merge(map& other) {
    for (auto b = other.begin(); b != other.end(); ++b) {
      this->insert(*b);
    }
  }

  class MapIterator {
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
    MapIterator(Node* r, Node* n, Node* tn) {
      ptr_ = n;
      root_ = r;
      TNULL_ = tn;
      // fake = std::make_pair((key_type)0, (mapped_type)0);
    }

    void operator++() {
      if (root_ != TNULL_ && ptr_ == maximum(root_))
        ptr_ = TNULL_;
      else
        ptr_ = successor(ptr_);
    }

    void operator--() {
      if (root_ != TNULL_ && ptr_ == TNULL_) {
        ptr_ = maximum(root_);
      } else {
        ptr_ = predecessor(ptr_);
      }
    }

    value_type& operator*() {
      if (ptr_ == TNULL_ && root_ != TNULL_) {
        std::cout << "1 case\n";
        std::pair<key_type, mapped_type> p =
            std::make_pair(TNULL_->key, TNULL_->data);
        std::pair<key_type, mapped_type>& r = p;
        return r;
      }
      if (ptr_ == nullptr) {
        std::cout << "2 case\n";
        std::pair<key_type, mapped_type> p =
            std::make_pair(*(new key_type()), *(new mapped_type()));
        std::pair<key_type, mapped_type>& r = p;
        return r;
      }
      fake.first = ptr_->key;
      fake.second = ptr_->data;
      value_type& r = fake;
      return r;
    }

    bool operator==(const MapIterator& other) { return ptr_ == other.ptr_; }

    bool operator!=(const MapIterator& other) { return ptr_ != other.ptr_; }
  };

  iterator begin() {
    // std::cout << "\nbegin state " << (root == TNULL) << (root == nullptr) <<
    // (TNULL == nullptr) << "\n"; minimum(root);

    // return MapIterator(nullptr, nullptr, nullptr);
    return MapIterator(root, root == TNULL ? TNULL : minimum(root), TNULL);
  }

  iterator end() {
    if (root == TNULL) return MapIterator(root, TNULL, TNULL);
    MapIterator r(root, TNULL, TNULL);
    TNULL->key = maximum(root)->key;
    TNULL->data = (mapped_type)0;
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

#endif