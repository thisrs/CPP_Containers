// Copyright 2022 nolanhea
#include <list>

#include "gtest/gtest.h"
#include "list.h"
// The fixture for testing class Foo.
class ListTest : public ::testing::Test {};
// Tests that the Foo::Bar() method does Abc.
TEST_F(ListTest, constructors_0) {
  std::list<int> original(10);
  s21::list<int> my(10);
  auto my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
}
TEST_F(ListTest, constructors_1) {
  std::list<int> original({1, 2, 3, 4, 5});
  s21::list<int> my({1, 2, 3, 4, 5});
  auto my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
}
TEST_F(ListTest, constructors_2) {
  const std::list<int> original({1, 2, 3, 4, 5});
  const s21::list<int> my({1, 2, 3, 4, 5});
  auto my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
}
TEST_F(ListTest, Size_0) {
  // Creating list from std library and inserting 3 values:
  std::list<int> original;
  EXPECT_EQ(original.size(), 0);
  EXPECT_EQ(original.empty(), 1);
  original.insert(original.begin(), 3);
  original.insert(original.begin(), 2);
  original.insert(original.begin(), 1);
  EXPECT_EQ(original.size(), 3);
  original.erase(original.begin());
  EXPECT_EQ(original.size(), 2);
  EXPECT_EQ(original.empty(), 0);
}
TEST_F(ListTest, ListInsert_0) {
  // Creating list from std library and inserting 3 values:
  std::list<int> original;
  original.insert(original.begin(), 3);
  original.insert(original.begin(), 2);
  original.insert(original.begin(), 1);
  // Creating list from s21 namespace and inserting 3 values, identical to
  // previous:
  s21::list<int> my;

  my.insert(my.begin(), 3);
  my.insert(my.begin(), 2);
  my.insert(my.begin(), 1);
  // Expecting created lists from std and s21 namespaces to hold similar values:
  auto original_iterator = original.begin();
  auto my_iterator = my.begin();
  EXPECT_EQ(*(original_iterator++), *(my_iterator++));
  EXPECT_EQ(*(original_iterator++), *(my_iterator++));
  EXPECT_EQ(*(original_iterator), *(my_iterator));
}
TEST_F(ListTest, ListErase_0) {
  // Creating list from std library and inserting 3 values:
  std::list<char> original;
  original.insert(original.begin(), 'a');
  original.insert(original.begin(), 'b');
  original.insert(++original.begin(), 'd');
  original.insert(original.begin(), 'c');
  original.insert(original.end(), 'e');
  original.insert(--original.end(), 'f');

  original.erase(--original.end());
  original.erase(original.begin());
  original.erase(--original.end());
  // Creating list from s21 namespace and inserting 3 values, identical to
  // previous:
  s21::list<char> my;
  my.insert(my.begin(), 'a');
  my.insert(my.begin(), 'b');
  my.insert(++my.begin(), 'd');
  my.insert(my.begin(), 'c');
  my.insert(my.end(), 'e');
  my.insert(--my.end(), 'f');

  my.erase(--my.end());
  my.erase(my.begin());
  my.erase(--my.end());
  // Expecting created lists from std and s21 namespaces to hold similar values:
  auto my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
}
TEST_F(ListTest, PushPop_0) {
  std::list<char> original;
  original.push_back('a');
  original.push_front('b');
  original.push_back('c');
  original.push_front('d');
  s21::list<char> my;
  my.push_back('a');
  my.push_front('b');
  my.push_back('c');
  my.push_front('d');
  auto my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
  original.pop_back();
  original.pop_front();
  my.pop_back();
  my.pop_front();
  my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
}
TEST_F(ListTest, Emplace) {
  std::list<char> original;
  original.emplace_back('a');
  original.emplace_front('b');
  original.emplace_back('a');
  original.emplace_front('b');
  original.emplace(original.begin()++, 'c');
  s21::list<char> my;
  my.emplace_back('a');
  my.emplace_front('b');
  my.emplace_back('a');
  my.emplace_front('b');
  my.emplace(my.begin()++, 'c');
  auto my_iterator = my.begin();
  for (auto i : original) {
    EXPECT_EQ(i, *my_iterator);
    my_iterator++;
  }
}

//  TEST_F(ListTest, MaxSize) {
//  s21::list<int> my;
//  auto max = my.max_size();
//
//  std::list<int> other;
//  auto max2 = other.max_size();
//  EXPECT_EQ(max, max2);
//}
