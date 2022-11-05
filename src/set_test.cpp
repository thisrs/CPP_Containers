#include "set.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
// Copyright 2022 nolanhea

template <class T>
using set = s21::set<T>;

template <class T>
using original_set = std::set<T>;

class SetTest : public ::testing::Test {};

TEST(SetTest, DefaultConstructorFunctional) {
  set<int> A;
  set<char> B;
  set<set<int *>> C;
  EXPECT_EQ(A.size(), 0);
  EXPECT_EQ(B.empty(), true);
  EXPECT_EQ(C.size(), 0);
}
TEST(SetTest, DefaultConstructorEfficiency) {
  clock_t my_time = clock();
  for (int i = 0; i < 100000; i++) {
    set<int> A;
  }
  my_time = clock() - my_time;

  clock_t original_time = clock();
  for (int i = 0; i < 100000; i++) {
    original_set<int> A;
  }
  original_time = clock() - original_time;
  EXPECT_TRUE(my_time < original_time * 2);
}
TEST(SetTest, InitlistConstructor) {
  set<int> A({});
  set<int> B({1, 2, 3, 4, 5, 6, 7, 8, 9});
  set<std::string> C({"one", "two", "three", "four"});
}
TEST(SetTest, CopyConstructor) {  // add check that D equal to E
  set<int> A;
  set<int> B(A);
  set<int> C({1, 2, 3, 4});
  B = C;
  const set<int> D({1, 2, 3, 4});
  const set<int> E(D);
}
TEST(SetTest, CopyConstructorEfficiency) {
  clock_t my_time = clock();
  for (int i = 0; i < 100; i++) {
    const set<int> D({1, 2, 3, 4});
    const set<int> E(D);
  }
  my_time = clock() - my_time;
  clock_t original_time = clock();
  for (int i = 0; i < 100; i++) {
    const original_set<int> D({1, 2, 3, 4});
    const original_set<int> E(D);
  }
  original_time = clock() - original_time;
  EXPECT_TRUE(my_time < original_time * 2);
}
TEST(SetTest, AssignmentOperators) {
  set<int> A = {1, 2, 3, 4};
  set<int> B = A;

  auto a = A.begin();
  auto b = B.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*a, *b);
    a++;
    b++;
  }
  b = B.begin();
  set<int> C = std::move(A);
  auto c = C.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*c, *b);
    c++;
    b++;
  }
}
TEST(SetTest, AssignmentOperatorsConst) {
  const set<int> A = {1, 2, 3, 4};
  const set<int> B = A;
  auto a = A.begin();
  auto b = B.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*a, *b);
    a++;
    b++;
  }
  b = B.begin();
  const set<int> C = std::move(A);
  auto c = C.begin();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(*c, *b);
    c++;
    b++;
  }
}
TEST(SetTest, Capacity) {
  set<int> A;
  EXPECT_EQ(A.size(), 0);
  EXPECT_EQ(A.empty(), true);
  A = {1, 2, 3, 4, 5, 6, 7, 8};
  EXPECT_EQ(A.size(), 8);
  EXPECT_EQ(A.empty(), false);
  A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  EXPECT_EQ(A.size(), 10);
  EXPECT_EQ(A.empty(), false);
}
TEST(SetTest, CapacityEfficiency) {
  clock_t my_time = clock();
  for (int i = 0; i < 1000; i++) {
    set<int> A;
    A = {1, 2, 3, 4, 5, 6, 7, 8};
    A.size();
    A.max_size();
  }
  my_time = clock() - my_time;

  clock_t original_time = clock();
  for (int i = 0; i < 1000; i++) {
    original_set<int> A;
    A = {1, 2, 3, 4, 5, 6, 7, 8};
    A.size();
    A.max_size();
  }
  original_time = clock() - original_time;
  EXPECT_TRUE(my_time < original_time * 2);
}
TEST(SetTest, Insert) {
  set<int> A;
  for (int i = 0; i < 100; i++) {
    auto ans = A.insert(i);
    EXPECT_EQ(*ans.first, i);
  }
}

TEST(MulitsetTest, Erase) {
  set<int> A;
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

TEST(MulitsetTest, Erase2) {
  set<int> A = {1, 2, 3, 4, 5};

  auto it = A.begin();
  it++;
  A.erase(++it);

  EXPECT_FALSE(A.contains(3));
}

TEST(MulitsetTest, Erase3) {
  set<int> A = {1, 2, 3, 4, 5};

  auto it = A.begin();
  it++;
  it++;
  A.erase(++it);

  EXPECT_FALSE(A.contains(4));
}

TEST(MulitsetTest, Erase4) {
  set<int> A = {1, 2, 3, 4, 5, 20, 40, 50, 1000, 11123, 123, 12355, 11111};

  auto it = A.begin();

  A.erase(it);

  EXPECT_FALSE(A.contains(1));
}

TEST(MulitsetTest, Erase5) {
  set<int> A = {1, 2, 3, 4, 5, 20, 40, 50, 1000, 11123, 123, 12355, 11111};

  auto it = A.begin();

  it++;
  it++;
  it++;
  it++;
  it++;
  it++;

  A.erase(it);

  EXPECT_FALSE(A.contains(40));
}

TEST(MulitsetTest, EraseEfficiency) {
  set<int> A;
  original_set<int> B;
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
TEST(SetTest, Swap) {
  set<int> A = {1, 2, 3, 4};
  set<int> B = {5, 6, 7, 8};
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

TEST(SetTest, Merge) {
  set<int> A = {5, 6};
  set<int> B = {1, 2, 3, 4};
  A.merge(B);
  auto a = A.begin();
  for (int i = 1; i <= 6; i++) {
    EXPECT_EQ(*a, i);
    a++;
  }
}

TEST(SetTest, Count) {
  set<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  int count = A.count(3);
  EXPECT_EQ(count, 1);
  count = A.count(1);
  EXPECT_EQ(count, 1);
  count = A.count(100);
  EXPECT_EQ(count, 0);
}
TEST(SetTest, Find) {
  const set<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                      3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  auto i = A.find(6);
  EXPECT_EQ(*i, 6);
}
TEST(SetTest, bounds) {
  const set<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                      3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  auto i = A.lower_bound(3);
  auto j = A.upper_bound(3);
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(*j, 3);
  EXPECT_EQ(*(++j), 4);
}

TEST(SetTest, bounds2) {
  set<int> A = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10};
  auto i = A.lower_bound(3);
  auto j = A.upper_bound(3);
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(*j, 3);
  EXPECT_EQ(*(++j), 4);
}

TEST(SetTest, Emplace) {
  set<int> A = {1, 2, 3, 4};
  original_set<int> B = {1, 2, 3, 4};
  A.emplace(5);
  A.emplace(6);
  B.emplace(5);
  B.emplace(6);
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ(i, *j);
    std::cout << i << std::endl;
    j++;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
