/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TupleUtilsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    05.08.2021 
 */ 
 
#ifndef TUPLEUTILSTEST_H_73842371739741906624452196191811335420033 
#define TUPLEUTILSTEST_H_73842371739741906624452196191811335420033 
 
 
// includes
#include "DummyClass.h"

#include <Meta/TupleUtils.h>
#include <Utils/LambdaOverload.h>

#include <functional>
#include <type_traits>


TEST(TupleUtils, ForEachTuple)
{
  const auto myTup = std::make_tuple(1,2,3,4,5);

  int sum = 0;
  meta::forEach(myTup, [&sum](int v) { sum += v; });

  EXPECT_EQ(sum, 15);
}

struct Sum {
  int s = 0;
  void operator()(int i) noexcept
  {
    s += i;
  }
};

TEST(TupleUtils, ForEachTupleWithFunctor)
{
  const auto myTup = std::make_tuple(1,2,3,4,5);
  constexpr int bias = 14;
  Sum sum{ bias };
  meta::forEach(myTup, sum);
  EXPECT_EQ(sum.s, bias + 15);
}

TEST(TupleUtils, ForEachTupleWithLambdaOverload)
{
  const auto myTup = std::make_tuple(1, 2, Person(13, 12), 4.0, 5);

  double result;
  auto myLam = cpptools::LambdaOverload{
    [&result](int v) { result += v; },
    [&result](Person const& p) { result += p.getAge(); },
    [&result](double v) { result -= v; }
  };

  meta::forEach(myTup, myLam);
  EXPECT_EQ(result, 1 + 2 + 13 - 4.0 + 5);
}

TEST(TupleUtils, AccumulateTupleAddComplete)
{
  constexpr auto myTup = std::make_tuple(1,2,3,4,5);
  constexpr auto sum = meta::accumulate(myTup, std::plus<int>{});

  EXPECT_EQ(sum, 15);
}

TEST(TupleUtils, AccumulateTupleAddIncomplete)
{
  constexpr auto myTup = std::make_tuple(1,2,3,4,5);
  constexpr auto sum = meta::accumulate<1ULL, 3ULL>(myTup, std::plus<int>{});

  EXPECT_EQ(sum, 5);
}

TEST(TupleUtils, AccumulateTupleMultiplyComplete)
{
  constexpr auto myTup = std::make_tuple(1,2,3,4,5);
  constexpr auto sum = meta::accumulate(myTup, std::multiplies<int>{});

  EXPECT_EQ(sum, 120);
}

TEST(TupleUtils, AccumulateTupleMultiplyIncomplete)
{
  constexpr auto myTup = std::make_tuple(1,2,3,4,5);
  constexpr auto sum = meta::accumulate<4ULL, 5ULL>(myTup, std::multiplies<int>{});

  EXPECT_EQ(sum, 5);
}

TEST(TupleUtils, AccumulateTupleCompleteCheckType)
{
  constexpr auto myTup = std::make_tuple(1,2.3F,3ULL,4.2,5);
  auto sum = meta::accumulate(myTup, [](auto v1, auto v2) { return v1 + v2; });

  EXPECT_TRUE((std::is_same_v<decltype(sum), double>));
}

TEST(TupleUtils, AccumulateTupleIncompleteCheckType)
{
  constexpr auto myTup = std::make_tuple(1,2.3F,3ULL,4.2,5);
  auto sum = meta::accumulate<1ULL, 2ULL>(myTup, [](auto v1, auto v2) { return v1 + v2; });

  EXPECT_TRUE((std::is_same_v<decltype(sum), float>));
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TUPLEUTILSTEST_H_73842371739741906624452196191811335420033 
