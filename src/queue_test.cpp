#include "queue.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "queue"
// Copyright 2022 nolanhea

template <class T>
using queue = s21::queue<T>;

template <class T>
using original_queue = std::queue<T>;

using std::pair;

class S21QueueTest : public ::testing::Test {};

TEST(S21QueueTest, DefaultConstructor) {
  queue<int> A;
  const queue<int> AA;
  original_queue<int> B;
  const original_queue<int> BB;
}

TEST(S21QueueTest, Constructors) {
  queue<int> A;
  original_queue<int> B;
  for (int i = 0; i < 100; i++) {
    A.push(i);
    B.push(i);
  }
  queue<int> A_copy(A);
  queue<int> A_move(std::move(A));
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(A_copy.front(), A_move.front());
    A_copy.pop();
    A_move.pop();
  }
}

TEST(S21QueueTest, PushAndPop) {
  queue<int> A;
  original_queue<int> B;
  for (int i = 0; i < 100; i++) {
    A.push(i);
    B.push(i);
  }
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(A.front(), B.front());
    EXPECT_EQ(A.back(), B.back());
    A.pop();
    B.pop();
  }
}

TEST(S21QueueTest, Emplace) {
  queue<int> A;
  original_queue<int> B;
  for (int i = 0; i < 100; i++) {
    A.emplace_back(i);
    B.emplace(i);
  }
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(A.front(), B.front());
    EXPECT_EQ(A.back(), B.back());
    A.pop();
    B.pop();
  }
}
