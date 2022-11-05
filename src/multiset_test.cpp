#include "multiset.h"

#include <cstdlib>
#include <ctime>
#include <string>

#include "gtest/gtest.h"
// Copyright 2022 nolanhea

template <class T>
using multiset = s21::multiset<T>;

template <class T>
using original_multiset = std::multiset<T>;

class MultisetTest : public ::testing::Test {};

TEST(MultisetTest, DefaultConstructorFunctional) {
  multiset<int> A;
  multiset<char> B;
  multiset<multiset<int *>> C;
  EXPECT_EQ(A.size(), 0);
  EXPECT_EQ(B.empty(), true);
  EXPECT_EQ(C.size(), 0);
}
TEST(MultisetTest, DefaultConstructorEfficiency) {
  clock_t my_time = clock();
  for (int i = 0; i < 100000; i++) {
    multiset<int> A;
  }
  my_time = clock() - my_time;

  clock_t original_time = clock();
  for (int i = 0; i < 100000; i++) {
    original_multiset<int> A;
  }
  original_time = clock() - original_time;
  EXPECT_TRUE(my_time < original_time * 2);
}
TEST(MultisetTest, InitlistConstructor) {
  multiset<int> A({});
  multiset<int> B({1, 2, 3, 4, 5, 6, 7, 8, 9});
  multiset<std::string> C({"one", "two", "three", "four"});
}
TEST(MultisetTest, CopyConstructor) {  // add check that D equal to E
  multiset<int> A;
  multiset<int> B(A);
  multiset<int> C({1, 2, 3, 4});
  B = C;
  const multiset<int> D({1, 2, 3, 4});
  const multiset<int> E(D);
}
TEST(MultisetTest, CopyConstructorEfficiency) {
  clock_t my_time = clock();
  for (int i = 0; i < 100; i++) {
    const multiset<int> D({1, 2, 3, 4});
    const multiset<int> E(D);
  }
  my_time = clock() - my_time;
  clock_t original_time = clock();
  for (int i = 0; i < 100; i++) {
    const original_multiset<int> D({1, 2, 3, 4});
    const original_multiset<int> E(D);
  }
  original_time = clock() - original_time;
  EXPECT_TRUE(my_time < original_time * 2);
}
TEST(MultisetTest, AssignmentOperators) {
  multiset<int> A = {1, 2, 3, 4};
  multiset<int> B = A;

  auto a = A.begin();
  auto b = B.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*a, *b);
    a++;
    b++;
  }
  b = B.begin();
  multiset<int> C = std::move(A);
  auto c = C.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*c, *b);
    c++;
    b++;
  }
}
TEST(MultisetTest, AssignmentOperatorsConst) {
  const multiset<int> A = {1, 2, 3, 4};
  const multiset<int> B = A;
  auto a = A.begin();
  auto b = B.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*a, *b);
    a++;
    b++;
  }
  b = B.begin();
  const multiset<int> C = std::move(A);
  auto c = C.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*c, *b);
    c++;
    b++;
  }
}
TEST(MultisetTest, Capacity) {
  multiset<int> A;
  EXPECT_EQ(A.size(), 0);
  EXPECT_EQ(A.empty(), true);
  A = {1, 2, 3, 4, 5, 6, 7, 8};
  EXPECT_EQ(A.size(), 8);
  EXPECT_EQ(A.empty(), false);
  A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  EXPECT_EQ(A.size(), 10);
  EXPECT_EQ(A.empty(), false);
}
TEST(MultisetTest, CapacityEfficiency) {
  clock_t my_time = clock();
  for (int i = 0; i < 1000; i++) {
    multiset<int> A;
    A = {1, 2, 3, 4, 5, 6, 7, 8};
    A.size();
    A.max_size();
  }
  my_time = clock() - my_time;

  clock_t original_time = clock();
  for (int i = 0; i < 1000; i++) {
    original_multiset<int> A;
    A = {1, 2, 3, 4, 5, 6, 7, 8};
    A.size();
    A.max_size();
  }
  original_time = clock() - original_time;
  EXPECT_TRUE(my_time < original_time * 2);
}
TEST(MultisetTest, Insert) {
  multiset<int> A;
  for (int i = 0; i < 100; i++) {
    auto ans = A.insert(i);
    EXPECT_EQ(*(ans), i);
  }
  // A.clear();

  // a = A.begin();
  // for (int i = 0; i < 100; i++) {
  //     a = A.insert(1);
  //     EXPECT_EQ(*a, 1);
  // }
}

TEST(MulitmultisetTest, Erase) {
  multiset<int> A;
  for (int i = 0; i < 1000; i++) {
    A.insert(i);
  }
  auto a = A.begin();
  for (int i = 0; i < 900; i++) {
    A.erase(a);
    a = A.begin();
  }
  a = A.begin();
  for (int i = 900; i < 1000; i++) {
    EXPECT_EQ(*a, i);
    a++;
  }
}
TEST(MulitmultisetTest, EraseEfficiency) {
  multiset<int> A;
  original_multiset<int> B;
  clock_t my_time = clock();
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 10000; i++) {
      A.insert(rand());
    }
    auto a = A.begin();
    for (int i = 0; i < 5000; i++) {
      A.erase(a);
      a = A.begin();
    }
  }
  my_time = clock() - my_time;
  clock_t original_time = clock();
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 10000; i++) {
      B.insert(rand());
    }
    auto b = B.begin();
    for (int i = 0; i < 5000; i++) {
      B.erase(b);
      b = B.begin();
    }
    EXPECT_TRUE(my_time < original_time);
  }
}
TEST(MultisetTest, Swap) {
  multiset<int> A = {1, 2, 3, 4};
  multiset<int> B = {5, 6, 7, 8};
  A.swap(B);
  int e = 5;
  for (auto i : A) {
    EXPECT_EQ(e, i);
    e++;
  }
  e = 1;
  for (auto i : B) {
    EXPECT_EQ(e, i);
    e++;
  }
}

TEST(MultisetTest, Merge) {
  multiset<int> A = {5, 6};
  multiset<int> B = {1, 2, 3, 4};
  A.merge(B);
  auto a = A.begin();
  for (int i = 1; i <= 6; i++) {
    EXPECT_EQ(*a, i);
    a++;
  }
}

TEST(MultisetTest, Count) {
  multiset<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                     3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  int count = A.count(3);
  EXPECT_EQ(count, 11);
  count = A.count(1);
  EXPECT_EQ(count, 1);
  count = A.count(100);
  EXPECT_EQ(count, 0);
}
TEST(MultisetTest, Find) {
  const multiset<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                           3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  auto i = A.find(6);
  EXPECT_EQ(*i, 6);
}
TEST(MultisetTest, bounds) {
  const multiset<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                           3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  auto i = A.lower_bound(3);
  auto j = A.upper_bound(3);
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(*j, 3);
  EXPECT_EQ(*(++j), 4);
}
//
// int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
