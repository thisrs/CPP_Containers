
#ifndef srcmultiset
#define srcmultiset
#include "btree.h"
#include "set.h"

namespace s21 {

template <class K>
class multiset : public btree<K, K, s21::TreeNode<K>, true> {
  using iterator = iterator_btree<K, K, TreeNode<K>, true>;

  using btree<K, K, TreeNode<K>, true>::btree;
  using value_type = K;

 public:
  iterator insert(const value_type &key) {
    return (this->insert_iter(key)).first;
  }
};

}  // namespace s21
#endif