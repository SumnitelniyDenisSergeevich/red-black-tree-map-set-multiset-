#pragma once

#include <initializer_list>
#include <limits>

#include "RBTree.h"
#include "s21_vector.h"

namespace s21 {
template <typename Key>
class set {
 public:
  using iterator = typename RBTree<Key>::RBIterator;
  using const_iterator = const typename RBTree<Key>::RBIterator;
  using size_type = std::size_t;

 public:
  set() = default;
  set(std::initializer_list<Key> const &items);
  set(const set &s);
  set(set &&s);
  ~set();

 public:
  void operator=(set &&s);

 public:
  iterator begin();
  iterator end();

 public:
  bool empty();
  size_type size();
  size_type max_size();

 public:
  void clear();
  std::pair<iterator, bool> insert(const Key &value);
  void erase(iterator pos);
  void swap(set &other);
  void merge(set &other);

 public:
  iterator find(const Key &key);
  bool contains(const Key &key);

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
      if (rbTree_.Contains(v0))
        vec.push_back({rbTree_.Find(v0), false});
      else
        vec.push_back(insert(v0));
      InsertManyRec(vec, args...);
    } else {
      if (rbTree_.Contains(v0))
        vec.push_back({rbTree_.Find(v0), false});
      else
        vec.push_back(insert(v0));
    }
  }

 private:
  RBTree<Key> rbTree_;
};
}  // namespace s21

template <typename Key>
s21::set<Key>::set(std::initializer_list<Key> const &items) {
  for (const Key &val : items) rbTree_.Insert(val);
}

template <typename Key>
s21::set<Key>::set(const set &s) {
  rbTree_ = s.rbTree_;
}

template <typename Key>
s21::set<Key>::set(set &&s) {
  rbTree_ = std::move(s.rbTree_);
}

template <typename Key>
s21::set<Key>::~set() {
  rbTree_.Clear();
}

template <typename Key>
void s21::set<Key>::operator=(set &&s) {
  rbTree_ = std::move(s.rbTree_);
}

template <typename Key>
typename s21::set<Key>::iterator s21::set<Key>::begin() {
  return rbTree_.begin();
}

template <typename Key>
typename s21::set<Key>::iterator s21::set<Key>::end() {
  return rbTree_.end();
}

template <typename Key>
bool s21::set<Key>::empty() {
  return rbTree_.Empty();
}

template <typename Key>
typename s21::set<Key>::size_type s21::set<Key>::size() {
  return rbTree_.Size();
}

template <typename Key>
typename s21::set<Key>::size_type s21::set<Key>::max_size() {
  return std::numeric_limits<Key>::max();  // need test
}

template <typename Key>
void s21::set<Key>::clear() {
  rbTree_.Clear();
}

template <typename Key>
void s21::set<Key>::erase(iterator pos) {
  if (pos != rbTree_.end()) rbTree_.Erase(pos);
}

template <typename Key>
void s21::set<Key>::swap(set &other) {
  rbTree_.Swap(other.rbTree_);
}

template <typename Key>
typename s21::set<Key>::iterator s21::set<Key>::find(const Key &key) {
  return rbTree_.Find(key);
}

template <typename Key>
bool s21::set<Key>::contains(const Key &key) {
  return rbTree_.Contains(key);
}

template <typename Key>
std::pair<typename s21::set<Key>::iterator, bool> s21::set<Key>::insert(
    const Key &value) {
  if (rbTree_.Contains(value))
    return {rbTree_.Find(value), false};
  else
    return rbTree_.Insert(value);
}

template <typename Key>
void s21::set<Key>::merge(set &other) {
  s21::set<Key> deleteValues;

  for (const auto &val : other)
    if (!contains(val)) {
      rbTree_.Insert(val);
      deleteValues.insert(val);
    }

  for (const auto &val : deleteValues) other.rbTree_.Erase(val);
}
