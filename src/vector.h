#ifndef S21_VECTOR
#define S21_VECTOR
#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

namespace s21 {

using std::cout;
using std::endl;

template <class T>
class iterator_vector;

template <class T>
class const_iterator_vector;

template <class T>
class vector {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = iterator_vector<T>;
  using const_iterator = const_iterator_vector<T>;

 private:
  value_type* front_;
  size_t size_;
  size_t alloc_size_;

 public:
  // CONSTRUCTORS, ASSIGNMENT OPERATORS
  vector() : front_(nullptr), size_(0), alloc_size_(0) {}

  vector(std::initializer_list<value_type> const& items) {
    size_ = alloc_size_ = items.size();
    if (size_ > 0) {
      front_ = new value_type[size_];
      pointer ptr = front_;
      for (auto i : items) {
        *(ptr++) = i;
      }
    } else {
      front_ = nullptr;
    }
  }

  vector(const vector& other)
      : size_(other.size_), alloc_size_(other.alloc_size_) {
    if (size_ > 0) {
      front_ = new T[other.alloc_size_];
      pointer ptr = front_;
      for (auto i : other) {
        *(ptr++) = i;
      }
    } else {
      front_ = nullptr;
    }
  }

  vector(vector&& other) noexcept
      : front_(other.front_),
        size_(other.size_),
        alloc_size_(other.alloc_size_) {
    other.front_ = nullptr;
    other.size_ = 0;
    other.alloc_size_ = 0;
  }

  ~vector() {
    if (!front_) {
      delete[] front_;
    }
    front_ = nullptr;
    size_ = 0;
    alloc_size_ = 0;
  }

  vector& operator=(vector&& other) noexcept {
    if (this == other) return *this;
    if (!front_) {
      delete[] front_;
    }
    front_ = other.front_;
    size_ = other.size_;
    alloc_size_ = other.alloc_size_;
    other.front_ = nullptr;
    other.size_ = 0;
    other.alloc_size_ = 0;
    return *this;
  }
  vector& operator=(const vector& other) {
    if (this->front_ == other.front_) return *this;
    if (!front_) {
      delete[] front_;
    }
    size_ = other.size_;
    alloc_size_ = other.alloc_size_;
    if (size_ > 0) {
      front_ = new T[other.alloc_size_];
      pointer ptr = front_;
      for (auto i : other) {
        *ptr = i;
        ptr++;
      }
    } else {
      front_ = nullptr;
    }
    return *this;
  }

  // METHODS FOR ACCESS OF ELEMENTS
  reference at(size_type pos) {
    if (pos < 0 || pos > size_ - 1) {
      throw std::out_of_range("index out of range");
    }
    return *(front_ + pos);
  }
  const_reference at(size_type pos) const {
    if (pos < 0 || pos > size_ - 1) {
      throw std::out_of_range("index out of range");
    }
    return *(front_ + pos);
  }
  reference operator[](size_type pos) { return *(front_ + pos); }
  const_reference operator[](size_type pos) const { return *(front_ + pos); }
  const_reference front() const { return *front_; }
  const_reference back() const { return *(front_ + size_ - 1); }
  T* data() { return front_; }

  // Vector iterators
  iterator begin() { return iterator(front_); }
  iterator end() { return iterator(front_ + size_); }
  const_iterator begin() const { return const_iterator(front_); }
  const_iterator end() const { return const_iterator(front_ + size_); }

  // Vector capacity methods
  [[nodiscard]] bool empty() const {
    if (size_ == 0) {
      return true;
    }
    return false;
  }
  [[nodiscard]] size_type size() const { return size_; }
  [[nodiscard]] size_type max_size() const {
    return std::numeric_limits<T>::max();
  }
  void reserve(size_type size) {
    if (size > alloc_size_) {
      alloc_size_ = size;
      T* reallocated = new T[alloc_size_];
      std::memcpy(reallocated, front_, size_ * sizeof(T));
      delete[] front_;
      front_ = reallocated;
    }
  }
  [[nodiscard]] size_type capacity() const { return alloc_size_; }
  void shrink_to_fit() {
    if (alloc_size_ > size_) {
      alloc_size_ = size_;
      T* reallocated = new T[alloc_size_];
      std::memmove(reallocated, front_, size_ * sizeof(T));
      delete[] front_;
      front_ = reallocated;
    }
  }

  void push_back(const_reference value) { this->insert(this->end(), value); }
  void pop_back() {
    if (size_ > 0) size_--;
  }
  void show() {
    size_t mysize = 0;
    while (mysize < size_) {
      cout << front_[mysize] << ' ';
      mysize++;
    }
    cout << endl;
  }

  iterator insert(iterator pos, const_reference value) {
    size_++;
    if (front_ == nullptr) {
      alloc_size_ = 1;
      front_ = new value_type[alloc_size_];
      *front_ = value;
      return iterator(front_);
    }
    if (size_ <= alloc_size_) {
      std::memmove(pos.ptr_ + 1, pos.ptr_,
                   (size_ - 1) * sizeof(T) - (pos.ptr_ - front_) * sizeof(T));
      *(pos.ptr_) = value;
      return iterator(pos);
    } else {
      alloc_size_ *= 2;
      T* reallocated = new T[alloc_size_];
      std::memmove(reallocated, front_, (size_ - 1) * sizeof(T));
      size_t p = pos.ptr_ - front_;
      delete[] front_;
      front_ = reallocated;
      if (p != size_ - 1)
        std::memmove(front_ + p + 1, front_ + p, (size_ - 1 - p) * sizeof(T));
      *(front_ + p) = value;
      return iterator(front_ + p);
    }
  }
  void erase(iterator pos) {
    if (front_ == nullptr) {
      return;
    }
    std::memmove(pos.ptr_, pos.ptr_ + 1,
                 (size_ - 1) * sizeof(value_type) -
                     (pos.ptr_ - front_) * sizeof(value_type));
    size_--;
  }
  void clear() { this->~vector(); }
  void swap(vector& other) {
    size_t temp = other.size_;
    other.size_ = size_;
    size_ = temp;
    temp = other.alloc_size_;
    other.alloc_size_ = alloc_size_;
    alloc_size_ = temp;
    T* ptr = other.front_;
    other.front_ = front_;
    front_ = ptr;
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    value_type val(args...);
    return this->insert(pos, val);
  }

  template <class... Args>
  iterator emplace_back(Args&&... args) {
    value_type val(args...);
    return this->insert(this->end(), val);
  }
};

template <class T>
class iterator_vector {
  friend class vector<T>;
  friend class const_iterator_vector<T>;

  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T*;  // or also value_type*
  using reference = T&;

  pointer ptr_;

 public:
  iterator_vector() : ptr_(nullptr){};
  iterator_vector(pointer ptr) : ptr_(ptr){};
  value_type& operator*() const { return (*ptr_); }
  pointer operator->() { return ptr_; }
  iterator_vector& operator++() {
    ptr_++;
    return *this;
  }
  iterator_vector& operator--() {
    ptr_--;
    return *this;
  }
  iterator_vector operator++(int) {
    iterator_vector tmp = *this;
    ++(*this);
    return tmp;
  }
  iterator_vector operator--(int) {
    iterator_vector tmp = *this;
    --(*this);
    return tmp;
  }
  friend bool operator==(const iterator_vector& a, const iterator_vector& b) {
    return a.ptr_ == b.ptr_;
  }
  friend bool operator!=(const iterator_vector& a, const iterator_vector& b) {
    return a.ptr_ != b.ptr_;
  }
  operator const_iterator_vector<T>() const {
    return const_iterator_vector<T>(ptr_);
  }
};

template <class T>
class const_iterator_vector {
  friend class vector<T>;
  friend class iterator_vector<T>;

  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T*;  // or also value_type*
  using reference = T&;
  pointer ptr_;

 public:
  const_iterator_vector() : ptr_(nullptr){};
  const_iterator_vector(pointer ptr) : ptr_(ptr){};
  value_type operator*() const { return (*ptr_); }
  pointer operator->() { return ptr_; }
  const_iterator_vector& operator++() {
    ptr_++;
    return *this;
  }
  const_iterator_vector& operator--() {
    ptr_--;
    return *this;
  }
  const_iterator_vector operator++(int) {
    const_iterator_vector tmp = *this;
    ++(*this);
    return tmp;
  }
  const_iterator_vector operator--(int) {
    const_iterator_vector tmp = *this;
    --(*this);
    return tmp;
  }
  friend bool operator==(const const_iterator_vector& a,
                         const const_iterator_vector& b) {
    return a.ptr_ == b.ptr_;
  }
  friend bool operator!=(const const_iterator_vector& a,
                         const const_iterator_vector& b) {
    return a.ptr_ != b.ptr_;
  }
  operator iterator_vector<T>() const { return iterator_vector<T>(ptr_); }
};

}  // namespace s21
#endif  // S21_VECTOR