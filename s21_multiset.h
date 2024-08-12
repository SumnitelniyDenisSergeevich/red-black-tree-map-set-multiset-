#pragma once

#include <initializer_list>
#include <limits>

#include "RBTree.h"
#include "s21_vector.h"

namespace s21 {
template <typename Key>
class multiset {
 public:
  using iterator = typename RBTree<Key>::RBIterator;
  using const_iterator = const typename RBTree<Key>::RBIterator;
  using size_type = std::size_t;

 public:
  multiset() = default;
  multiset(std::initializer_list<Key> const &items);
  multiset(const multiset &s);
  multiset(multiset &&s);
  ~multiset();

 public:
  void operator=(multiset &&s);
  void operator=(multiset &s);

 public:
  iterator begin();
  iterator end();
  const_iterator cbegin();
  const_iterator cend();

 public:
  bool empty();
  size_type size();
  size_type max_size();

 public:
  void clear();
  std::pair<iterator, bool> insert(const Key &value);
  void erase(iterator pos);
  void swap(multiset &other);
  void merge(multiset &other);

 public:
  iterator find(const Key &key);
  bool contains(const Key &key);
  size_t count(const Key &key);

 public:
  std::pair<iterator, iterator> equal_range(const Key &key);
  iterator lower_bound(const Key &key);
  iterator upper_bound(const Key &key);

 public:
  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    return rbTree_.insert_many(args...);
  }

 private:
  RBTree<Key> rbTree_;
};
}  // namespace s21

template <typename Key>
s21::multiset<Key>::multiset(std::initializer_list<Key> const &items) {
  for (const Key &val : items) rbTree_.Insert(val);
}

template <typename Key>
s21::multiset<Key>::multiset(const multiset &s) {
  rbTree_ = s.rbTree_;
}

template <typename Key>
s21::multiset<Key>::multiset(multiset &&s) {
  rbTree_ = std::move(s.rbTree_);
}

template <typename Key>
s21::multiset<Key>::~multiset() {
  rbTree_.Clear();
}

template <typename Key>
void s21::multiset<Key>::operator=(multiset &&s) {
  rbTree_ = std::move(s.rbTree_);
}

template <typename Key>
void s21::multiset<Key>::operator=(multiset &s) {
  clear();
  rbTree_ = s.rbTree_;
}

template <typename Key>
typename s21::multiset<Key>::iterator s21::multiset<Key>::begin() {
  return rbTree_.begin();
}

template <typename Key>
typename s21::multiset<Key>::iterator s21::multiset<Key>::end() {
  return rbTree_.end();
}

template <typename Key>
typename s21::multiset<Key>::const_iterator s21::multiset<Key>::cbegin() {
  return rbTree_.begin();
}

template <typename Key>
typename s21::multiset<Key>::const_iterator s21::multiset<Key>::cend() {
  return rbTree_.end();
}

template <typename Key>
bool s21::multiset<Key>::empty() {
  return rbTree_.Empty();
}

template <typename Key>
typename s21::multiset<Key>::size_type s21::multiset<Key>::size() {
  return rbTree_.Size();
}

template <typename Key>
typename s21::multiset<Key>::size_type s21::multiset<Key>::max_size() {
  return std::numeric_limits<Key>::max();  // need test
}

template <typename Key>
void s21::multiset<Key>::clear() {
  rbTree_.Clear();
}

template <typename Key>
void s21::multiset<Key>::erase(iterator pos) {
  if (pos != rbTree_.end()) rbTree_.Erase(pos);
}

template <typename Key>
void s21::multiset<Key>::swap(multiset &other) {
  rbTree_.Swap(other.rbTree_);
}

template <typename Key>
typename s21::multiset<Key>::iterator s21::multiset<Key>::find(const Key &key) {
  return rbTree_.Find(key);
}

template <typename Key>
bool s21::multiset<Key>::contains(const Key &key) {
  return rbTree_.Contains(key);
}

template <typename Key>
size_t s21::multiset<Key>::count(const Key &key) {
  return rbTree_.Count(key);
}

template <typename Key>
std::pair<typename s21::multiset<Key>::iterator,
          typename s21::multiset<Key>::iterator>
s21::multiset<Key>::equal_range(const Key &key) {
  return rbTree_.equal_range(key);
}

template <typename Key>
typename s21::multiset<Key>::iterator s21::multiset<Key>::lower_bound(
    const Key &key) {
  return rbTree_.lower_bound(key);
}

template <typename Key>
typename s21::multiset<Key>::iterator s21::multiset<Key>::upper_bound(
    const Key &key) {
  return rbTree_.upper_bound(key);
}

template <typename Key>
std::pair<typename s21::multiset<Key>::iterator, bool>
s21::multiset<Key>::insert(const Key &value) {
  return rbTree_.Insert(value);
}

template <typename Key>
void s21::multiset<Key>::merge(multiset &other) {
  for (const auto &val : other) rbTree_.Insert(val);

  other.clear();
}
