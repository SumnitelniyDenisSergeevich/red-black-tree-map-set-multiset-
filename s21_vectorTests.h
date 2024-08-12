#pragma once

#include <gtest/gtest.h>

#include "s21_vector.h"

// VECTOR
TEST(vector, DefaultConstructor) {
  s21::vector<int> t;
  EXPECT_TRUE(t.empty());
  EXPECT_EQ(static_cast<int>(t.size()), 0);
  EXPECT_EQ(static_cast<int>(t.capacity()), 0);
}

TEST(vector, InitializerListConstructor) {
  s21::vector<int> t = {1, 2, 3, 4, 5};
  EXPECT_FALSE(t.empty());
  EXPECT_EQ(static_cast<int>(t.size()), 5);
  EXPECT_EQ(static_cast<int>(t.capacity()), 5);

  EXPECT_EQ(t[0], 1);
  EXPECT_EQ(t[4], 5);
}

TEST(vector, CopyConstructor) {
  s21::vector<int> t1 = {1, 2, 3, 4, 5};
  s21::vector<int> t2 = t1;
  EXPECT_EQ(static_cast<int>(t1.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.size()), 5);

  EXPECT_EQ(static_cast<int>(t1.capacity()), 5);
  EXPECT_EQ(static_cast<int>(t2.capacity()), 5);

  EXPECT_EQ(t1[0], 1);
  EXPECT_EQ(t2[0], 1);
}

TEST(vector, MoveConstructor) {
  s21::vector<int> t1 = {1, 2, 3, 4, 5};
  s21::vector<int> t2 = std::move(t1);

  EXPECT_TRUE(t1.empty());
  EXPECT_EQ(static_cast<int>(t1.size()), 0);
  EXPECT_EQ(static_cast<int>(t1.capacity()), 0);
  EXPECT_FALSE(t2.empty());
  EXPECT_EQ(static_cast<int>(t2.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.capacity()), 5);

  EXPECT_EQ(t2[2], 3);
}

TEST(vector, AssignmentOperatorCopy) {
  s21::vector<int> t1 = {1, 2, 3, 4, 5};
  s21::vector<int> t2 = {6, 7, 8};

  EXPECT_EQ(static_cast<int>(t1.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.size()), 3);
  t2.pop_back();
  EXPECT_EQ(t2[t2.size() - 1], 7);
  t2.push_back(9);
  EXPECT_EQ(t2[t2.size() - 1], 9);

  t2 = t1;
  EXPECT_EQ(static_cast<int>(t1.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.size()), 5);

  EXPECT_EQ(t2[t2.size() - 1], 5);
  EXPECT_EQ(t2[t2.size() - 1], 5);

  s21::vector<int> t3 = {10, 11, 12, 14, 15, 16, 17};
  EXPECT_EQ(static_cast<int>(t3.size()), 7);
  EXPECT_EQ(static_cast<int>(t3.capacity()), 7);

  t3 = t2;
  EXPECT_EQ(static_cast<int>(t3.size()), 5);
  EXPECT_EQ(static_cast<int>(t3.capacity()), 7);
}

TEST(vector, AssignmentOperatorMove) {
  s21::vector<int> t1 = {1, 2, 3, 4, 5};
  s21::vector<int> t2 = {6, 7, 8};

  EXPECT_EQ(static_cast<int>(t1.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.size()), 3);

  EXPECT_EQ(static_cast<int>(t1.capacity()), 5);
  EXPECT_EQ(static_cast<int>(t2.capacity()), 3);

  t2 = std::move(t1);

  EXPECT_TRUE(t1.empty());
  EXPECT_EQ(static_cast<int>(t1.size()), 0);
  EXPECT_EQ(static_cast<int>(t1.capacity()), 0);

  EXPECT_EQ(static_cast<int>(t2.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.capacity()), 5);

  EXPECT_EQ(t2[t2.size() - 1], 5);
}

TEST(vector, Insert) {
  s21::vector<int> t = {1, 2, 3, 4, 5};

  EXPECT_EQ(static_cast<int>(t.size()), 5);
  EXPECT_EQ(static_cast<int>(t.capacity()), 5);
  t.insert(t.begin(), 10);

  EXPECT_EQ(static_cast<int>(t.size()), 6);
  EXPECT_EQ(static_cast<int>(t.capacity()), 10);

  EXPECT_EQ(t[0], 10);
  t.insert(t.begin(), 11);
  t.insert(t.begin(), 12);
  t.insert(t.begin(), 13);
  t.insert(t.begin(), 14);
  EXPECT_EQ(static_cast<int>(t.size()), 10);
  EXPECT_EQ(static_cast<int>(t.capacity()), 10);
  t.insert(t.begin(), 15);

  EXPECT_EQ(static_cast<int>(t.size()), 11);
  EXPECT_EQ(static_cast<int>(t.capacity()), 20);
  EXPECT_EQ(t[0], 15);
}

TEST(vector, InsertMany) {
  s21::vector<int> t = {1, 2, 3, 4, 5};
  EXPECT_EQ(t[3], 4);
  EXPECT_EQ(static_cast<int>(t.size()), 5);
  t.insert_many(t.cbegin(), 6, 7, 8);
  EXPECT_EQ(static_cast<int>(t.size()), 8);
  EXPECT_EQ(*(t.cbegin()), 6);

  t.insert_many_back(100, 90, 200);
  EXPECT_EQ(static_cast<int>(t.size()), 11);
  EXPECT_EQ(static_cast<int>(t.capacity()), 20);
  EXPECT_EQ(*(t.cend() - 1), 200);
}

TEST(vector, Swap) {
  s21::vector<int> t1 = {1, 2, 3, 4, 5};
  s21::vector<int> t2 = {6, 7, 8};
  EXPECT_EQ(static_cast<int>(t1.size()), 5);
  EXPECT_EQ(static_cast<int>(t2.size()), 3);
  EXPECT_EQ(static_cast<int>(t1.capacity()), 5);
  EXPECT_EQ(static_cast<int>(t2.capacity()), 3);

  t1.swap(t2);
  EXPECT_EQ(static_cast<int>(t1.size()), 3);
  EXPECT_EQ(static_cast<int>(t2.size()), 5);
  EXPECT_EQ(static_cast<int>(t1.capacity()), 3);
  EXPECT_EQ(static_cast<int>(t2.capacity()), 5);
}

TEST(vector, AllocateMemory) {
  s21::vector<int> t;
  EXPECT_EQ(static_cast<int>(t.size()), 0);
  EXPECT_EQ(static_cast<int>(t.capacity()), 0);

  t.push_back(99);
  EXPECT_EQ(static_cast<int>(t.size()), 1);
  EXPECT_EQ(static_cast<int>(t.capacity()), 1);

  t.push_back(100);
  EXPECT_EQ(static_cast<int>(t.size()), 2);
  EXPECT_EQ(static_cast<int>(t.capacity()), 2);

  t.push_back(101);
  EXPECT_EQ(static_cast<int>(t.size()), 3);
  EXPECT_EQ(static_cast<int>(t.capacity()), 4);

  t.push_back(102);
  EXPECT_EQ(static_cast<int>(t.size()), 4);
  EXPECT_EQ(static_cast<int>(t.capacity()), 4);

  t.push_back(103);
  EXPECT_EQ(static_cast<int>(t.size()), 5);
  EXPECT_EQ(static_cast<int>(t.capacity()), 8);

  t.insert_many(t.cend(), 104, 105, 106, 107);
  EXPECT_EQ(static_cast<int>(t.size()), 9);
  EXPECT_EQ(static_cast<int>(t.capacity()), 16);

  t.clear();
  EXPECT_EQ(static_cast<int>(t.size()), 0);
  EXPECT_EQ(static_cast<int>(t.capacity()), 16);
}

TEST(vector, MemoryManagment) {
  s21::vector<int> t = {1, 2, 3, 4, 5};
  EXPECT_EQ(static_cast<int>(t.size()), 5);
  EXPECT_EQ(static_cast<int>(t.capacity()), 5);
  t.reserve(20);

  EXPECT_EQ(static_cast<int>(t.size()), 5);
  EXPECT_EQ(static_cast<int>(t.capacity()), 20);

  t.push_back(6);
  EXPECT_EQ(static_cast<int>(t.size()), 6);
  EXPECT_EQ(static_cast<int>(t.capacity()), 20);

  t.shrink_to_fit();
  EXPECT_EQ(static_cast<int>(t.size()), 6);
  EXPECT_EQ(static_cast<int>(t.capacity()), 6);

  std::vector<int> v;
  // на разных системах могут не сходить значения
  //     EXPECT_EQ(v.max_size(), t.max_size());
}

TEST(vector, UsingIterator) {
  s21::vector<int> t = {1, 2, 3, 4, 5};
  int i = 0;
  for (auto it = t.begin(); it != t.end(); ++it, ++i) {
    EXPECT_EQ(*it, t[i]);
  }

  EXPECT_EQ(*(t.cbegin() + 2), 3);
  EXPECT_EQ(*(t.cend() - 2), 4);
}

TEST(vector, View) {
  s21::vector<int> t = {1, 2, 3, 4, 5};

  EXPECT_EQ(t.at(t.size() - 1), t[t.size() - 1]);
  EXPECT_THROW(t.at(t.size()), std::out_of_range);
  EXPECT_EQ(*t.data(), t[0]);

  EXPECT_EQ(t.front(), 1);
  EXPECT_EQ(t.back(), 5);
}

TEST(vector, Erase) {
  s21::vector<int> t = {1, 2, 3, 4, 5};
  t.erase(t.begin());

  EXPECT_EQ(t.front(), 2);
}
