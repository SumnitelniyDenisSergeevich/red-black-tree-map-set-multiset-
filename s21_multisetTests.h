#pragma once

#include <gtest/gtest.h>

#include "s21_array.h"
#include "s21_multiset.h"

// MULTISET
TEST(multiset, DefaultConstructor) {
  s21::multiset<int> t;
  EXPECT_TRUE(t.empty());
  EXPECT_EQ(static_cast<int>(t.size()), 0);
}

TEST(multiset, InitializerListConstructor) {
  s21::multiset<int> t = {1,  5,  2,  55, -9, -5, 18, 56,  10,  3,  13, 23,
                          27, 20, 22, 57, 60, 59, 75, -10, -11, 55, -11};
  EXPECT_FALSE(t.empty());
  EXPECT_EQ(static_cast<int>(t.size()), 23);

  EXPECT_EQ(*t.cbegin(), -11);
  EXPECT_EQ(*(--t.cend()), 75);
}

TEST(multiset, CopyConstructor) {
  s21::multiset<int> t1 = {1,  5,  2,  55, -9, -5, 18, 56,  10,  3,  13, 23,
                           27, 20, 22, 57, 60, 59, 75, -10, -11, 55, -11};
  s21::multiset<int> t2 = t1;
  EXPECT_EQ(static_cast<int>(t1.size()), 23);
  EXPECT_EQ(static_cast<int>(t2.size()), 23);

  EXPECT_EQ(*t1.cbegin(), -11);
  EXPECT_EQ(*t2.cbegin(), -11);

  EXPECT_EQ(*(--t1.cend()), 75);
  EXPECT_EQ(*(--t2.cend()), 75);
}

TEST(multiset, MoveConstructor) {
  s21::multiset<int> t1 = {1, 2, 3, 4, 5, 5};
  s21::multiset<int> t2 = std::move(t1);

  EXPECT_TRUE(t1.empty());
  EXPECT_EQ(static_cast<int>(t1.size()), 0);
  EXPECT_FALSE(t2.empty());
  EXPECT_EQ(static_cast<int>(t2.size()), 6);

  EXPECT_EQ(*t2.cbegin(), 1);
  EXPECT_EQ(*(--t2.cend()), 5);
}

TEST(multiset, AssignmentOperatorCopy) {
  s21::multiset<int> t1 = {1, 2, 3, 4, 5, 5};
  s21::multiset<int> t2 = {6, 7, 8, 6};

  EXPECT_EQ(static_cast<int>(t1.size()), 6);
  EXPECT_EQ(static_cast<int>(t2.size()), 4);

  t2 = t1;
  EXPECT_EQ(static_cast<int>(t1.size()), 6);
  EXPECT_EQ(static_cast<int>(t2.size()), 6);
}

TEST(multiset, AssignmentOperatorMove) {
  s21::multiset<int> t1 = {1, 2, 3, 4, 5, 5};
  s21::multiset<int> t2 = {6, 7, 8, 6};

  EXPECT_EQ(static_cast<int>(t1.size()), 6);
  EXPECT_EQ(static_cast<int>(t2.size()), 4);

  t2 = std::move(t1);

  EXPECT_TRUE(t1.empty());
  EXPECT_EQ(static_cast<int>(t1.size()), 0);
  EXPECT_EQ(static_cast<int>(t2.size()), 6);
}

TEST(multiset, Insert) {
  s21::multiset<int> t = {1, 2, 3, 4, 5, 5};

  EXPECT_EQ(static_cast<int>(t.size()), 6);
  t.insert(1);

  EXPECT_EQ(static_cast<int>(t.size()), 7);

  EXPECT_EQ(*(--t.cend()), 5);
  t.insert(11);
  t.insert(12);
  t.insert(11);
  t.insert(12);
  EXPECT_EQ(static_cast<int>(t.size()), 11);
}

TEST(multiset, InsertMany) {
  s21::multiset<int> t = {1, 2, 3, 4, 5, 5};
  t.insert_many(100, 99, 99);
  EXPECT_EQ(*(--t.cend()), 100);

  EXPECT_EQ(static_cast<int>(t.size()), 9);
  t.insert_many(-6, -6, 8);
  EXPECT_EQ(static_cast<int>(t.size()), 12);
  EXPECT_EQ(*(t.cbegin()), -6);
}

TEST(multiset, Swap) {
  s21::multiset<int> t1 = {1, 2, 3, 4, 5, 5};
  s21::multiset<int> t2 = {6, 7, 8, 6};
  EXPECT_EQ(static_cast<int>(t1.size()), 6);
  EXPECT_EQ(static_cast<int>(t2.size()), 4);

  t1.swap(t2);
  EXPECT_EQ(static_cast<int>(t1.size()), 4);
  EXPECT_EQ(static_cast<int>(t2.size()), 6);
}

TEST(multiset, MemoryManagment) {
  s21::multiset<int> t = {1, 2, 3, 4, 5, 5};
  EXPECT_EQ(static_cast<int>(t.size()), 6);

  t.clear();
  EXPECT_EQ(static_cast<int>(t.size()), 0);

  std::set<int> s;
  // на разных системах могут не сходить значения
  // EXPECT_EQ(s.max_size(), t.max_size());
}

TEST(multiset, UsingIterator) {
  s21::multiset<int> t = {1,   5,  2,   55, -9, -5, 18,   56,  10, 3,
                          13,  23, 27,  20, 22, 57, 60,   59,  75, -10,
                          -11, 55, -11, 1,  1,  1,  -100, -99, -88};

  s21::array<int, 40> res = {-100, -99, -88, -11, -11, -10, -9, -5, 1,  1,
                             1,    1,   2,   3,   5,   10,  13, 18, 20, 22,
                             23,   27,  55,  55,  56,  57,  59, 60, 75};

  int i = 0;
  for (auto it = t.begin(); it != t.end(); ++it, ++i) {
    EXPECT_EQ(*it, res[i]);
  }

  s21::multiset<int> x;
  //     EXPECT_THROW(*x.cbegin(), std::domain_error);
  //     EXPECT_THROW(*x.cend(), std::domain_error);
  //     EXPECT_THROW(*x.begin(), std::domain_error);
  //     EXPECT_THROW(*x.end(), std::domain_error);
  //     EXPECT_THROW(*(--x.end()), std::domain_error);

  s21::multiset<int> z = {1,   5,  2,   55, -9, -5, 18,   56,  10, 3,
                          13,  23, 27,  20, 22, 57, 60,   59,  75, -10,
                          -11, 55, -11, 1,  1,  1,  -100, -99, -88};
  s21::array<int, 30> res_reverse = {75, 60, 59, 57,  56,  55,  55,  27, 23, 22,
                                     20, 18, 13, 10,  5,   3,   2,   1,  1,  1,
                                     1,  -5, -9, -10, -11, -11, -88, -99};

  int j = 0;
  for (auto it = --z.cend(); it != z.cbegin(); --it, ++j) {
    EXPECT_EQ(*it, res_reverse[j]);
  }
}

TEST(multiset, Erase) {
  s21::multiset<int> t = {1,  5,   2,   55, -9,  -5, 18, 56, 10,
                          3,  13,  23,  27, 20,  22, 57, 60, 59,
                          75, -10, -11, 55, -11, 1,  1,  1,  12};

  t.erase(t.begin());
  t.erase(t.find(1));
  t.erase(t.find(10));
  t.erase(t.find(13));
  t.erase(t.find(56));

  s21::array<int, 22> res = {-11, -10, -9, -5, 1,  1,  1,  2,  3,  5,  12,
                             18,  20,  22, 23, 27, 55, 55, 57, 59, 60, 75};

  EXPECT_EQ(static_cast<int>(t.size()), 22);

  int i = 0;
  for (auto it = t.begin(); it != t.end(); ++it, ++i) {
    EXPECT_EQ(*it, res[i]);
  }
}

TEST(multiset, Merge) {
  s21::multiset<int> t1 = {1,  5,   2,   55, -9,  -5, 18, 56, 10,
                           3,  13,  23,  27, 20,  22, 57, 60, 59,
                           75, -10, -11, 55, -11, 1,  1,  1};
  s21::multiset<int> t2 = {-100, -99, -88, -88};

  s21::array<int, 40> res = {-100, -99, -88, -88, -11, -11, -10, -9, -5, 1,
                             1,    1,   1,   2,   3,   5,   10,  13, 18, 20,
                             22,   23,  27,  55,  55,  56,  57,  59, 60, 75};

  t1.merge(t2);

  int i = 0;
  for (auto it = t1.begin(); it != t1.end(); ++it, ++i) {
    EXPECT_EQ(*it, res[i]);
  }

  EXPECT_TRUE(t2.empty());
}

TEST(multiset, Contains) {
  s21::multiset<int> t = {1,  5,  2,  55, -9, -5, 18,  56,  10, 3,   13, 23, 27,
                          20, 22, 57, 60, 59, 75, -10, -11, 55, -11, 1,  1,  1};

  EXPECT_EQ(static_cast<int>(t.count(1)), 4);
  EXPECT_EQ(static_cast<int>(t.count(-1000)), 0);

  //     EXPECT_THROW(*t.find(-1000), std::domain_error);
  EXPECT_TRUE(t.contains(55));
}

TEST(multiset, Range) {
  s21::multiset<int> t = {1,  5,  2,  55, -9, -5, 18,  56,  10, 3,   13, 23, 27,
                          20, 22, 57, 60, 59, 75, -10, -11, 55, -11, 1,  1,  1};

  auto test = t.lower_bound(19);

  EXPECT_EQ(*test, 20);
  EXPECT_EQ(*(t.lower_bound(18)), 18);
  EXPECT_EQ(*(t.upper_bound(1)), 2);

  auto pair1 = t.equal_range(1);
  auto pair2 = t.equal_range(-500);
  auto pair3 = t.equal_range(56);
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
  for (auto it = pair1.first; it != pair1.second; ++it) {
    ++i1;
  }

  for (auto it = pair2.first; it != pair2.second; ++it) {
    ++i2;
  }
  for (auto it = pair3.first; it != pair3.second; ++it) {
    ++i3;
  }

  EXPECT_EQ(i1, 4);
  EXPECT_EQ(i2, 0);
  EXPECT_EQ(i3, 1);
}
