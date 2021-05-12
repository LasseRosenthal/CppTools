/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    IntegerRangeTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    11.05.2021 
 */ 
 
#ifndef INTEGERRANGETEST_H_243723226115167192325406116124856149956 
#define INTEGERRANGETEST_H_243723226115167192325406116124856149956 
 
 
// includes
#include <Utils/IntegerRange.h>

#include <algorithm>
#include <ranges>
#include <vector>
 
using namespace cpptools; 

TEST(IntegerRange, emptyRangeNoFilter)
{
  std::vector<int> expected;
  
  std::vector<int> range;

  for (auto v : IntegerRange<int>(6, 6, 3))
  {
    range.push_back(v);
  }

  EXPECT_EQ(range, expected);
}

TEST(IntegerRange, noFilterStepOne)
{
  std::vector expected{1, 2, 3, 4, 5};
  
  std::vector<int> range;
  range.reserve(5);

  for (auto v : IntegerRange<int>(1, 6))
  {
    range.push_back(v);
  }

  EXPECT_EQ(range, expected);
}

TEST(IntegerRange, stepOneWithFilter)
{
  std::vector expected{2, 4};
  
  std::vector<int> range;
  range.reserve(2);

  for(auto v : IntegerRange<int>(1, 6) | [](int i) { return i % 2 == 0; })
  {
    range.push_back(v);
  }

  EXPECT_EQ(range, expected);
}

TEST(IntegerRange, emptyRangeStepOneWithFilter)
{
  std::vector<int> expected;
  std::vector<int> range;

  for(auto v : IntegerRange<int>(6, 6) | [](int i) { return i % 2 == 0; })
  {
    range.push_back(v);
  }

  EXPECT_EQ(range, expected);
}

TEST(IntegerRange, noFilterStepTwo)
{
  std::vector expected{1, 3, 5, 7, 9};
  
  std::vector<int> range;
  range.reserve(5);

  for (auto v : IntegerRange<int>(1, 11, 2))
  {
    range.push_back(v);
  }

  EXPECT_EQ(range, expected);
}



//TEST(IntegerRange, noFilterStepOneUseTransform)
//{
//  std::vector expected{1, 2, 3, 4, 5};
//  
//  std::vector<int> range;
//  range.reserve(5);
//
//  IntegerRange<int> myRange(1, 6);
//  std::vector       myRange2{1, 2, 3, 4, 5};
//  std::ranges::transform(myRange.begin(), myRange.end(), std::back_inserter(range),
//                 [](int v) { return v; });
//
//  EXPECT_EQ(range, expected);
//}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // INTEGERRANGETEST_H_243723226115167192325406116124856149956 
