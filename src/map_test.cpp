#include "map.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
// Copyright 2022 nolanhea

template <class T, class K>
using map = s21::map<T, K>;

template <class T, class K>
using original_map = std::map<T, K>;

using std::pair;

class S21MapTest : public ::testing::Test {};

TEST(S21MapTest, DefaultConstructor) {
  map<int, char> A;
  map<int, int> B;
  const map<int, int> C;
  const map<char, int> D;
  EXPECT_EQ(A.size(), 0);
  EXPECT_EQ(B.size(), 0);
  EXPECT_EQ(C.size(), 0);
  EXPECT_EQ(D.size(), 0);
}
TEST(S21MapTest, InitializerConstructor) {
  map<int, int> A({pair(1, 10), pair(2, 20), pair(2, 30), pair(3, 30)});
  original_map<int, int> B(
      {pair(1, 10), pair(2, 20), pair(2, 30), pair(3, 30)});
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ(i.second, (*j).second);
    j++;
  }
  EXPECT_EQ(A.size(), B.size());
}

TEST(S21MapTest, At) {
  map<int, int> A({pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  original_map<int, int> B(
      {pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  EXPECT_EQ(A.at(1), B.at(1));
  EXPECT_EQ(A.at(2), B.at(2));
  EXPECT_EQ(A.at(3), B.at(3));
  A.at(3) = 200;
  B.at(3) = 200;
  EXPECT_EQ(A.at(3), B.at(3));

  const map<int, int> AA({pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  const original_map<int, int> BB(
      {pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  EXPECT_EQ(AA.at(1), BB.at(1));
  EXPECT_EQ(AA.at(2), BB.at(2));
  EXPECT_EQ(AA.at(3), BB.at(3));
}

TEST(S21MapTest, Brackets) {
  map<int, int> A({pair(1, 10)});
  original_map<int, int> B({pair(1, 10)});
  EXPECT_EQ(A[1], B[1]);
  A[2] = 20;
  B[2] = 20;
  B[4] = 999;
  A[4] = 999;
  EXPECT_EQ(A[4], B[4]);
  EXPECT_EQ(A[2], B[2]);
}

TEST(S21MapTest, Insert) {
  map<int, int> A({pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  original_map<int, int> B(
      {pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  auto ans = A.insert(std::pair(4, 40));
  auto original_ans = B.insert(std::pair(4, 40));
  EXPECT_EQ((*(ans.first)).first, (*(original_ans.first)).first);
  EXPECT_EQ((*(ans.first)).second, (*(original_ans.first)).second);
  EXPECT_EQ(ans.second, original_ans.second);
  ans = A.insert(std::pair(4, 40));
  original_ans = B.insert(std::pair(4, 40));
  EXPECT_EQ((*(ans.first)).first, (*(original_ans.first)).first);
  EXPECT_EQ((*(ans.first)).second, (*(original_ans.first)).second);
  EXPECT_EQ(ans.second, original_ans.second);
  ans = A.insert(std::pair(5, 50));
  original_ans = B.insert(std::pair(5, 50));
  EXPECT_EQ((*(ans.first)).first, (*(original_ans.first)).first);
  EXPECT_EQ((*(ans.first)).second, (*(original_ans.first)).second);
  EXPECT_EQ(ans.second, original_ans.second);
  ans = A.insert(std::pair(5, 50));
  original_ans = B.insert(std::pair(5, 50));
  EXPECT_EQ((*(ans.first)).first, (*(original_ans.first)).first);
  EXPECT_EQ((*(ans.first)).second, (*(original_ans.first)).second);
  EXPECT_EQ(ans.second, original_ans.second);
  ans = A.insert(std::pair(6, 60));
  original_ans = B.insert(std::pair(6, 60));
  EXPECT_EQ((*(ans.first)).first, (*(original_ans.first)).first);
  EXPECT_EQ((*(ans.first)).second, (*(original_ans.first)).second);
  EXPECT_EQ(ans.second, original_ans.second);
  ans = A.insert(std::pair(6, 60));
  original_ans = B.insert(std::pair(6, 60));
  EXPECT_EQ((*(ans.first)).first, (*(original_ans.first)).first);
  EXPECT_EQ((*(ans.first)).second, (*(original_ans.first)).second);
  EXPECT_EQ(ans.second, original_ans.second);
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ((*j).first, i.first);
    EXPECT_EQ((*j).second, i.second);
    j++;
  }
  EXPECT_EQ(A.size(), B.size());
}

TEST(S21MapTest, InsertSecond) {
  map<int, int> A({pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  original_map<int, int> B(
      {pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  A.insert(4, 40);
  B.insert(std::pair(4, 40));
  A.insert(4, 40);
  B.insert(std::pair(4, 40));
  A.insert(5, 50);
  B.insert(std::pair(5, 50));
  A.insert(5, 50);
  B.insert(std::pair(5, 50));

  A.insert(6, 60);
  B.insert(std::pair(6, 60));
  A.insert(6, 60);
  B.insert(std::pair(6, 60));
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ((*j).first, i.first);
    EXPECT_EQ((*j).second, i.second);
    j++;
  }
  EXPECT_EQ(A.size(), B.size());
}

TEST(S21MapTest, InsertOrAssign) {
  map<int, int> A({pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  original_map<int, int> B(
      {pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  A.insert_or_assign(3, 60);
  B.insert_or_assign(3, 60);
  A.insert_or_assign(4, 60);
  B.insert_or_assign(4, 60);
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ((*j).first, i.first);
    EXPECT_EQ((*j).second, i.second);
    j++;
  }
  EXPECT_EQ(A.size(), B.size());
}
TEST(S21MapTest, Emplace) {
  map<int, int> A({pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  original_map<int, int> B(
      {pair(1, 10), pair(2, 20), pair(2, 20), pair(3, 30)});
  A.emplace(4, 60);
  B.emplace(4, 60);
  auto j = B.begin();
  for (auto i : A) {
    EXPECT_EQ((*j).first, i.first);
    EXPECT_EQ((*j).second, i.second);
    j++;
  }
  EXPECT_EQ(A.size(), B.size());
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }
