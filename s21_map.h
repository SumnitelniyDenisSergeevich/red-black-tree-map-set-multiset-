#pragma once

#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "RBTree.h"
#include "s21_pair.h"
#include "s21_vector.h"

namespace s21 {

template <typename Key, typename T, typename value_type = s21_pair<Key, T>>
class map {
 public:
  using iterator = typename RBTree<value_type>::RBIterator;
  using const_iterator = const typename RBTree<value_type>::RBIterator;
  using size_type = std::size_t;

 public:
  map() = default;
  map(std::initializer_list<value_type> const &items);
  map(const map &m);
  map(map &&m);
  ~map();

 public:
  map &operator=(const map &m);
  map &operator=(map &&m);

 public:
  T &at(const Key &key);
  T &operator[](const Key &key);

 public:
  iterator begin();
  iterator end();

 public:
  bool contains(const Key &key);
  bool empty();
  size_type size();
  size_type max_size();
  void clear();

 public:
  std::pair<iterator, bool> insert(const value_type &value);
  std::pair<iterator, bool> insert(const std::pair<Key, T> value);
  std::pair<iterator, bool> insert(const Key &key, const T &obj);
  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj);
  void erase(iterator pos);
  void swap(map &other);
  void merge(map &other);

 public:
  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> result;
    InsertManyRec(result, args...);
    return result;
  }

 private:
  template <typename T0, typename... Args>
  void InsertManyRec(s21::vector<std::pair<iterator, bool>> &vec, const T0 &v0,
                     Args &&...args) {
    if constexpr (sizeof...(args) != 0) {
      if (rbTree_.Contains(value_type{v0}))
        vec.push_back({rbTree_.Find(value_type{v0}), false});
      else
        vec.push_back(insert(v0));
      InsertManyRec(vec, args...);
    } else {
      if (rbTree_.Contains(value_type{v0}))
        vec.push_back({rbTree_.Find(value_type{v0}), false});
      else
        vec.push_back(insert(v0));
    }
  }

 private:
  RBTree<value_type> rbTree_;
};
}  // namespace s21

template <typename Key, typename T, typename value_type>
s21::map<Key, T, value_type>::map(
    std::initializer_list<value_type> const &items) {
  for (const value_type &val : items) rbTree_.Insert(val);
}

template <typename Key, typename T, typename value_type>
s21::map<Key, T, value_type>::map(const map &m) {
  if (rbTree_ != m.rbTree_) rbTree_ = m.rbTree_;
}

template <typename Key, typename T, typename value_type>
s21::map<Key, T, value_type>::map(map &&m) {
  if (rbTree_ != m.rbTree_) rbTree_ = std::move(m.rbTree_);
}

template <typename Key, typename T, typename value_type>
s21::map<Key, T, value_type>::~map() {
  rbTree_.Clear();
}

template <typename Key, typename T, typename value_type>
s21::map<Key, T, value_type> &s21::map<Key, T, value_type>::operator=(map &&m) {
  if (rbTree_ != m.rbTree_) rbTree_ = std::move(m.rbTree_);
  return *this;
}

template <typename Key, typename T, typename value_type>
s21::map<Key, T, value_type> &s21::map<Key, T, value_type>::operator=(
    const map &m) {
  if (rbTree_ != m.rbTree_) rbTree_ = m.rbTree_;
  return *this;
}

template <typename Key, typename T, typename value_type>
T &s21::map<Key, T, value_type>::at(const Key &key) {
  iterator it = rbTree_.Find({key, {}});
  if (it && it != rbTree_.end())
    return (*it).second;
  else
    throw std::out_of_range("out of map range");
}

template <typename Key, typename T, typename value_type>
T &s21::map<Key, T, value_type>::operator[](const Key &key) {
  return (*rbTree_.Find({key, {}})).second;
}

template <typename Key, typename T, typename value_type>
typename s21::map<Key, T, value_type>::iterator
s21::map<Key, T, value_type>::begin() {
  return rbTree_.begin();
}

template <typename Key, typename T, typename value_type>
typename s21::map<Key, T, value_type>::iterator
s21::map<Key, T, value_type>::end() {
  return rbTree_.end();
}

template <typename Key, typename T, typename value_type>
bool s21::map<Key, T, value_type>::contains(const Key &key) {
  return rbTree_.Contains({key, {}});
}

template <typename Key, typename T, typename value_type>
bool s21::map<Key, T, value_type>::empty() {
  return rbTree_.Empty();
}

template <typename Key, typename T, typename value_type>
typename s21::map<Key, T, value_type>::size_type
s21::map<Key, T, value_type>::size() {
  return rbTree_.Size();
}

template <typename Key, typename T, typename value_type>
typename s21::map<Key, T, value_type>::size_type
s21::map<Key, T, value_type>::max_size() {
  return std::numeric_limits<value_type>::max();  // need test
}

template <typename Key, typename T, typename value_type>
void s21::map<Key, T, value_type>::clear() {
  rbTree_.Clear();
}

template <typename Key, typename T, typename value_type>
std::pair<typename s21::map<Key, T, value_type>::iterator, bool>
s21::map<Key, T, value_type>::insert(const value_type &value) {
  if (rbTree_.Contains(value))
    return {rbTree_.Find(value), false};
  else
    return rbTree_.Insert(value);
}

template <typename Key, typename T, typename value_type>
std::pair<typename s21::map<Key, T, value_type>::iterator, bool>
s21::map<Key, T, value_type>::insert(const std::pair<Key, T> value) {
  if (rbTree_.Contains(value_type{value.first, value.second}))
    return {rbTree_.Find(value_type{value.first, value.second}), false};
  else
    return rbTree_.Insert(value_type{value.first, value.second});
}

template <typename Key, typename T, typename value_type>
std::pair<typename s21::map<Key, T, value_type>::iterator, bool>
s21::map<Key, T, value_type>::insert(const Key &key, const T &obj) {
  if (rbTree_.Contains({key, {}}))
    return {rbTree_.Find({key, {}}), false};
  else
    return rbTree_.Insert({key, obj});
}

template <typename Key, typename T, typename value_type>
std::pair<typename s21::map<Key, T, value_type>::iterator, bool>
s21::map<Key, T, value_type>::insert_or_assign(const Key &key, const T &obj) {
  if (rbTree_.Contains({key, {}})) {
    iterator it = rbTree_.Find({key, {}});
    (*it).second = obj;
    return {it, true};
  } else
    return rbTree_.Insert({key, obj});
}

template <typename Key, typename T, typename value_type>
void s21::map<Key, T, value_type>::erase(iterator pos) {
  if (pos != rbTree_.end()) rbTree_.Erase(pos);
}

template <typename Key, typename T, typename value_type>
void s21::map<Key, T, value_type>::swap(map &other) {
  rbTree_.Swap(other.rbTree_);
}

template <typename Key, typename T, typename value_type>
void s21::map<Key, T, value_type>::merge(map &other) {
  for (const auto &val : other)
    if (!rbTree_.Contains(val)) rbTree_.Insert(val);
}
