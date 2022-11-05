#include "stack.h"

#include "gtest/gtest.h"
#include "stack"
// Copyright 2022 nolanhea

template <class T>
using stack = s21::stack<T>;

template <class T>
using original_stack = std::stack<T>;

using std::pair;

class S21StackTest : public ::testing::Test {};

TEST(S21StackTest, DefaultConstructor) {
  stack<int> A;
  const stack<int> AA;
  original_stack<int> B;
  const original_stack<int> BB;
}

TEST(S21StackTest, Constructors) {
  stack<int> A;
  original_stack<int> B;
  for (int i = 0; i < 100; i++) {
    A.push(i);
    B.push(i);
  }
  stack<int> A_copy(A);
  stack<int> A_move(std::move(A));
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(A_copy.top(), A_move.top());
    A_copy.pop();
    A_move.pop();
  }
}

TEST(S21StackTest, InitializerConstructor) {
  stack<int> A({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  original_stack<int> B({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  while (B.size() != 0) {
    EXPECT_EQ(A.top(), B.top());
    A.pop();
    B.pop();
  }
}

TEST(S21StackTest, AssignmentOperators) {
  stack<int> A({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  stack<int> AA({0, 0, 0});
  AA = A;
  original_stack<int> B({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  original_stack<int> BB({0, 0, 0});
  BB = B;
  while (BB.size() != 0) {
    EXPECT_EQ(AA.top(), BB.top());
    AA.pop();
    BB.pop();
  }
}

TEST(S21StackTest, PushAndPop) {
  stack<int> A;
  original_stack<int> B;
  for (int i = 0; i < 100; i++) {
    A.push(i);
    B.push(i);
  }
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(A.top(), B.top());
    A.pop();
    B.pop();
  }
}
TEST(S21StackTest, PushPop2) {
  stack<int> A;
  original_stack<int> B;
  for (int i = 0; i < 100; i++) {
    A.push(i);
    B.push(i);
  }
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(A.top(), B.top());
    A.pop();
    B.pop();
  }
}

TEST(S21StackTest, CapacityTest) {
  stack<int> A;
  original_stack<int> B;
  EXPECT_EQ(A.size(), B.size());
  EXPECT_EQ(A.empty(), B.empty());
  A.push(1);
  A.push(1);
  A.push(1);
  B.push(1);
  B.push(1);
  B.push(1);
  EXPECT_EQ(A.size(), B.size());
  EXPECT_EQ(A.empty(), B.empty());
  A.pop();
  B.pop();
  EXPECT_EQ(A.size(), B.size());
  EXPECT_EQ(A.empty(), B.empty());
}

TEST(S21StackTest, Swap) {
  stack<int> A({1, 2, 3});
  stack<int> B({4, 5, 6, 7});
  A.swap(B);
  EXPECT_EQ(A.top(), 7);
  EXPECT_EQ(B.top(), 3);
}

TEST(S21StackTest, Emplace) {
  stack<int> A({1, 2, 3});
  A.emplace_front(4);
  A.emplace_front(5);
  A.emplace_front(6);

  original_stack<int> B({1, 2, 3});
  B.push(4);
  B.push(5);
  B.push(6);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(A.top(), B.top());
    A.pop();
    B.pop();
  }
}
