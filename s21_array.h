#pragma once
#include <limits>

namespace s21 {
template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  array();
  array(std::initializer_list<value_type> const &items);
  array(const array &a);
  array(array &&a) noexcept;
  ~array() = default;

  iterator begin();
  iterator end();
  const_iterator cbegin(void) const;
  const_iterator cend(void) const;

  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference front();
  reference back();
  const_reference front() const;
  const_reference back() const;

  bool empty();
  size_type size();
  size_type max_size();

  void swap(array &a);
  void fill(const_reference value);

  array &operator=(const array &a);
  array &operator=(array &&a) noexcept;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

 private:
  value_type elems_[N] = {};
  size_type size_ = N;
};
}  // namespace s21

template <typename T, std::size_t N>
s21::array<T, N>::array() : size_(N) {}

template <typename T, std::size_t N>
s21::array<T, N>::array(const std::initializer_list<value_type> &items) {
  std::copy(items.begin(), items.end(), elems_);
}

template <typename T, std::size_t N>
s21::array<T, N>::array(const array<T, N> &a) {
  std::copy(a.elems_, a.elems_ + N, elems_);
}

template <typename T, std::size_t N>
s21::array<T, N>::array(array<T, N> &&a) noexcept {
  std::move(a.elems_, a.elems_ + N, elems_);
}

template <typename T, std::size_t N>
typename s21::array<T, N>::iterator s21::array<T, N>::begin() {
  return elems_;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::iterator s21::array<T, N>::end() {
  return elems_ + size_;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::const_iterator s21::array<T, N>::cbegin() const {
  return elems_;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::const_iterator s21::array<T, N>::cend() const {
  return elems_ + size_;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::reference s21::array<T, N>::at(size_type pos) {
  return (*this)[pos];
}

template <typename T, std::size_t N>
typename s21::array<T, N>::const_reference s21::array<T, N>::at(
    size_type pos) const {
  return (*this)[pos];
}

template <typename T, std::size_t N>
typename s21::array<T, N>::reference s21::array<T, N>::front() {
  return elems_[0];
}

template <typename T, std::size_t N>
typename s21::array<T, N>::reference s21::array<T, N>::back() {
  return elems_[N - 1];
}

template <typename T, std::size_t N>
typename s21::array<T, N>::const_reference s21::array<T, N>::front() const {
  return elems_[0];
}

template <typename T, std::size_t N>
typename s21::array<T, N>::const_reference s21::array<T, N>::back() const {
  return elems_[N - 1];
}

template <typename T, std::size_t N>
bool s21::array<T, N>::empty() {
  return !size_;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::size_type s21::array<T, N>::size() {
  return size_;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::size_type s21::array<T, N>::max_size() {
  return size();
}

template <typename T, std::size_t N>
void s21::array<T, N>::swap(array<T, N> &a) {
  std::swap_ranges(elems_, elems_ + N, a.elems_);
}

template <typename T, std::size_t N>
void s21::array<T, N>::fill(const_reference value) {
  std::fill(elems_, elems_ + N, value);
}

template <typename T, std::size_t N>
typename s21::array<T, N>::array &s21::array<T, N>::operator=(
    const array<T, N> &a) {
  std::copy(a.elems_, a.elems_ + N, elems_);

  return *this;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::array &s21::array<T, N>::operator=(
    array<T, N> &&a) noexcept {
  for (size_t i = 0; i < N; ++i) {
    elems_[i] = std::move(a.elems_[i]);
  }

  return *this;
}

template <typename T, std::size_t N>
typename s21::array<T, N>::reference s21::array<T, N>::operator[](
    size_type pos) {
  if (pos >= N) {
    throw std::out_of_range("OutOfRangeError: Index out of array range");
  }

  return elems_[pos];
}

template <typename T, std::size_t N>
typename s21::array<T, N>::const_reference s21::array<T, N>::operator[](
    size_type pos) const {
  if (pos >= N) {
    throw std::out_of_range("OutOfRangeError: Index out of array range");
  }

  return elems_[pos];
}
