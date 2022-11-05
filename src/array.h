#ifndef SRC_ARRAY
#define SRC_ARRAY

#include <map>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

namespace s21 {

using std::cout;
using std::endl;

template <class T, std::size_t N = 0>
class array {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = T*;
  using const_iterator = const T*;

 private:
  value_type front_[N];

 public:
  array(){};
  array(std::initializer_list<value_type> const& items) {
    // std::copy(items.begin(), items.end(), this -> begin());  - too slow!
    pointer ptr = front_;
    for (auto i : items) {
      *ptr = i;
      ptr++;
    }
  }
  // By following rule of five, no other constructors needed.

  // Methods for access of elements:
  reference operator[](size_type pos) { return front_[pos]; }
  const_reference operator[](size_type pos) const { return *(front_ + pos); }
  const_reference front() const { return *front_; }
  const_reference back() const { return *(front_ + N - 1); }

  reference at(size_type pos) {
    if (pos < 0 || pos > N - 1) {
      throw std::out_of_range("index out of range");
    }
    return front_[pos];
  }

  const_reference at(size_type pos) const {
    if (pos < 0 || pos > N - 1) {
      throw std::out_of_range("index out of range");
    }
    return front_[pos];
  }

  pointer data() { return front_; }
  const_pointer data() const { return front_; }

  // Iterators
  iterator begin() { return front_; }
  iterator end() { return front_ + N; }
  const_iterator begin() const { return front_; }
  const_iterator end() const { return front_ + N; }

  // Capacity
  [[nodiscard]] bool empty() const {
    if (N == 0) {
      return true;
    }
    return false;
  }
  [[nodiscard]] size_type size() const { return N; }
  [[nodiscard]] size_type max_size() const { return N; }

  // Modifiers
  void swap(array& other) {
    if (this->front_ == other.front_) {
      return;
    }
    std::swap_ranges(this->begin(), this->end(), other.begin());
  }

  void fill(const_reference value) {
    for (auto i : *this) {
      i = value;
    }
  }
};

}  // namespace s21

#endif  // SRC_ARRAY