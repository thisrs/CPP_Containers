#include <cstddef>
#include <iostream>
#include <utility>

namespace s21 {
template <class T>
struct QueueNode {
  QueueNode<T>* next;
  QueueNode<T>* prev;
  T key;
  QueueNode() : next(nullptr), prev(nullptr), T(0) {}
  QueueNode(T i, QueueNode<T>* p, QueueNode<T>* n) : next(n), prev(p), key(i) {}
};
template <class T>
class queue {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using Node = QueueNode<T>;

 private:
  Node* front_;
  Node* back_;
  size_t size_;

 public:
  // CONSTRUCTORS
  queue() : front_(nullptr), back_(nullptr), size_(0) {}

  queue(std::initializer_list<value_type> const& items)
      : front_(nullptr), back_(nullptr), size_(0) {
    for (auto i : items) {
      this->push(i);
    }
  }
  queue(const queue& q) {
    front_ = back_ = nullptr;
    size_ = 0;
    Node* n = q.front_;
    while (n != nullptr) {
      this->push(n->key);
      n = n->next;
    }
  }

  queue(queue&& q) noexcept {
    front_ = q.front_;
    back_ = q.back_;
    size_ = q.size_;
    q.front_ = q.back_ = nullptr;
    q.size_ = 0;
  }

  ~queue() {
    while (front_ != nullptr) {
      this->pop();
    }
  }
  queue& operator=(queue&& q) noexcept {
    if (front_ != nullptr) {
      while (front_ != nullptr) {
        this->pop();
      }
    }
    front_ = q.front_;
    back_ = q.back_;
    size_ = q.size_;
    q.front_ = q.back_ = nullptr;
    q.size_ = 0;
    return *this;
  }
  queue& operator=(const queue& q) {
    if (this == q) return *this;
    if (front_ != nullptr) {
      while (front_ != nullptr) {
        this->pop();
      }
    }
    front_ = back_ = nullptr;
    Node* n = q.front_;
    while (n != nullptr) {
      this->push(n->key);
      n = n->next;
    }
    return *this;
  }

  // ELEMENT ACCESS
  const_reference front() { return front_->key; }
  const_reference back() { return back_->key; }

  // CAPACITY
  [[nodiscard]] bool empty() const {
    if (size_ == 0) {
      return true;
    }
    return false;
  }
  [[nodiscard]] size_type size() const { return size_; }

  // MODIFIERS
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
      Node* temp = front_;
      front_ = front_->next;
      delete temp;
      if (front_ == nullptr) {
        back_ = nullptr;
      }
    }
  }

  void swap(queue& other) {
    queue temp = other;
    other = *this;
    *this = temp;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    value_type val(args...);
    this->push(val);
  }
};

}  // namespace s21