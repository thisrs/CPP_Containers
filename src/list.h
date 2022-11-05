#ifndef SEC
#define SEC

#include <cstddef>   // For std::ptrdiff_t
#include <iterator>  // For std::forward_iterator_tag
#include <list>
namespace s21 {

template <class T>
struct ListNode {
  ListNode<T>* prev;
  ListNode<T>* next;
  T data;
  ListNode() {
    prev = nullptr;
    next = nullptr;
  }
};

template <class T>
class ListIterator;
template <class T>
class ConstListIterator;

template <class T>
class list {
  using value_type = T;
  using reference = T&;  // or also value_type&
  using const_reference = const T&;
  using pointer = T*;
  using Node = ListNode<value_type>;
  using iterator = ListIterator<value_type>;
  using const_iterator = ConstListIterator<value_type>;
  using size_type = unsigned int;

  // private members:
  Node* _front;
  Node* _back;
  size_type _size;

 public:
  // CONSTRUCTORS:
  list() {
    _front = _back = new Node;  // empty node for end() method
    _size = 0;
  }
  ~list() {
    this->clear();
    delete _back;
    _back = _front = nullptr;
  }
  explicit list(size_type n) {
    _front = _back = new Node;
    _size = 0;
    while (n--) {
      value_type value = value_type();
      this->push_back(value);
    }
  }
  list(std::initializer_list<value_type> const& items) {
    _front = _back = new Node;
    _size = 0;
    for (auto i : items) {
      this->push_back(i);
    }
  }
  list(const list& l) {
    _front = _back = new Node;
    _size = 0;
    for (auto i : l) {
      this->push_back(i);
    }
  }
  list(list&& l) noexcept {
    _back = l._back;
    _front = l._front;
    _size = l._size;
    l._front = nullptr;
    l._back = nullptr;
    l._size = 0;
  }
  list& operator=(const list& l) {
    if (this == l) return *this;
    if (_front != nullptr) {
      this->clear();
      delete _back;
    }
    _front = _back = new Node;
    _size = 0;
    for (auto i : l) {
      this->push_back(i);
    }
    return *this;
  }
  list& operator=(list&& l) noexcept {
    _front = l._front;
    _back = l._back;
    _size = l._size;
    l._front = nullptr;
    l._back = nullptr;
    l._size = 0;
    return *this;
  }
  const_iterator begin() const { return const_iterator(_front); }
  const_iterator end() const { return const_iterator(_back); }
  iterator begin() { return iterator(_front); }
  iterator end() { return iterator(_back); }

  // CAPACITY
  bool empty() { return !(bool)_size; }
  size_type size() { return _size; }
  size_type max_size() { return std::numeric_limits<T>::max(); }

  // MODIFIERS

  void clear() {
    iterator i = this->begin();
    while (i._node != _back) {
      ++i;
      delete i->prev;
    }
    _front = _back;
  }
  void erase(iterator pos) {
    _size--;
    if (pos._node != _back) {
      pos->next->prev = pos->prev;
    }
    if (pos._node != _front) {
      pos->prev->next = pos->next;
    } else {
      _front = pos->next;
    }
    delete pos._node;
  }
  iterator insert(iterator pos, const_reference value) {
    ++_size;
    Node* node = new Node;
    node->data = value;
    node->next = pos._node;

    if (pos._node == _front) {
      _front = node;
    } else {
      pos->prev->next = node;
      node->prev = pos->prev;
    }
    pos->prev = node;
    return iterator(node);
  }
  void push_back(const_reference value) {
    this->insert(iterator(_back), value);
  }
  void push_front(const_reference value) {
    this->insert(iterator(_front), value);
  }
  void pop_back() { this->erase(--iterator(_back)); }
  void pop_front() { this->erase(iterator(_front)); }
  void swap(list& other) {
    list tmp(other);
    other = *this;
    *this = tmp;
  }
  void merge(list& other) {
    iterator other_iterator = other.begin();
    for (auto i = this->begin(); i != this->end(); i++) {
      while (other_iterator != other.end() && *other_iterator < *i) {
        this->insert(i, *other_iterator);
        other.erase(other_iterator);
        other_iterator = other.begin();
      }
    }
    while (other.size() != 0) {
      this->insert(this->end(), *(other.begin()));
      other.erase(other.begin());
    }
  }

  void splice(iterator pos, list& other) {
    if (pos != _front) {
      pos->prev->next = other.begin()._node;
      other.begin()._node->prev = pos->prev;
    } else {
      _front = other._front;
    }
    (--other.end())->next = pos._node;
    pos->prev = (--other.end())._node;
    other._front = other._back;
  }
  void reverse() {
    iterator i = this->begin();
    while (i != this->end()) {
      ListNode<T>* tmp = i._node->next;
      i._node->next = i._node->prev;
      i._node->prev = tmp;
      i--;
    }
    _front->next = _back;
    _back->next = nullptr;
    ListNode<T>* tmp = _back->prev;
    _back->prev = _front;
    _front = tmp;
  }
  void unique() {
    auto i = ++(this->begin());
    while (i != this->end()) {
      iterator tmp(i->next);
      if (*i == (i->prev)->data) {
        this->erase(i);
      }
      i = tmp;
    }
  }
  void sort() {
    if (_size <= 1) {
      return;
    }
    list first;
    list second;
    int counter = 0;
    for (auto i : *this) {
      if (counter < _size / 2) {
        first.push_back(i);
      } else {
        second.push_back(i);
      }
      counter++;
    }
    first.sort();
    second.sort();
    first.merge(second);
    *this = first;
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    value_type val(args...);
    return this->insert(pos, val);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    value_type val(args...);
    this->insert(this->end(), val);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    value_type val(args...);
    this->insert(this->begin(), val);
  }
};

template <class T>
class ListIterator {
 public:
  friend class list<T>;
  // tags
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = ListNode<T>*;    // or also value_type*
  using reference = ListNode<T>&;  // or also value_type&

  // constructors
  ListIterator() = default;
  explicit ListIterator(pointer ptr) { _node = ptr; }
  value_type& operator*() const { return _node->data; }
  pointer operator->() { return _node; }

  // Prefix increment
  ListIterator& operator++() {
    _node = _node->next;
    return *this;
  }

  // Postfix increment
  ListIterator operator++(int) {
    ListIterator tmp(*this);
    ++(*this);
    return tmp;
  }
  ListIterator& operator--() {
    _node = _node->prev;
    return *this;
  }
  ListIterator operator--(int) {
    ListIterator tmp = *this;
    --(*this);
    return tmp;
  }

  friend bool operator==(const ListIterator& a, const ListIterator& b) {
    return a._node == b._node;
  };
  friend bool operator!=(const ListIterator& a, const ListIterator& b) {
    return a._node != b._node;
  };
  operator ConstListIterator<T>() const { return ConstListIterator<T>(_node); }

 protected:
  pointer _node;
};

template <class T>
class ConstListIterator {
 public:
  friend class list<T>;
  // tags
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = ListNode<T>*;    // or also value_type*
  using reference = ListNode<T>&;  // or also value_type&

  // constructors
  ConstListIterator() = default;
  explicit ConstListIterator(pointer ptr) { _node = ptr; }
  value_type operator*() const { return _node->data; }
  // pointer operator*() const { return _node }
  pointer operator->() { return _node; }

  // Prefix increment
  ConstListIterator& operator++() {
    _node = _node->next;
    return *this;
  }

  // Postfix increment
  ConstListIterator operator++(int) {
    ConstListIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  ConstListIterator& operator--() {
    _node = _node->prev;
    return *this;
  }
  ConstListIterator operator--(int) {
    ConstListIterator tmp = *this;
    --(*this);
    return tmp;
  }
  operator ListIterator<T>() const { return ListIterator<T>(_node); }
  friend bool operator==(const ConstListIterator& a,
                         const ConstListIterator& b) {
    return a._node == b._node;
  };
  friend bool operator!=(const ConstListIterator& a,
                         const ConstListIterator& b) {
    return a._node != b._node;
  };

 protected:
  pointer _node;
};

}  // namespace s21
#endif