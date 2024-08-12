#pragma once

#include <gtest/gtest.h>

#include <map>

#include "s21_map.h"

// MAP
TEST(map, ConstructorDefaultMap) {
  s21::map<int, char> my_empty_map;
  std::map<int, char> orig_empty_map;
  EXPECT_EQ(my_empty_map.empty(), orig_empty_map.empty());
}

TEST(map, ConstructorInitializerMap) {
  s21::map<int, char> my_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  std::map<int, char> orig_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  while (my_it != my_map.end() || orig_it != orig_map.end()) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
    my_it++;
    orig_it++;
  }
}

TEST(map, ConstructorInitializer2Map) {
  s21::map<int, char> my_map = {};
  std::map<int, char> orig_map = {};
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, ConstructorCopyMap) {
  s21::map<int, int> my_map = {{1, 2}, {3, 4}, {5, 6}};
  std::map<int, int> orig_map = {{1, 2}, {3, 4}, {5, 6}};
  s21::map<int, int> my_map_copy = my_map;
  std::map<int, int> orig_map_copy = orig_map;
  EXPECT_EQ(my_map_copy.size(), orig_map_copy.size());
  auto my_it = my_map_copy.begin();
  auto orig_it = orig_map_copy.begin();
  for (; my_it != my_map_copy.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, ConstructorMoveMap) {
  s21::map<int, int> my_map = {{1, 2}, {3, 4}, {5, 6}};
  std::map<int, int> orig_map = {{1, 2}, {3, 4}, {5, 6}};
  s21::map<int, int> my_map_copy = std::move(my_map);
  std::map<int, int> orig_map_copy = std::move(orig_map);
  EXPECT_EQ(my_map.size(), orig_map.size());
  EXPECT_EQ(my_map_copy.size(), orig_map_copy.size());
  auto my_it = my_map_copy.begin();
  auto orig_it = orig_map_copy.begin();
  for (; my_it != my_map_copy.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, MapOperator) {
  s21::map<char, std::string> my_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  std::map<char, std::string> orig_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};

  my_map['a'] = "Alisa";
  orig_map['a'] = "Alisa";
  orig_map['b'] = "Ben";
  EXPECT_TRUE(my_map['a'] == orig_map['a']);
  EXPECT_FALSE(my_map['b'] == orig_map['b']);
  EXPECT_TRUE(my_map['c'] == orig_map['c']);
}

TEST(map, MapAtOperatorException) {
  s21::map<char, std::string> my_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  EXPECT_THROW(my_map.at('g') = "Alisa", std::out_of_range);
}

TEST(map, MapAtOperator) {
  s21::map<char, std::string> my_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  std::map<char, std::string> orig_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  my_map.at('a') = "Alisa";
  orig_map.at('a') = "Alisa";
  orig_map.at('b') = "Ben";
  EXPECT_TRUE(my_map['a'] == orig_map['a']);
  EXPECT_FALSE(my_map['b'] == orig_map['b']);
  EXPECT_TRUE(my_map['c'] == orig_map['c']);
}

TEST(map, MapCapacity) {
  s21::map<char, std::string> my_map;
  std::map<char, std::string> orig_map;
  EXPECT_TRUE(my_map.empty() == orig_map.empty());
  my_map.insert('z', "wow");
  EXPECT_FALSE(my_map.empty() == orig_map.empty());
  EXPECT_EQ(my_map.size(), 1);
}

TEST(map, MapClear) {
  s21::map<int, int> my_map;
  std::map<int, int> orig_map;
  my_map.clear();
  orig_map.clear();
  EXPECT_EQ(my_map.empty(), orig_map.empty());
  my_map.insert(std::make_pair(1, 1));
  orig_map.insert(std::make_pair(1, 1));
  EXPECT_EQ(my_map.empty(), orig_map.empty());
  my_map.clear();
  orig_map.clear();
  EXPECT_EQ(my_map.empty(), orig_map.empty());
}

TEST(map, MapInsert1) {
  s21::map<int, char> my_map;
  std::map<int, char> orig_map;
  my_map.insert(std::make_pair(1, 'a'));
  my_map.insert(std::make_pair(2, 'a'));
  my_map.insert(std::make_pair(3, 'a'));
  orig_map.insert(std::make_pair(1, 'a'));
  orig_map.insert(std::make_pair(2, 'a'));
  orig_map.insert(std::make_pair(3, 'a'));

  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }

  auto pr1 = my_map.insert(std::make_pair(1, 'a'));
  auto pr2 = orig_map.insert(std::make_pair(1, 'a'));
  EXPECT_TRUE(pr1.second == pr2.second);
}

TEST(map, MapInsert2) {
  s21::map<int, char> my_map;
  std::map<int, char> orig_map;
  my_map.insert(1, 'a');
  my_map.insert(2, 'a');
  my_map.insert(3, 'a');
  orig_map.insert(std::make_pair(1, 'a'));
  orig_map.insert(std::make_pair(2, 'a'));
  orig_map.insert(std::make_pair(3, 'a'));

  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }

  auto pr1 = my_map.insert(1, 'a');
  auto pr2 = orig_map.insert(std::make_pair(1, 'a'));
  EXPECT_TRUE(pr1.second == pr2.second);
}

TEST(map, MapInsert3) {
  s21::map<int, char> my_map;
  std::map<int, char> orig_map;
  my_map.insert(1, 'a');
  my_map.insert(2, 'a');
  my_map.insert(3, 'a');
  orig_map.insert(std::make_pair(1, 'a'));
  orig_map.insert(std::make_pair(2, 'a'));
  orig_map.insert(std::make_pair(3, 'a'));

  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }

  auto pr1 = my_map.insert_or_assign(1, 'b');
  auto i = orig_map.begin();
  EXPECT_TRUE((*pr1.first).first == (*i).first);
  EXPECT_FALSE((*pr1.first).second == (*i).second);
}

TEST(map, MapErase) {
  s21::map<int, char> my_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  std::map<int, char> orig_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  EXPECT_EQ(my_map.size(), orig_map.size());
  my_map.erase(my_map.begin());
  orig_map.erase(orig_map.begin());
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, MapEraseRootNoChildren) {
  s21::map<int, char> my_map = {{1, 'x'}};
  std::map<int, char> orig_map = {{1, 'x'}};
  EXPECT_EQ(my_map.size(), orig_map.size());
  my_map.erase(my_map.begin());
  orig_map.erase(orig_map.begin());
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, MapEraseRootOnLeftChild) {
  s21::map<int, char> my_map = {{2, 'x'}, {1, 'x'}};
  std::map<int, char> orig_map = {{2, 'x'}, {1, 'x'}};
  EXPECT_EQ(my_map.size(), orig_map.size());
  my_map.erase(++my_map.begin());
  orig_map.erase(2);
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, MapEraseRootOnRightChild) {
  s21::map<int, char> my_map = {{1, 'x'}, {2, 'x'}};
  std::map<int, char> orig_map = {{1, 'x'}, {2, 'x'}};
  EXPECT_EQ(my_map.size(), orig_map.size());
  my_map.erase(my_map.begin());
  orig_map.erase(1);
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, MapEraseRootTwoChildren) {
  s21::map<int, char> my_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  std::map<int, char> orig_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  EXPECT_EQ(my_map.size(), orig_map.size());
  my_map.erase((++my_map.begin()));
  orig_map.erase(2);
  EXPECT_EQ(my_map.size(), orig_map.size());
  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
}

TEST(map, SwapMap) {
  s21::map<int, int> my_map = {{1, 1}};
  s21::map<int, int> my_swap_map = {{3, 3}, {4, 4}};

  my_map.swap(my_swap_map);
  EXPECT_EQ(my_map.size(), 2);
  EXPECT_EQ(my_swap_map.size(), 1);
  auto x = (*(my_map.begin())).first;
  auto y = (*(my_swap_map.begin())).first;
  EXPECT_EQ(x, 3);
  EXPECT_EQ(y, 1);
}

TEST(map, MergeMap) {
  s21::map<int, int> my_map = {{1, 1}, {4, 4}, {2, 2}};
  s21::map<int, int> my_map_merge = {{3, 3}, {4, 4}};

  std::map<int, int> orig_map = {{1, 1}, {4, 4}, {2, 2}};
  std::map<int, int> orig_map_merge = {{3, 3}, {4, 4}};

  my_map.merge(my_map_merge);
  orig_map.merge(orig_map_merge);

  auto my_it = my_map.begin();
  auto orig_it = orig_map.begin();
  for (; my_it != my_map.end(); ++my_it, ++orig_it) {
    EXPECT_TRUE((*my_it).first == (*orig_it).first);
    EXPECT_TRUE((*my_it).second == (*orig_it).second);
  }
  EXPECT_EQ(my_map_merge.contains(4), (orig_map_merge.count(4) == 1));
  EXPECT_EQ(my_map_merge.contains(3), (orig_map_merge.count(3) == 0));
}

TEST(map, DefaultConstructor) {
  // Проверяем, что конструктор по умолчанию создает пустую карту
  s21::map<int, std::string> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(static_cast<int>(map.size()), (0));
}

TEST(map, InitializerListConstructor) {
  // Проверяем конструктор с инициализацией из списка инициализации
  s21::map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  EXPECT_FALSE(map.empty());
  EXPECT_EQ(static_cast<int>(map.size()), 3);
}

TEST(map, CopyConstructor) {
  // Проверяем копирующий конструктор
  s21::map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> map_copy(map);
  EXPECT_EQ(map.size(), map_copy.size());
  // Проверяем, что значения в копии равны значениям в оригинале
  for (const auto &pair : map) {
    EXPECT_EQ(map_copy.at(pair.first), pair.second);
  }
}

TEST(map, erase) {
  s21::map<int, int> map = {
      {10, 10}, {46, 46}, {19, 19}, {17, 17}, {23, 23}, {14, 14}, {49, 49},
      {30, 30}, {38, 38}, {22, 22}, {50, 50}, {43, 43}, {27, 27}, {13, 13},
      {5, 5},   {33, 33}, {20, 20}, {15, 15}, {29, 29}, {36, 36}, {37, 37},
      {7, 7},   {25, 25}, {44, 44}, {2, 2},   {41, 41}, {48, 48}, {21, 21},
      {39, 39}, {1, 1},   {26, 26}, {11, 11}, {47, 47}, {34, 34}, {9, 9},
      {8, 8},   {31, 31}, {16, 16}, {12, 12}, {45, 45}, {6, 6},   {3, 3},
      {42, 42}, {24, 24}, {28, 28}, {35, 35}, {4, 4},   {32, 32}, {40, 40},
      {18, 18}};

  EXPECT_FALSE(map.empty());

  auto it = map.begin();
  auto end = map.end();

  // Ищу нужный итератор ноды для удаления
  while (it != end) {
    if ((*it).first == 16) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 24) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 40) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 25) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 25) {
      map.erase(it);
      break;
    }
    ++it;
  }

  it = map.begin();
  end = map.end();

  while (it != end) {
    if ((*it).first == 32) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 1) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 5) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  while (it != end) {
    if ((*it).first == 30) {
      map.erase(it);
      it = map.begin();
      end = map.end();
      break;
    }
    ++it;
  }

  // Проверяю, что нод теперь 3
  EXPECT_EQ(static_cast<int>(map.size()), 42);
}

TEST(map, Merge) {
  // Создаем две карты
  s21::map<int, int> map1 = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5},
                             {6, 6}, {7, 7}, {8, 8}, {9, 9}, {10, 10}};
  s21::map<int, int> map2 = {{11, 11}, {12, 12}, {13, 13}, {14, 14}, {15, 15},
                             {16, 16}, {17, 17}, {18, 18}, {19, 19}, {20, 20}};

  // Объединяем карты
  map1.merge(map2);

  // Проверяем, что карты были успешно объединены
  EXPECT_EQ(static_cast<int>(map1.size()), 20);
  EXPECT_EQ(static_cast<int>(map2.size()), 10);

  // Проверяем, что все элементы из второй карты перенеслись в первую
  for (int i = 1; i <= 20; ++i) {
    EXPECT_EQ(map1.at(i), i);
  }
}

TEST(map, InsertOrAssign) {
  s21::map<int, int> map = {
      {10, 10}, {46, 46}, {19, 19}, {17, 17}, {23, 23}, {14, 14}, {49, 49},
      {30, 30}, {38, 38}, {22, 22}, {50, 50}, {43, 43}, {27, 27}, {13, 13},
      {5, 5},   {33, 33}, {20, 20}, {15, 15}, {29, 29}, {36, 36}, {37, 37},
      {7, 7},   {25, 25}, {44, 44}, {2, 2},   {41, 41}, {48, 48}, {21, 21},
      {39, 39}, {1, 1},   {26, 26}, {11, 11}, {47, 47}, {34, 34}, {9, 9},
      {8, 8},   {31, 31}, {16, 16}, {12, 12}, {45, 45}, {6, 6},   {3, 3},
      {42, 42}, {24, 24}, {28, 28}, {35, 35}};
  ;
  map.insert_or_assign(1, 1);
  map.insert_or_assign(2, 2);
  map.insert_or_assign(3, 3);
  map.insert_or_assign(51, 51);
  map.insert_or_assign(4, 4);
  map.insert_or_assign(32, 32);
  map.insert_or_assign(40, 40);
  map.insert_or_assign(18, 18);

  // Проверка наличия вставленных элементов и их значений
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_TRUE(map.contains(3));
  EXPECT_EQ(map.at(1), 1);
  EXPECT_EQ(map.at(2), 2);
  EXPECT_EQ(map.at(3), 3);
  EXPECT_EQ(map.at(51), 51);

  // Проверка перезаписи элемента при повторной вставке с тем же ключом
  map.insert_or_assign(2, 22);
  EXPECT_EQ(map.at(2), 22);
}

TEST(map, Swap) {
  s21::map<int, std::string> map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> map2 = {{4, "four"}, {5, "five"}, {6, "six"}};

  map1.swap(map2);

  // Проверка, что содержимое поменялось местами
  EXPECT_FALSE(map1.contains(1));
  EXPECT_FALSE(map1.contains(2));
  EXPECT_FALSE(map1.contains(3));
  EXPECT_TRUE(map1.contains(4));
  EXPECT_TRUE(map1.contains(5));
  EXPECT_TRUE(map1.contains(6));

  EXPECT_TRUE(map2.contains(1));
  EXPECT_TRUE(map2.contains(2));
  EXPECT_TRUE(map2.contains(3));
  EXPECT_FALSE(map2.contains(4));
  EXPECT_FALSE(map2.contains(5));
  EXPECT_FALSE(map2.contains(6));
}

TEST(map, PostfixDecrement) {
  s21::map<int, int> map = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
  auto it = map.end();
  it--;

  EXPECT_EQ((*it).first, 4);

  it--;
  EXPECT_EQ((*it).first, 3);
}

TEST(MapTest, InsertMany) {
  s21::map<int, std::string> map;

  // Вставляем несколько элементов сразу
  auto results =
      map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                      std::make_pair(3, "three"));

  // Проверяем, что все элементы были успешно вставлены
  EXPECT_EQ(static_cast<int>(results.size()), 3);
  for (const auto &result : results) {
    EXPECT_TRUE(result.second);  // Проверяем, что элемент был успешно вставлен
  }

  // Проверяем наличие всех вставленных элементов и их значения
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_TRUE(map.contains(3));
  EXPECT_EQ(map.at(1), "one");
  EXPECT_EQ(map.at(2), "two");
  EXPECT_EQ(map.at(3), "three");
}

TEST(MapTest, InsertManyVariadic) {
  s21::map<int, std::string> map;

  // Вставляем несколько элементов сразу
  auto results =
      map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                      std::make_pair(3, "three"), std::make_pair(4, "four"));

  // Проверяем, что все элементы были успешно вставлены
  EXPECT_EQ(static_cast<int>(results.size()), 4);
  for (const auto &result : results) {
    EXPECT_TRUE(result.second);  // Проверяем, что элемент был успешно вставлен
  }

  // Проверяем наличие всех вставленных элементов и их значения
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_TRUE(map.contains(3));
  EXPECT_TRUE(map.contains(4));
  EXPECT_EQ(map.at(1), "one");
  EXPECT_EQ(map.at(2), "two");
  EXPECT_EQ(map.at(3), "three");
  EXPECT_EQ(map.at(4), "four");
}

// MAP END
