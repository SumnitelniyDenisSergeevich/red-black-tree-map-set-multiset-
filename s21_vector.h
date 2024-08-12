#pragma once
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>

template <typename T>
class RawMemory {
 public:
  RawMemory() = default;

  explicit RawMemory(size_t capacity)
      : buffer_(Allocate(capacity)), capacity_(capacity) {}

  ~RawMemory() { Deallocate(buffer_); }

  RawMemory(const RawMemory &) = delete;
  RawMemory &operator=(const RawMemory &rhs) = delete;

  RawMemory(RawMemory &&other) noexcept {
    buffer_ = std::exchange(other.buffer_, nullptr);
    capacity_ = std::exchange(other.capacity_, 0);
  }

  RawMemory &operator=(RawMemory &&rhs) noexcept {
    Deallocate(buffer_);
    buffer_ = std::exchange(rhs.buffer_, nullptr);
    capacity_ = std::exchange(rhs.capacity_, 0);
    return *this;
  }

  T *operator+(size_t offset) noexcept {
    // Разрешается получать адрес ячейки памяти, следующей за последним
    // элементом массива
    assert(offset <= capacity_);
    return buffer_ + offset;
  }

  const T *operator+(size_t offset) const noexcept {
    return const_cast<RawMemory &>(*this) + offset;
  }

  const T &operator[](size_t index) const noexcept {
    return const_cast<RawMemory &>(*this)[index];
  }

  T &operator[](size_t index) noexcept {
    assert(index < capacity_);
    return buffer_[index];
  }

  void Swap(RawMemory &other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(capacity_, other.capacity_);
  }

  const T *GetAddress() const noexcept { return buffer_; }

  T *GetAddress() noexcept { return buffer_; }

  size_t Capacity() const { return capacity_; }

 private:
  // Выделяет сырую память под n элементов и возвращает указатель на неё
  static T *Allocate(size_t n) {
    return n != 0 ? static_cast<T *>(operator new(n * sizeof(T))) : nullptr;
  }

  // Освобождает сырую память, выделенную ранее по адресу buf при помощи
  // Allocate
  static void Deallocate(T *buf) noexcept { operator delete(buf); }

  T *buffer_ = nullptr;
  size_t capacity_ = 0;
};

namespace s21 {
template <typename T>
class vector {
 public:
  vector() = default;

  explicit vector(size_t size)
      : data_(size),
        size_(size)  //
  {
    std::uninitialized_value_construct_n(begin(), size);
  }

  vector(std::initializer_list<T> const &items)
      : data_(items.size()),
        size_(items.size())  //
  {
    std::uninitialized_value_construct_n(begin(), size_);

    size_t ind = 0;
    for (auto it = items.begin(); it != items.end(); ++it)
      const_cast<vector &>(*this)[ind++] = *it;
  }

  vector(const vector &other)
      : data_(other.size_),
        size_(other.size_)  //
  {
    std::uninitialized_copy_n(other.begin(), other.size_, begin());
  }

  vector(vector &&other) noexcept
      : data_(std::move(other.data_)), size_(std::exchange(other.size_, 0)) {}

  ~vector() { std::destroy_n(begin(), size_); }

  using iterator = T *;
  using const_iterator = const T *;

  iterator begin() noexcept { return data_.GetAddress(); }

  iterator end() noexcept { return data_.GetAddress() + size_; }

  const_iterator begin() const noexcept { return cbegin(); }

  const_iterator end() const noexcept { return cend(); }

  const_iterator cbegin() const noexcept { return data_.GetAddress(); }

  const_iterator cend() const noexcept { return data_.GetAddress() + size_; }

  vector &operator=(const vector &rhs) {
    if (this != &rhs) {
      if (rhs.size_ > data_.Capacity()) {
        vector rhs_copy(rhs);
        swap(rhs_copy);
      } else {
        size_t copy_size = std::min(rhs.size_, size_);
        std::copy_n(rhs.begin(), copy_size, begin());
        if (rhs.size_ < size_) {
          std::destroy_n(begin() + rhs.size_, size_ - rhs.size_);
        } else {
          std::uninitialized_copy_n(rhs.begin() + size_, rhs.size_ - size_,
                                    end());
        }
        size_ = rhs.size_;
      }
    }
    return *this;
  }

  vector &operator=(vector &&rhs) noexcept {
    if (this != &rhs) {
      data_ = std::move(rhs.data_);
      size_ = std::exchange(rhs.size_, 0);
    }
    return *this;
  }

  void clear() { size_ = 0; }

  void reserve(size_t new_capacity) {
    if (new_capacity <= data_.Capacity()) {
      return;
    }
    RawMemory<T> new_data(new_capacity);
    InitializeWithCopyMoveUninitializedN(begin(), size_, new_data.GetAddress());
    std::destroy_n(begin(), size_);
    data_.Swap(new_data);
  }

  void resize(size_t new_size) {
    if (new_size < size_) {
      std::destroy_n(begin() + new_size, size_ - new_size);
    } else if (new_size > size_) {
      reserve(new_size);
      std::uninitialized_value_construct_n(end(), new_size - size_);
    }
    size_ = new_size;
  }

  void push_back(const T &value) { (void)emplace_back(value); }

  void push_back(T &&value) { (void)emplace_back(std::move(value)); }

  void shrink_to_fit() {
    if (data_.Capacity() > size_) {
      RawMemory<T> new_data(size_);
      InitializeWithCopyMoveUninitializedN(begin(), size_,
                                           new_data.GetAddress());
      std::destroy_n(begin(), size_);
      data_.Swap(new_data);
    }
  }

  template <typename... Args>
  T &emplace_back(Args &&...args) {
    if (size_ == capacity()) {
      RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
      new (new_data.GetAddress() + size_) T(std::forward<Args>(args)...);
      InitializeWithCopyMoveUninitializedN(begin(), size_,
                                           new_data.GetAddress());
      std::destroy_n(begin(), size_);
      data_.Swap(new_data);
    } else {
      new (data_ + size_) T(std::forward<Args>(args)...);
    }
    ++size_;
    return *(end() - 1);
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args &&...args) {
    auto iter = pos - begin();
    assert(begin() <= pos && pos <= end());
    if (pos == end()) {
      return &emplace_back(std::forward<Args>(args)...);
    }
    if (size_ == capacity()) {
      EmplaceFilledVector(iter, std::forward<Args>(args)...);
    } else {
      EmplaceUnFilledVector(iter, std::forward<Args>(args)...);
    }
    ++size_;
    return begin() + iter;
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    assert(begin() <= pos && pos <= end());
    auto iter = pos - begin();
    InsertManyRec(iter, args...);
    return begin() + iter;
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    InsertManyBackRec(args...);
  }

  iterator erase(const_iterator pos) noexcept(
      std::is_nothrow_move_assignable_v<T>) {
    auto iter = pos - begin();
    std::move(begin() + iter + 1, end(), begin() + iter);
    pop_back();
    return begin() + iter;
  }

  iterator insert(const_iterator pos, const T &value) {
    return emplace(pos, value);
  }

  iterator insert(const_iterator pos, T &&value) {
    return emplace(pos, std::move(value));
  }

  void pop_back() noexcept {
    assert(size_ != 0);
    --size_;
    std::destroy_at(end());
  }

  void swap(vector &other) noexcept {
    data_.Swap(other.data_);
    std::swap(size_, other.size_);
  }

  size_t size() const noexcept { return size_; }

  size_t capacity() const noexcept { return data_.Capacity(); }

  const T &operator[](size_t index) const noexcept {
    return const_cast<vector &>(*this)[index];
  }

  bool empty() const noexcept { return size_ == 0; }

  T &operator[](size_t index) noexcept {
    assert(index < size_);
    return data_[index];
  }

  T &at(size_t index) {
    if (index >= size_) throw std::out_of_range("out_of_range");
    return data_[index];
  }

  const T &at(size_t index) const {
    if (index >= size_) throw std::out_of_range("out_of_range");
    return data_[index];
  }

  T *data() { return data_.GetAddress(); }

  const T *data() const { return data_.GetAddress(); }

  T &front() { return data_[0]; }

  const T &front() const { return data_[0]; }

  T &back() { return data_[size_ - 1]; }

  const T &back() const { return data_[size_ - 1]; }

 private:
  template <typename T0, typename... Args>
  void InsertManyRec(size_t iter, const T0 &v0, Args &&...args) {
    if constexpr (sizeof...(args) != 0) {
      insert(begin() + iter, v0);
      InsertManyRec(iter + 1, args...);
    } else
      insert(begin() + iter, v0);
  }

  template <typename T0, typename... Args>
  void InsertManyBackRec(const T0 &v0, Args &&...args) {
    if constexpr (sizeof...(args) != 0) {
      push_back(v0);
      InsertManyBackRec(args...);
    } else
      push_back(v0);
  }

  template <typename... Args>
  void EmplaceFilledVector(size_t iter, Args &&...args) {
    RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
    new (new_data.GetAddress() + iter) T(std::forward<Args>(args)...);
    try {
      InitializeWithCopyMoveUninitializedN(begin(), iter,
                                           new_data.GetAddress());
    } catch (...) {
      std::destroy_at(new_data.GetAddress() + iter);
      throw;
    }
    try {
      InitializeWithCopyMoveUninitializedN(begin() + iter, size_ - iter,
                                           new_data.GetAddress() + iter + 1);
    } catch (...) {
      std::destroy_n(new_data.GetAddress(), iter + 1);
      throw;
    }
    std::destroy_n(begin(), size_);
    data_.Swap(new_data);
  }

  template <typename... Args>
  void EmplaceUnFilledVector(size_t iter, Args &&...args) {
    T temp_obj = T(std::forward<Args>(args)...);
    std::uninitialized_move_n(end() - 1, 1, end());
    std::move_backward(begin() + iter, end() - 1, end());
    data_[iter] = std::move(temp_obj);
  }

  void InitializeWithCopyMoveUninitializedN(iterator from, size_t count,
                                            iterator to) {
    if constexpr (std::is_nothrow_move_constructible_v<T> ||
                  !std::is_copy_constructible_v<T>) {
      std::uninitialized_move_n(from, count, to);
    } else {
      std::uninitialized_copy_n(from, count, to);
    }
  }

  RawMemory<T> data_;
  size_t size_ = 0;
};
}  // namespace s21
