/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    AlgorithmsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    22.11.2020 
 */ 
 
#ifndef ALGORITHMSTEST_H_660819788744120126495741087761482729187 
#define ALGORITHMSTEST_H_660819788744120126495741087761482729187 
 
 
// includes
#include <Meta/Algorithms.h> 
 
using namespace meta;
 
 
TEST(Algorithms, accumulatePlus)
{
  constexpr std::array<int, 5> arr{1, 3, 5, 7, 9};
  constexpr int init = 145;
  constexpr auto sum = meta::accumulate(arr, [](auto& a, auto b){ a += b;}, init);
  constexpr int expected = 25 + init;

  EXPECT_EQ(sum, expected);
}

TEST(Algorithms, accumulateMultiply)
{
  constexpr std::array<int, 5> arr{1, 3, 5, 7, 9};
  constexpr int init = 145;
  constexpr auto sum = meta::accumulate(arr, [](auto& a, auto b){ a *= 3*b;}, init);
  constexpr int expected = init * 3 * 9 * 15 * 21 * 27;

  EXPECT_EQ(sum, expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ALGORITHMSTEST_H_660819788744120126495741087761482729187 
