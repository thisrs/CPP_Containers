
#include <cstddef>
#include <iostream>
#include <utility>

namespace s21 {
template <class T>
struct StackNode {
  StackNode<T>* next;
  StackNode<T>* prev;
  T key;
  StackNode() {
    next = nullptr;
    prev = nullptr;
  }
  StackNode(T i, StackNode<T>* p, StackNode<T>* n) {
    key = i;
    next = n;
    prev = p;
  }
};
template <class T>
class stack {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using Node = StackNode<T>;

 private:
  Node* front_;
  Node* back_;
  size_t size_;

 public:
  // CONSTRUCTORS
  stack() : front_(nullptr), back_(nullptr), size_(0) {}

  stack(std::initializer_list<value_type> const& items)
      : front_(nullptr), back_(nullptr), size_(0) {
    for (auto i : items) {
      this->push(i);
    }
  }

  stack(const stack& q) : front_(nullptr), back_(nullptr), size_(0) {
    Node* n = q.front_;
    while (n != nullptr) {
      this->push(n->key);
      n = n->next;
    }
  }

  stack(stack&& q) noexcept : front_(q.front_), back_(q.back_), size_(q.size_) {
    q.front_ = q.back_ = nullptr;
    q.size_ = 0;
  }

  ~stack() {
    while (front_ != nullptr) {
      this->pop();
    }
  }

  stack& operator=(stack&& q) noexcept {
    if (front_ != nullptr) {
      this->~stack();
    }
    front_ = q.front_;
    back_ = q.back_;
    size_ = q.size_;
    q.front_ = q.back_ = nullptr;
    q.size_ = 0;
    return *this;
  }
  stack& operator=(const stack& q) {
    if (this->front_ == q.front_) return *this;
    if (front_ != nullptr) {
      this->~stack();
    }
    Node* n = q.front_;
    while (n != nullptr) {
      this->push(n->key);
      n = n->next;
    }
    return *this;
  }

  // ELEMENT ACCESS
  const_reference top() const { return back_->key; }

  // CAPACITY
  [[nodiscard]] bool empty() const {
    if (size_ == 0) {
      return true;
    }
    return false;
  }
  [[nodiscard]] size_type size() const { return size_; }

  // ELEMENT ACCESS
  void push(const_reference value) {
    size_++;
    Node* n = new Node(value, back_, nullptr);
    if (back_ != nullptr) {
      back_->next = n;
    } else {
      front_ = n;
    }
    back_ = n;
  }
  void pop() {
    if (front_ != nullptr) {
      size_--;
      Node* temp = back_;
      back_ = back_->prev;
      delete temp;
      if (back_ == nullptr) {
        front_ = nullptr;
      }
    }
  }
  void swap(stack& other) {
    stack temp = other;
    other = *this;
    *this = temp;
  }
  template <class... Args>
  void emplace_front(Args&&... args) {
    value_type val(args...);
    this->push(val);
  }
};

}  // namespace s21
