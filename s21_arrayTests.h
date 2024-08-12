#pragma once

#include <gtest/gtest.h>

#include <array>

#include "s21_array.h"

TEST(array, DefaultConstructor) {
  s21::array<int, 5> arr;
  // Check that the size is correct
  EXPECT_EQ(arr.size(), 5);

  // Check that all elements are initialized to their default value (0 for int)
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], 0);
  }
}

TEST(array, InitListConstructor) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};

  // Check that the size is correct
  EXPECT_EQ(arr.size(), 5);

  // Check that each element is initialized correctly
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], static_cast<int>(i) + 1);
  }
}

TEST(array, CopyConstructor) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  s21::array<int, 5> arrCopy(arr);

  // Check that the size is correct
  EXPECT_EQ(arrCopy.size(), 5);

  // Check that the elements are copied correctly
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arrCopy[i], arr[i]);
  }
}

TEST(array, MoveConstructor) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  s21::array<int, 5> arrMoved(std::move(arr));

  // Check that the size is correct
  EXPECT_EQ(arrMoved.size(), 5);

  // Check that the elements are moved correctly
  for (size_t i = 0; i < arrMoved.size(); ++i) {
    EXPECT_EQ(arrMoved[i], static_cast<int>(i) + 1);
  }
}

TEST(array, Begin) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.begin();
  EXPECT_EQ(*it, 1);
  // Modify the data through the iterator and check if the array is updated
  *it = 10;
  EXPECT_EQ(arr[0], 10);
}

TEST(array, End) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.end();
  // end() iterator should not be dereferenced; comparing it to another iterator
  // should work
  auto beginIt = arr.begin();
  EXPECT_EQ(it, beginIt + 5);
}

TEST(array, CBegin) {
  const s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.cbegin();
  EXPECT_EQ(*it, 1);
  // Attempting to modify data through a const_iterator should result in a
  // compilation error Uncommenting the following line should result in a
  // compilation error. *it = 10;
}

TEST(array, CEnd) {
  const s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.cend();
  // cend() iterator should not be dereferenced; comparing it to another
  // iterator should work
  auto beginIt = arr.cbegin();
  EXPECT_EQ(it, beginIt + 5);
}

TEST(array, At_ValidIndex) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  int value = arr.at(2);
  EXPECT_EQ(value, 3);
}

TEST(array, At_OutOfBounds) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  // Attempting to access an out-of-bounds index using at() should throw an
  // exception
  EXPECT_THROW(arr.at(10), std::out_of_range);
}

TEST(array, ConstAt_ValidIndex) {
  const s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  int value = arr.at(2);
  EXPECT_EQ(value, 3);
}

TEST(array, ConstAt_OutOfBounds) {
  const s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  // Attempting to access an out-of-bounds index using const at() should throw
  // an exception
  EXPECT_THROW(arr.at(10), std::out_of_range);
}

TEST(array, Front) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  int value = arr.front();
  EXPECT_EQ(value, 1);
}

TEST(array, Back) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  int value = arr.back();
  EXPECT_EQ(value, 5);
}

TEST(array, ConstFront) {
  const s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  int value = arr.front();
  EXPECT_EQ(value, 1);
}

TEST(array, ConstBack) {
  const s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  int value = arr.back();
  EXPECT_EQ(value, 5);
}

TEST(array, Empty_NonEmptyArray) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  EXPECT_FALSE(arr.empty());
}

TEST(array, Empty_EmptyArray) {
  s21::array<int, 0> arr;
  EXPECT_TRUE(arr.empty());
}

TEST(array, Size_NonEmptyArray) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(arr.size(), 5);
}

TEST(array, Size_EmptyArray) {
  s21::array<int, 0> arr;
  EXPECT_EQ(arr.size(), 0);
}

TEST(array, MaxSize) {
  s21::array<int, 5> arr;
  // The max size of an array with a specified size is the same as its declared
  // size
  EXPECT_EQ(arr.max_size(), 5);
}

TEST(array, Swap) {
  s21::array<int, 5> arr1 = {1, 2, 3, 4, 5};
  s21::array<int, 5> arr2 = {6, 7, 8, 9, 10};

  arr1.swap(arr2);

  // Check that the elements of arr1 are now from arr2
  for (size_t i = 0; i < arr1.size(); ++i) {
    EXPECT_EQ(arr1[i], static_cast<int>(i) + 6);
  }

  // Check that the elements of arr2 are now from arr1
  for (size_t i = 0; i < arr2.size(); ++i) {
    EXPECT_EQ(arr2[i], static_cast<int>(i) + 1);
  }
}

TEST(array, Fill) {
  s21::array<int, 5> arr;
  arr.fill(10);

  // Check that all elements are filled with the specified value
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], 10);
  }
}

TEST(array, CopyAssignmentOperator) {
  s21::array<int, 5> arr1 = {1, 2, 3, 4, 5};
  s21::array<int, 5> arr2 = {6, 7, 8, 9, 10};

  arr1 = arr2;

  // Check that arr1 now contains the same elements as arr2
  for (size_t i = 0; i < arr1.size(); ++i) {
    EXPECT_EQ(arr1[i], arr2[i]);
  }
}

TEST(array, MoveAssignmentOperator) {
  s21::array<int, 5> arr1 = {1, 2, 3, 4, 5};
  s21::array<int, 5> arr2 = {6, 7, 8, 9, 10};

  arr1 = std::move(arr2);

  // Check that arr1 now contains the same elements as arr2
  for (size_t i = 0; i < arr1.size(); ++i) {
    EXPECT_EQ(arr1[i], static_cast<int>(i) + 6);
  }
}

TEST(array, OperatorBracket) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};

  // Check that elements can be accessed using the [] operator
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], static_cast<int>(i) + 1);
  }

  // Modify an element using the [] operator and check if the array is updated
  arr[2] = 10;
  EXPECT_EQ(arr[2], 10);
}
