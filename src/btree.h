#ifndef srcbtree
#define srcbtree
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace s21 {

template <class K, class M, class Node, bool MULTI>
class iterator_btree;

template <class K, class M, class Node, bool MULTI>
class const_iterator_btree;

enum Color { RED, BLACK };

template <class K, class M, class Node, bool MULTI>
class btree {
 public:
  using key_type = K;
  using value_type = typename Node::value_type;
  using reference = K&;
  using const_reference = const K&;
  using iterator = iterator_btree<K, M, Node, MULTI>;
  using const_iterator = const_iterator_btree<K, M, Node, MULTI>;
  using size_type = size_t;

 protected:
  Node* root_;
  Node* front_;
  Node* back_;
  size_t size_;

 public:
  // CONSTRUCTORS

  btree();
  btree(std::initializer_list<value_type> const& keys);
  btree(const btree& ms);
  btree(btree&& ms);
  ~btree();

  bool is_equal(K a, K b) const {
    if (a == b) {
      return true;
    }
    return false;
  }
  bool is_equal(std::pair<K, M> a, std::pair<K, M> b) const {
    if (a.first == b.first) {
      return true;
    }
    return false;
  }

  // OPERATORS
  btree& operator=(std::initializer_list<value_type> const& keys);
  btree& operator=(const btree& ms);
  btree& operator=(btree&& other);

  // METHODS
  void clear();
  iterator begin() {
    iterator ret(front_);
    return ret;
  }
  iterator end() {
    iterator ret(back_);
    return ret;
  }
  const_iterator begin() const {
    const_iterator ret(front_);
    return ret;
  }
  const_iterator end() const {
    const_iterator ret(back_);
    return ret;
  }

  void erase(iterator pos) {
    if (pos.ptr_ == front_) {
      auto temp = this->begin();
      temp++;
      front_ = temp.ptr_;
    }
    // Case 1: node is leaf -> both children are nullptr.
    if (pos->left == nullptr && pos->right == nullptr) {
      replace_node_(pos.ptr_, pos->right);
      if (pos.ptr_->color == BLACK) {
        fixDoubleBlack(nullptr, pos.ptr_->parent);
      }
    } else
        // Case2: one kid is nullptr
        if (pos->left == nullptr) {
      replace_node_(pos.ptr_, pos.ptr_->right);
      if (pos.ptr_->color == BLACK && pos.ptr_->right->color == BLACK) {
        fixDoubleBlack(pos->right);
      } else {
        pos.ptr_->right->color = BLACK;
      }
    } else if (pos->right == nullptr) {
      replace_node_(pos.ptr_, pos.ptr_->left);
      if (pos.ptr_->color == BLACK && pos.ptr_->left->color == BLACK) {
        fixDoubleBlack(pos->left);
      } else {
        pos.ptr_->left->color = BLACK;
      }
    } else {  // case 3 - both kids exist
      Node* successor = pos->right;
      while (successor->left != nullptr) {
        successor = successor->left;
      }
      if (successor->parent == pos.ptr_) {
        replace_node_(pos.ptr_, successor);
        if (successor->right != nullptr) {
          if (successor->right->color == BLACK && successor->color == BLACK) {
            fixDoubleBlack(successor->right);
          } else {
            successor->right->color = BLACK;
            successor->color = pos.ptr_->color;
          }
        } else {
          successor->color = pos.ptr_->color;
          fixDoubleBlack(nullptr, successor);
        }
      } else {
        if (successor->right != nullptr) {
          successor->right->parent = successor->parent;
        }
        if (successor == successor->parent->left) {
          successor->parent->left = successor->right;
        } else {
          successor->parent->right = successor->right;
        }
        if (successor->right != nullptr) {
          if (successor->color == BLACK && successor->right->color == BLACK) {
            fixDoubleBlack(successor->right);
          } else {
            successor->right->color = BLACK;
          }
        } else {
          if (successor->color == BLACK) {
            fixDoubleBlack(nullptr, successor->parent);
            successor->color = pos.ptr_->color;
          }
        }
        replace_node_(pos.ptr_, successor);
      }
    }
    delete pos.ptr_;
  }
  void merge(btree& other) {
    size_ = size_ + other.size_;
    Node* n = merge_(root_, other.root_);
    other.root_ = nullptr;
    other.size_ = 0;
    root_ = n;
    front_ = findFront(root_);
  }
  size_type size() const { return size_; }
  size_type max_size() const { return std::numeric_limits<value_type>::max(); }
  bool empty() const {
    if (root_ == nullptr) {
      return true;
    } else {
      return false;
    }
  }
  void swap(btree& other) {
    btree<K, M, Node, MULTI> temp = other;
    other = *this;
    *this = temp;
  }
  size_type count(const value_type& key) const { return count_(key, root_); }
  iterator find(const value_type& value) {
    Node* ret = find_(value, root_);
    return iterator(ret);
  }
  const_iterator find(const value_type& value) const {
    Node* ret = find_(value, root_);
    return const_iterator(ret);
  }
  iterator lower_bound(const value_type& value) {
    Node* ret = find_(value, root_);
    Node* ans = ret;
    while (ans->left != nullptr && (ans = find_(value, ans->left)) != nullptr) {
      ret = ans;
    }
    return iterator(ret);
  }
  iterator upper_bound(const value_type& value) {
    Node* ret = find_(value, root_);
    Node* ans = ret;
    while (ans->right != nullptr &&
           (ans = find_(value, ans->right)) != nullptr) {
      ret = ans;
    }
    return iterator(ret);
  }
  const_iterator lower_bound(const value_type& value) const {
    Node* ret = find_(value, root_);
    Node* ans = ret;
    while (ans->left != nullptr && (ans = find_(value, ans->left)) != nullptr) {
      ret = ans;
    }
    return const_iterator(ret);
  }
  const_iterator upper_bound(const value_type& value) const {
    Node* ret = find_(value, root_);
    Node* ans = ret;
    while (ans->right != nullptr &&
           (ans = find_(value, ans->right)) != nullptr) {
      ret = ans;
    }
    return const_iterator(ret);
  }

  template <class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    value_type val(args...);
    return insert_iter(val);
  }

 protected:
  void show() { display(root_); }
  // HELPER FUNCTIONS
  std::pair<iterator, bool> insert_iter(const value_type& key) {
    size_t increment = 1;
    size_ = increment + size_;
    Node* pt = new Node(key, nullptr);
    Node* retret = pt;
    if (root_ == nullptr) {
      auto temp = this->BSTInsert(root_, pt);
      if (!temp.second) {
        return std::make_pair(iterator(temp.first), temp.second);
      }
      root_ = temp.first;

      front_ = root_;
    } else {
      auto temp = this->BSTInsert(root_, pt);
      if (!temp.second) {
        return std::make_pair(iterator(temp.first), temp.second);
      }
      root_ = temp.first;
    }
    this->fixViolation(root_, pt);
    return std::make_pair(iterator(retret), true);
  }
  std::pair<Node*, bool> BSTInsert(Node*& root, Node* pt) {
    if (root == nullptr) return std::make_pair(pt, true);
    if (pt->key <= root->key) {
      if (is_equal(pt->key, root->key) && !MULTI) {
        delete pt;
        size_--;
        return std::make_pair(root, false);
      }
      if (root == front_) {
        front_ = pt;
      }
      auto temp = BSTInsert(root->left, pt);
      if (!temp.second) {
        return std::make_pair(temp.first, temp.second);
      }
      root->left = temp.first;
      root->left->parent = root;
    } else if (pt->key > root->key) {
      if (is_equal(pt->key, root->key) && !MULTI) {
        delete pt;
        size_--;
        return std::make_pair(root, false);
      }
      auto temp = BSTInsert(root->right, pt);
      if (!temp.second) {
        return std::make_pair(temp.first, temp.second);
      }
      root->right = temp.first;
      root->right->parent = root;
    }
    return std::make_pair(root, true);
  }
  void clear_(Node* r);
  Node* findFront(Node* root_);
  void CopyTree(Node*& first_root, Node* second_root);
  void display(Node* my) {
    if (my != nullptr) {
      display(my->left);
      std::cout << my->key << ' ';
      display(my->right);
    }
  }

  int depth(Node* n) {
    if (n == nullptr) {
      return 0;
    }
    int depthr = 0, depthl = 0;
    depthr += depth(n->right);
    depthl += depth(n->left);
    return (std::max(depthr, depthl) + 1);
  }
  int cdepth() { return depth(root_); }

  void rotateLeft(Node*& root, Node*& pt) {
    Node* pt_right = pt->right;
    pt->right = pt_right->left;
    if (pt->right != nullptr) pt->right->parent = pt;
    pt_right->parent = pt->parent;
    if (pt->parent == nullptr)
      root = pt_right;
    else if (pt == pt->parent->left)
      pt->parent->left = pt_right;
    else
      pt->parent->right = pt_right;
    pt_right->left = pt;
    pt->parent = pt_right;
  }
  void rotateRight(Node*& root, Node*& pt) {
    Node* pt_left = pt->left;
    pt->left = pt_left->right;
    if (pt->left != nullptr) pt->left->parent = pt;
    pt_left->parent = pt->parent;
    if (pt->parent == nullptr)
      root = pt_left;
    else if (pt == pt->parent->left)
      pt->parent->left = pt_left;
    else
      pt->parent->right = pt_left;
    pt_left->right = pt;
    pt->parent = pt_left;
  }
  void fixViolation(Node*& root, Node*& pt) {
    Node* parent_pt = nullptr;
    Node* grand_parent_pt = nullptr;
    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
      parent_pt = pt->parent;
      grand_parent_pt = pt->parent->parent;
      if (parent_pt == grand_parent_pt->left) {
        Node* uncle_pt = grand_parent_pt->right;
        if (uncle_pt != nullptr && uncle_pt->color == RED) {
          grand_parent_pt->color = RED;
          parent_pt->color = BLACK;
          uncle_pt->color = BLACK;
          pt = grand_parent_pt;
        } else {
          if (pt == parent_pt->right) {
            this->rotateLeft(root, parent_pt);
            pt = parent_pt;
            parent_pt = pt->parent;
          }
          this->rotateRight(root, grand_parent_pt);
          std::swap(parent_pt->color, grand_parent_pt->color);
          pt = parent_pt;
        }
      } else {
        Node* uncle_pt = grand_parent_pt->left;
        if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
          grand_parent_pt->color = RED;
          parent_pt->color = BLACK;
          uncle_pt->color = BLACK;
          pt = grand_parent_pt;
        } else {
          if (pt == parent_pt->left) {
            this->rotateRight(root, parent_pt);
            pt = parent_pt;
            parent_pt = pt->parent;
          }
          this->rotateLeft(root, grand_parent_pt);
          std::swap(parent_pt->color, grand_parent_pt->color);
          pt = parent_pt;
        }
      }
    }
    root->color = BLACK;
  }
  void replace_node_(Node* old_node, Node* new_node) {
    if (old_node->parent != nullptr) {
      if (old_node == old_node->parent->left) {
        old_node->parent->left = new_node;
      } else {
        old_node->parent->right = new_node;
      }
    }
    if (new_node != nullptr) {
      new_node->parent = old_node->parent;
      if (old_node->left != new_node) {
        new_node->left = old_node->left;
        if (old_node->left != nullptr) {
          old_node->left->parent = new_node;
        }
      }
      if (old_node->right != new_node) {
        new_node->right = old_node->right;
        if (old_node->right != nullptr) {
          old_node->right->parent = new_node;
        }
      }
    }
    if (old_node == root_) {
      root_ = new_node;
    }
  }
  Node* get_sibling(Node* u) {
    // Assume u no root
    if (u == u->parent->left) {
      return u->parent->right;
    } else {
      return u->parent->left;
    }
  }
  Node* get_red_kid(Node* n) {
    Node* ret = nullptr;
    if (n->left->color == RED) {
      ret = n->left;
    } else if (n->right->color == RED) {
      ret = n->right;
    }
    return ret;
  }
  void fix_tree_after_erase(Node* u, Node* v) {
    // Simple case - either u or v is RED
    if (v->color == RED || (u != nullptr && u->color == RED)) {
      u->color = BLACK;
    } else {
      fixDoubleBlack(u);
    }
  }
  bool is_on_left(Node* n) {
    if (n->parent == nullptr) {
      return false;
    } else if (n->parent->left == n) {
      return true;
    } else {
      return false;
    }
  }
  bool has_red_kid(Node* n) {
    bool ret = false;
    if (n->left != nullptr && n->left->color == RED) {
      ret = true;
    }
    if (n->right != nullptr && n->right->color == RED) {
      ret = true;
    }
    return ret;
  }
  void fixDoubleBlack(Node* x, Node* y = nullptr) {
    if (x == root_)
      // Reached root
      return;

    Node *sibling, *parent;
    if (y == nullptr) {
      sibling = get_sibling(x);
      parent = x->parent;
    } else {
      parent = y;
      if (y->left != nullptr) {
        sibling = y->left;
      } else {
        sibling = y->right;
      }
    }
    if (sibling == NULL) {
      // No sibiling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        // Sibling red
        parent->color = RED;
        sibling->color = BLACK;
        if (is_on_left(sibling)) {
          // left case
          rotateRight(root_, parent);
        } else {
          // right case
          rotateLeft(root_, parent);
        }
        if (x != nullptr) {
          fixDoubleBlack(x);
        } else {
          fixDoubleBlack(x, parent);
        }
      } else {
        // Sibling black
        if (has_red_kid(sibling)) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (is_on_left(sibling)) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rotateRight(root_, parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rotateRight(root_, sibling);
              rotateLeft(root_, parent);
            }
          } else {
            if (is_on_left(sibling)) {
              // left right
              sibling->right->color = parent->color;
              rotateLeft(root_, sibling);
              rotateRight(root_, parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              rotateLeft(root_, parent);
            }
          }
          parent->color = BLACK;
        } else {
          // 2 black children
          sibling->color = RED;
          if (parent->color == BLACK) {
            fixDoubleBlack(parent);
          } else {
            parent->color = BLACK;
          }
        }
      }
    }
  }
  Node*& getRoot() { return root_; }
  int isRedBlack(Node* n) {
    if (n == nullptr) {
      return 1;
    }
    int a = isRedBlack(n->right);
    int b = isRedBlack(n->left);
    if (a == -1 || b == -1 || a != b) {
      return -1;
    }
    if (n->color == BLACK) {
      return a + 1;
    } else {
      return a;
    }
  }
  int getTreeHeight(Node* root) {
    if (root == nullptr) {
      return 1;
    } else if (root->color == BLACK) {
      return getTreeHeight(root->right) + 1;
    } else {
      return getTreeHeight(root->right);
    }
  }
  Node* joinSimple(Node* tl, value_type k, Node* tr, Color c) {
    Node* kk = new Node(k, nullptr);
    if (tl != nullptr) tl->parent = kk;
    if (tr != nullptr) tr->parent = kk;
    kk->left = tl;
    kk->right = tr;
    kk->color = c;
    // delete k;
    return kk;
  }
  Color getColor(Node* n) {
    if (n != nullptr) {
      return n->color;
    } else {
      return BLACK;
    }
  }
  Node* joinRightRB(Node* tl, value_type k, Node* tr) {
    int tl_height = getTreeHeight(tl);
    int tr_height = getTreeHeight(tr);
    if (getColor(tl) == BLACK && tl_height == tr_height) {
      Node* ret = joinSimple(tl, k, tr, RED);
      return ret;
    }
    Node* t = joinSimple(tl->left, tl->key, joinRightRB(tl->right, k, tr),
                         getColor(tl));
    t->parent = tl->parent;

    if (getColor(tl) == BLACK && getColor(t->right) == RED &&
        getColor(t->right->right) == RED) {
      t->right->right->color = BLACK;
      rotateLeft(root_, t);
      delete tl;
      return t->parent;
    }
    delete tl;
    return t;
  }
  Node* joinLeftRB(Node* tl, value_type k, Node* tr) {
    int tl_height = getTreeHeight(tl);
    int tr_height = getTreeHeight(tr);
    if (getColor(tr) == BLACK && tl_height == tr_height) {
      Node* ret = joinSimple(tl, k, tr, RED);
      return ret;
    }
    Node* t = joinSimple(joinLeftRB(tl, k, tr->left), tr->key, tr->right,
                         getColor(tr));
    t->parent = tr->parent;

    if (getColor(tr) == BLACK && getColor(t->left) == RED &&
        getColor(t->left->left) == RED) {
      t->left->left->color = BLACK;
      rotateRight(root_, t);
      delete tr;
      return t->parent;
    }
    delete tr;
    return t;
  }
  Node* join(Node* tl, value_type k, Node* tr) {
    int tl_height = getTreeHeight(tl);
    int tr_height = getTreeHeight(tr);
    if (tl_height > tr_height) {
      Node* t = joinRightRB(tl, k, tr);
      if (getColor(t) == RED && getColor(t->right) == RED) {
        t->color = BLACK;
      }
      return t;
    }
    if (tr_height > tl_height) {
      Node* t = joinLeftRB(tl, k, tr);
      if (getColor(t) == RED && getColor(t->left) == RED) {
        t->color = BLACK;
      }
      return t;
    }
    if (getColor(tl) == BLACK && getColor(tr) == BLACK) {
      return joinSimple(tl, k, tr, RED);
    }
    return joinSimple(tl, k, tr, BLACK);
  }
  struct triplet {
    Node* first;
    bool second;
    Node* third;
    triplet(Node* a, bool b, Node* c) : first(a), second(b), third(c) {}
  };
  triplet split(Node*& t, value_type k) {
    if (t == nullptr) {
      return triplet(nullptr, false, nullptr);
    }
    if (k == t->key) {
      triplet a = triplet(t->left, true, t->right);
      return a;
    }
    if (k <= t->key) {
      triplet a = split(t->left, k);
      triplet ret = triplet(a.first, a.second, join(a.third, t->key, t->right));
      size_--;
      delete t;
      return ret;
    }
    triplet a = split(t->right, k);
    triplet ret = triplet(join(t->left, t->key, a.first), a.second, a.third);
    delete t;
    return ret;
  }
  Node* merge_(Node*& t1, Node*& t2) {
    if (t1 == nullptr) {
      return t2;
    }
    if (t2 == nullptr) {
      return t1;
    }
    triplet a = split(t1, t2->key);
    Node* tl = merge_(a.first, t2->left);
    Node* tr = merge_(a.third, t2->right);
    Node* ret = join(tl, t2->key, tr);

    delete t2;
    return ret;
  }
  size_type count_(value_type key, Node* r) const {
    if (r == nullptr) {
      return 0;
    }
    size_type ret = 0;
    if (r->key == key) {
      ret += 1;
      ret += count_(key, r->left);
      ret += count_(key, r->right);
    } else if (r->key < key) {
      ret += count_(key, r->right);
    } else if (r->key > key) {
      ret += count_(key, r->left);
    }
    return ret;
  }
  Node* find_(const value_type& key, Node* r) const {
    if (r == nullptr) {
      return nullptr;
    }
    if (is_equal(r->key, key)) {
      return r;
    }
    if (r->key > key) {
      return find_(key, r->left);
    } else {
      return find_(key, r->right);
    }
  }
};
template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>::btree() {
  root_ = nullptr;
  front_ = back_ = nullptr;
  size_ = 0;
}
template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>::btree(
    std::initializer_list<value_type> const& values) {
  root_ = nullptr;
  front_ = back_ = nullptr;
  size_ = 0;
  for (auto i : values) {
    this->insert_iter(i);
  }
}

template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>::btree(const btree& ms) {
  back_ = nullptr;
  if (ms.root_ != nullptr) {
    this->root_ = new Node;
    CopyTree(root_, ms.root_);
    front_ = findFront(root_);
    size_ = ms.size_;
  } else {
    root_ = front_ = nullptr;
    size_ = 0;
  }
}

template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>::btree(btree&& ms)
    : root_(ms.root_), front_(ms.front_), size_(ms.size_) {
  back_ = nullptr;
  ms.root_ = nullptr;
  ms.front_ = nullptr;
  ms.size_ = 0;
}

template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>::~btree() {
  this->clear();
}

template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>& btree<K, M, Node, MULTI>::operator=(
    std::initializer_list<value_type> const& keys) {
  if (root_ != nullptr) {
    this->clear();
  }
  size_ = 0;
  root_ = nullptr;
  front_ = back_ = nullptr;
  for (auto i : keys) {
    this->insert_iter(i);
  }
  return *this;
}

template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>& btree<K, M, Node, MULTI>::operator=(const btree& ms) {
  if (root_ == ms.root_) {
    return *this;
  }
  if (root_ != nullptr) {
    this->clear();
  }
  back_ = nullptr;
  if (ms.root_ != nullptr) {
    this->root_ = new Node;
    CopyTree(root_, ms.root_);
    front_ = findFront(root_);
    size_ = ms.size_;
  } else {
    root_ = front_ = nullptr;
    size_ = 0;
  }
  front_ = findFront(root_);
  return *this;
}

template <class K, class M, class Node, bool MULTI>
btree<K, M, Node, MULTI>& btree<K, M, Node, MULTI>::operator=(btree&& other) {
  if (root_ == other.root_) {
    return *this;
  }
  if (root_ != nullptr) {
    this->clear();
  }
  root_ = other.root_;
  front_ = other.front_;
  size_ = other.size_;
  back_ = nullptr;
  other.root_ = nullptr;
  other.front_ = nullptr;
  other.size_ = 0;
  return *this;
}

// METHODS
template <class K, class M, class Node, bool MULTI>
void btree<K, M, Node, MULTI>::clear() {
  this->size_ = 0;
  clear_(root_);
  root_ = front_ = back_ = nullptr;
}

// HELPER FUNCTIONS
template <class K, class M, class Node, bool MULTI>
void btree<K, M, Node, MULTI>::clear_(Node* r) {
  if (r == nullptr) {
    return;
  }
  if (r->left != nullptr) {
    clear_(r->left);
  }
  if (r->right != nullptr) {
    clear_(r->right);
  }
  if (r->parent != nullptr) {
    Node* p = r->parent;
    if (r == p->left) {
      if (r == front_) {
        front_ = p;
      }
      p->left = nullptr;
    } else {
      p->right = nullptr;
    }
  }
  delete r;
}

template <class K, class M, class Node, bool MULTI>
Node* btree<K, M, Node, MULTI>::findFront(Node* root_) {
  if (root_ == nullptr) {
    return nullptr;
  }
  if (root_->left == nullptr) {
    return root_;
  } else {
    return findFront(root_->left);
  }
}

template <class K, class M, class Node, bool MULTI>
void btree<K, M, Node, MULTI>::CopyTree(Node*& first_root, Node* second_root) {
  first_root->key = second_root->key;
  first_root->color = second_root->color;
  if (second_root->left != nullptr) {
    Node* l = new Node;
    l->parent = first_root;
    first_root->left = l;
    CopyTree(l, second_root->left);
  }
  if (second_root->right != nullptr) {
    Node* r = new Node;
    r->parent = first_root;
    first_root->right = r;
    CopyTree(r, second_root->right);
  }
}

template <class K, class M, class Node, bool MULTI>
class iterator_btree {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = typename Node::value_type;
  using pointer = typename Node::value_type*;    // or also value_type*
  using reference = typename Node::value_type&;  // or also value_type&

 public:
  friend class btree<K, M, Node, MULTI>;

  iterator_btree() : ptr_(nullptr){};
  iterator_btree(Node* ptr) : ptr_(ptr){};
  value_type& operator*() const { return (ptr_->key); }
  Node* operator->() { return ptr_; }
  // Prefix increment
  iterator_btree& operator++() {
    if (ptr_->right != nullptr) {
      ptr_ = ptr_->right;
      while (ptr_->left != nullptr) {
        ptr_ = ptr_->left;
      }
    } else {
      Node* temp = ptr_->parent;
      while (ptr_ == temp->right && temp->parent != nullptr) {
        ptr_ = temp;
        temp = temp->parent;
      }
      if (ptr_ == temp->left) {
        ptr_ = temp;
      } else {
        ptr_ = nullptr;
      }
    }
    return *this;
  }

  iterator_btree& operator--() {
    if (ptr_->left != nullptr) {
      ptr_ = ptr_->left;
      while (ptr_->right != nullptr) {
        ptr_ = ptr_->right;
      }
    } else {
      Node* temp = ptr_->parent;
      while (ptr_ == temp->left && temp->parent != nullptr) {
        ptr_ = temp;
        temp = temp->parent;
      }
      if (ptr_ == temp->right) {
        ptr_ = temp;
      } else {
        ptr_ = nullptr;
      }
    }
    return *this;
  }

  // Postfix increment
  iterator_btree operator++(int) {
    iterator_btree tmp = *this;
    ++(*this);
    return tmp;
  }
  iterator_btree operator--(int) {
    iterator_btree tmp = *this;
    --(*this);
    return tmp;
  }

  friend class const_iterator_btree<K, M, Node, MULTI>;

  operator const_iterator_btree<K, M, Node, MULTI>() const {
    return const_iterator_btree<K, M, Node, MULTI>(ptr_);
  }
  friend bool operator==(const iterator_btree& a, const iterator_btree& b) {
    return a.ptr_ == b.ptr_;
  };
  friend bool operator!=(const iterator_btree& a, const iterator_btree& b) {
    return a.ptr_ != b.ptr_;
  };

 private:
  Node* ptr_;
};

template <class K, class M, class Node, bool MULTI>
class const_iterator_btree {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = typename Node::value_type;
  using pointer = typename Node::value_type*;    // or also value_type*
  using reference = typename Node::value_type&;  // or also value_type&

 public:
  friend class btree<K, M, Node, MULTI>;

  const_iterator_btree() : ptr_(nullptr){};
  const_iterator_btree(Node* ptr) : ptr_(ptr){};
  value_type operator*() const { return (ptr_->key); }
  Node* operator->() const { return ptr_; }
  // Prefix increment
  const_iterator_btree& operator++() {
    // m_ptr++; return *this;
    if (ptr_->right != nullptr) {
      ptr_ = ptr_->right;
      while (ptr_->left != nullptr) {
        ptr_ = ptr_->left;
      }
    } else {
      Node* temp = ptr_->parent;
      while (ptr_ == temp->right && temp->parent != nullptr) {
        ptr_ = temp;
        temp = temp->parent;
      }
      if (ptr_ == temp->left) {
        ptr_ = temp;
      } else {
        ptr_ = nullptr;
      }
    }
    return *this;
  }

  const_iterator_btree& operator--() {
    if (ptr_->left != nullptr) {
    } else {
      K* temp = ptr_->parent;
    }
  }

  // Postfix increment
  const_iterator_btree operator++(int) {
    const_iterator_btree tmp = *this;
    ++(*this);
    return tmp;
  }
  friend class iterator_btree<K, M, Node, MULTI>;

  operator iterator_btree<K, M, Node, MULTI>() const {
    return iterator_btree<K, M, Node, MULTI>(ptr_);
  }
  friend bool operator==(const const_iterator_btree& a,
                         const const_iterator_btree& b) {
    return a.ptr_ == b.ptr_;
  };
  friend bool operator!=(const const_iterator_btree& a,
                         const const_iterator_btree& b) {
    return a.ptr_ != b.ptr_;
  };

 private:
  Node* ptr_;
};
}  // namespace s21
#endif