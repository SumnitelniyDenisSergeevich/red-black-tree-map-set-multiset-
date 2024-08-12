#pragma once

#include <iostream>
#include <utility>

namespace s21 {

template <typename Type1, typename Type2>
class s21_pair {
 public:
  s21_pair() = default;
  s21_pair(const Type1 &v1, const Type2 &v2);
  s21_pair(std::pair<Type1, Type2> pair);

 public:
  bool operator<(const s21_pair &other);
  bool operator>(const s21_pair &other);

 public:
  Type1 first;
  Type2 second;
};

}  // namespace s21

template <typename Type1, typename Type2>
s21::s21_pair<Type1, Type2>::s21_pair(const Type1 &v1, const Type2 &v2)
    : first(v1), second(v2) {}

template <typename Type1, typename Type2>
s21::s21_pair<Type1, Type2>::s21_pair(std::pair<Type1, Type2> pair)
    : first(pair.first), second(pair.second) {}

template <typename Type1, typename Type2>
bool s21::s21_pair<Type1, Type2>::operator<(const s21_pair &other) {
  return first < other.first;
}

template <typename Type1, typename Type2>
bool s21::s21_pair<Type1, Type2>::operator>(const s21_pair &other) {
  return first > other.first;
}
