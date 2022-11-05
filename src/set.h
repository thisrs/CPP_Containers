
#ifndef srcset
#define srcset
#include "btree.h"
namespace s21 {

template <class K>
struct TreeNode {
  using value_type = K;

 public:
  Color color;
  K key;
  TreeNode<K>* left = nullptr;
  TreeNode<K>* right = nullptr;
  TreeNode<K>* parent;
  TreeNode() {
    key = 0;
    parent = left = right = nullptr;
    color = BLACK;
  }
  TreeNode(K k, TreeNode<K>* p) {
    color = RED;
    key = k;
    parent = p;
  }
};

template <class K>
class set : public btree<K, K, TreeNode<K>, false> {
  using key_type = K;
  using value_type = typename TreeNode<K>::value_type;
  using reference = K&;
  using const_reference = const K&;
  using iterator = iterator_btree<K, K, TreeNode<K>, false>;
  using const_iterator = const_iterator_btree<K, K, TreeNode<K>, false>;
  using size_type = size_t;
  using Node = TreeNode<K>;
  using btree<K, K, TreeNode<K>, false>::btree;

 public:
  std::pair<iterator, bool> insert(const value_type& k) {
    std::pair<iterator, bool> a;
    a = this->insert_iter(k);
    return a;
  }

  bool contains(const key_type& k) {
    Node* f = btree<K, K, TreeNode<K>, false>::find_(k, this->root_);
    if (f == nullptr) {
      return false;
    } else {
      return true;
    }
  }
};

}  // namespace s21
#endif