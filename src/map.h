
#ifndef srcmap
#define srcmap

#include <stdexcept>

#include "btree.h"
namespace s21 {

template <class K, class M>
struct MapNode {
  using value_type = std::pair<K, M>;

 public:
  Color color;
  std::pair<K, M> key;
  MapNode<K, M>* left = nullptr;
  MapNode<K, M>* right = nullptr;
  MapNode<K, M>* parent;
  MapNode() {
    key = 0;
    parent = left = right = nullptr;
    color = BLACK;
  }
  MapNode(const value_type k, MapNode<K, M>* p) {
    color = RED;
    parent = p;
    key = k;
  }
};

template <class K, class V>
class map : public btree<K, V, MapNode<K, V>, false> {
  using btree<K, V, MapNode<K, V>, false>::btree;
  using Node = MapNode<K, V>;
  using mapped_type = V;
  using value_type = typename std::pair<K, V>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = iterator_btree<K, K, MapNode<K, V>, false>;
  using const_iterator = const_iterator_btree<K, V, MapNode<K, V>, false>;
  using size_type = size_t;

 public:
  V& at(const K& key) {
    Node* found = find_(key, this->root_);
    if (found == nullptr) {
      throw std::out_of_range("index out of range");
    }
    return found->key.second;
  }
  const V& at(const K& key) const {
    Node* found = find_(key, this->root_);
    if (found == nullptr) {
      throw std::out_of_range("index out of range");
    }
    return found->key.second;
  }
  V& operator[](const K& key) {
    Node* found = find_(key, this->root_);
    if (found == nullptr) {
      V dummy = 0;
      auto i = this->insert_iter(std::pair<K, V>(key, dummy));
      return (*(i.first)).second;
    }
    return found->key.second;
  }
  std::pair<iterator, bool> insert(const value_type& value) {
    return this->insert_iter(value);
  }

  std::pair<iterator, bool> insert(const K& key, const V& obj) {
    std::pair<K, V> value(key, obj);
    return this->insert_iter(value);
  }

  std::pair<iterator, bool> insert_or_assign(const K& key, const V& obj) {
    std::pair<K, V> value(key, obj);
    std::pair<iterator, bool> a;
    a = this->insert_iter(value);
    if (a.second == false) {
      (*a.first).second = obj;
      a.second = true;
    }
    return a;
  }

 private:
  Node* find_(const K& key, Node* r) const {
    if (r == nullptr) {
      return nullptr;
    }
    if (r->key.first == key) {
      return r;
    }
    if (r->key.first > key) {
      return find_(key, r->left);
    } else {
      return find_(key, r->right);
    }
  }
};
}  // namespace s21
#endif