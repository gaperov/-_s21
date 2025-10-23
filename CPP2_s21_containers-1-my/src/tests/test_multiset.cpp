#include "test.hpp"

TEST(multiset, Insert) {
  std::multiset<int> true_ms;
  s21::multiset<int> my_ms;

  for (int i = 0; i < 3; i++) {
    true_ms.insert(3);
    my_ms.insert(3);
  }
  EXPECT_EQ(true_ms.size(), my_ms.size());
}

TEST(multiset, equal_range) {
  std::multiset<int> true_ms = {1, 2, 3, 3, 3, 3, 4, 5};
  s21::multiset<int> my_ms = {1, 2, 3, 3, 3, 3, 4, 5};

  auto tp = true_ms.equal_range(3);
  auto tf = tp.first;
  auto ts = tp.second;

  int frange = 0;
  for (; tf != ts; tf++) {
    frange++;
  }

  auto mp = my_ms.equal_range(3);
  auto mf = mp.first;
  auto ms = mp.second;

  int mrange = 0;
  for (; mf != ms; mf++) {
    mrange++;
  }

  EXPECT_EQ(frange, mrange);
}