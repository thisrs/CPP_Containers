#include "array.h"

#include <array>
#include <ctime>
#include <iostream>

#include "gtest/gtest.h"
#include "list"

// Copyright 2022 nolanhea

template <class T, int N = 0>
using array = s21::array<T, N>;

template <class T, int N>
using original_array = std::array<T, N>;

using std::cout;
using std::endl;
using std::list;
using std::pair;

class S21ArrayTest : public ::testing::Test {};

TEST(S21ArrayTest, DefaultConstructorEfficiency) {
  int val1 = 0;
  int val2 = 0;
  clock_t original_time = clock();
  for (int i = 0; i < 100000; i++) {
    original_array<int, 10> A;
    val1 += A[0];
  }
  original_time = clock() - original_time;

  clock_t my_time = clock();
  for (int i = 0; i < 100000; i++) {
    array<int, 10> A;
    val2 += A[0];
  }
  my_time = clock() - my_time;
  EXPECT_TRUE(my_time < original_time * 5)
      << "my time:" << my_time << endl
      << "original time:" << original_time << endl;
}

TEST(S21ArrayTest, InitializerListConstructor) {
  array<int, 10> A({1, 2, 3, 4, 5});
  original_array<int, 10> B({1, 2, 3, 4, 5});
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(A[i], B[i]);
  }
  const array<int, 10> AA({1, 2, 3, 4, 5});
  const original_array<int, 10> BB({1, 2, 3, 4, 5});
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(AA[i], BB[i]);
  }
}

TEST(S21ArrayTest, InitListOther) {
  list<int> a = {1, 2, 3};
  list<int> b = {11, 12, 13};
  array<list<int>, 2> A({a, b});
  array<list<int>, 2> B({a, b});
  EXPECT_EQ(*A[0].begin(), *B[0].begin());
  *(A[0].begin()) = 100;
  EXPECT_NE(*A[0].begin(), *B[0].begin());
}

TEST(S21ArrayTest, InitializerListConstructorEfficiency) {
  int val1 = 0;
  int val2 = 0;
  clock_t original_time = clock();
  for (int i = 0; i < 100000; i++) {
    const original_array<int, 10> BB({1, 2, 3, 4, 5, 6});
    val1 += BB[0];
    val1 += BB[1];
    val1 += BB[2];
  }
  original_time = clock() - original_time;

  clock_t my_time = clock();
  for (int i = 0; i < 100000; i++) {
    const array<int, 10> AA({1, 2, 3, 4, 5, 6});
    val2 += AA[0];
    val2 += AA[1];
    val2 += AA[2];
  }
  my_time = clock() - my_time;
  // cout << "my time:" << my_time << endl;
  // cout << "original time:" << original_time << endl;
  EXPECT_TRUE(my_time < original_time * 5)
      << "my time:" << my_time << endl
      << "original time:" << original_time << endl;
}

TEST(S21ArrayTest, CopyConstructor) {
  array<int, 10> A({1, 2, 3, 4, 5});
  const array<int, 10> B(A);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(A[i], B[i]);
  }
}

TEST(S21ArrayTest, MoveConstructor) {
  array<int, 10> A({1, 2, 3, 4, 5});
  const array<int, 10> B(std::move(A));
  auto j = B.begin();
  for (int i = 1; i <= 5; i++) {
    EXPECT_EQ(i, *j);
    j++;
  }
}
TEST(S21ArrayTest, CopyAssignment) {
  array<int, 10> A({1, 2, 3, 4, 5});
  array<int, 10> B = A;
  auto j = B.begin();
  for (int i = 1; i <= 5; i++) {
    EXPECT_EQ(i, *j);
    j++;
  }
}
TEST(S21ArrayTest, MoveAssignment) {
  array<int, 10> A({1, 2, 3, 4, 5});
  array<int, 10> B = std::move(A);
  auto j = B.begin();
  for (int i = 1; i <= 5; i++) {
    EXPECT_EQ(i, *j);
    j++;
  }
}

TEST(S21ArrayTest, SquareBrackets) {
  array<int, 1000> A({1, 2, 3, 4, 5});
  original_array<int, 1000> B({1, 2, 3, 4, 5});
  for (int i = 0; i < 1000; i++) {
    A[i] = i + 2;
    B[i] = i + 2;
    EXPECT_EQ(A[i], B[i]);
  }
  const array<int, 10> AA({1, 2, 3, 4, 5});
  const original_array<int, 10> BB({1, 2, 3, 4, 5});
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(A[i], B[i]);
  }
}

TEST(S21ArrayTest, SquareBracketsEfficiency) {
  int val1 = 0;
  int val2 = 0;
  clock_t original_time = clock();
  for (int i = 0; i < 100000; i++) {
    const original_array<int, 10> BB({1, 2, 3, 4, 5, 6});
    val1 += BB[0];
    val1 += BB[1];
    val1 += BB[2];
    val1 += BB[3];
    val1 += BB[4];
    val1 += BB[5];
    val1 += BB[6];
  }
  original_time = clock() - original_time;

  clock_t my_time = clock();
  for (int i = 0; i < 100000; i++) {
    const array<int, 10> AA({1, 2, 3, 4, 5, 6});
    val2 += AA[0];
    val2 += AA[1];
    val2 += AA[2];
    val2 += AA[3];
    val2 += AA[4];
    val2 += AA[5];
    val2 += AA[6];
  }
  my_time = clock() - my_time;

  EXPECT_TRUE(my_time < original_time * 5)
      << "my time:" << my_time << endl
      << "original time:" << original_time << endl;
}

TEST(S21ArrayTest, FrontBack) {
  array<int, 5> A({1, 2, 3, 4, 5});
  const array<int, 5> B({1, 2, 3, 4, 5});
  EXPECT_EQ(A.front(), 1);
  EXPECT_EQ(B.front(), 1);
  EXPECT_EQ(A.back(), 5);
  EXPECT_EQ(B.back(), 5);
}

TEST(S21ArrayTest, At) {
  array<int, 1000> A;
  original_array<int, 1000> B;
  for (int i = 0; i < 1000; i++) {
    A.at(i) = i + 2;
    B.at(i) = i + 2;
    EXPECT_EQ(A.at(i), B.at(i));
  }
  const array<int, 5> AA({1, 2, 3, 4, 5});
  const original_array<int, 5> BB({1, 2, 3, 4, 5});
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(AA.at(i), BB.at(i));
  }
}
TEST(S21ArrayTest, Data) {
  array<int, 1000> A;
  original_array<int, 1000> B;
  for (int i = 0; i < 1000; i++) {
    A.at(i) = i;
    B.at(i) = i;
    EXPECT_EQ(A.data()[i], B.data()[i]);
  }
  const array<int, 5> AA({1, 2, 3, 4, 5});
  const original_array<int, 5> BB({1, 2, 3, 4, 5});
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(AA.data()[i], BB.data()[i]);
  }
}

TEST(S21ArrayTest, Iterators) {
  array<int, 1000> A;
  original_array<int, 1000> B;
  for (int i = 0; i < 1000; i++) {
    A.at(i) = i;
    B.at(i) = i;
  }
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ(i, *j);
    j++;
  }
  j = B.end();
  auto i = A.end();
  EXPECT_EQ(*(--i), *(--j));
  const array<int, 5> AA({1, 2, 3, 4, 5});
  const original_array<int, 5> BB({1, 2, 3, 4, 5});
  auto ii = AA.begin();
  auto jj = BB.begin();
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(*ii, *jj);
    ii++;
    jj++;
  }
}

TEST(S21ArrayTest, Empty) {
  array<int, 10> A;
  EXPECT_EQ(A.empty(), false);
}

TEST(S21ArrayTest, Size) {
  const array<int, 10> A({1, 2, 3});
  const original_array<int, 10> B({1, 2, 3});
  EXPECT_EQ(B.max_size(), A.max_size());
  EXPECT_EQ(B.size(), 10);
  EXPECT_EQ(A.size(), B.size());
}

TEST(S21ArrayTest, Swap) {
  array<int, 5> A({1, 4, 5, 6, 7});
  array<int, 5> B({10, 14, 15, 16, 17});
  A.swap(B);
  EXPECT_EQ(A[1], 14);
  EXPECT_EQ(B[1], 4);
}

TEST(S21ArrayTest, Swap2) {
  array<list<int>, 3> A;
  array<list<int>, 3> B;
  list<int> a = {1, 2, 3, 4, 5};
  list<int> b = {11, 12, 13, 14, 15};
  A[0] = a;
  A[1] = a;
  A[2] = a;
  B[0] = b;
  B[1] = b;
  B[2] = b;
  A.swap(B);
  EXPECT_EQ(*A[0].begin(), 11);
  EXPECT_EQ(*++A[0].begin(), 12);
}

TEST(S21ArrayTest, SwapEfficiency) {
  clock_t original_time = clock();
  for (int i = 0; i < 10000; i++) {
    original_array<int, 10> AA({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    original_array<int, 10> BB({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
    AA.swap(BB);
  }
  original_time = clock() - original_time;

  clock_t my_time = clock();
  for (int i = 0; i < 10000; i++) {
    array<int, 10> A({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    array<int, 10> B({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
    A.swap(B);
  }
  my_time = clock() - my_time;
  EXPECT_TRUE(my_time < original_time * 5)
      << "my time:" << my_time << endl
      << "original time:" << original_time << endl;
}

TEST(S21ArrayTest, Fill) {
  list<int> a = {1, 2, 3};
  array<list<int>, 2> A;
  A.fill(a);
  array<list<int>, 2> B;
  B.fill(a);
  EXPECT_EQ(*A[0].begin(), *B[0].begin());
  *(A[0].begin()) = 100;
  EXPECT_NE(*A[0].begin(), *B[0].begin());

  EXPECT_EQ(*A[1].begin(), *B[1].begin());
  *(A[1].begin()) = 100;
  EXPECT_NE(*A[1].begin(), *B[1].begin());
}

TEST(S21ArrayTest, FillEfficiency) {
  clock_t original_time = clock();
  for (int i = 0; i < 30; i++) {
    list<int> a = {1, 2, 3};
    original_array<list<int>, 30> A;
    A.fill(a);
  }
  original_time = clock() - original_time;

  clock_t my_time = clock();
  for (int i = 0; i < 30; i++) {
    list<int> a = {1, 2, 3};
    array<list<int>, 30> A;
    A.fill(a);
  }
  my_time = clock() - my_time;
  EXPECT_TRUE(my_time < original_time * 1.5)
      << "my time:" << my_time << endl
      << "original time:" << original_time << endl;
}
