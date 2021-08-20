/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestMiscellaneous.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    29.09.2020 
 */ 
 
#ifndef TESTMISCELLANEOUS_H_159342320523450356628748210722901825509637 
#define TESTMISCELLANEOUS_H_159342320523450356628748210722901825509637 
 
 
// includes
#include <Utils/miscellaneous.h>

#include <map>
#include <sstream>
#include <vector>
 


TEST(Miscellaneous, findLastIfExpectEnd)
{
  const std::vector<int> values{1, 2, 3, 4, 5, 6, 7};
  const auto e = cpptools::findLastIf(values, [](int v) { return v < 1; });
  EXPECT_EQ(e, values.cend());
}

TEST(Miscellaneous, findLastIfExpectValidValue)
{
  std::vector<int> values{1, 2, 3, 4, 5, 6, 7};
  const auto e = cpptools::findLastIf(values, [](int v) { return v < 5; });
  EXPECT_NE(e, values.end());
  EXPECT_EQ(*e, 4);
}

TEST(Miscellaneous, arraySize)
{
  constexpr std::size_t size = 56ULL;
  double                arr[size];
  constexpr auto        arrSize = cpptools::arraySize(arr);
  EXPECT_EQ(arrSize, size);
}

TEST(Miscellaneous, minCheckValue)
{
  constexpr short s = 153;
  constexpr int   i = 12;
  constexpr auto m = cpptools::min(s, 1.24F, i);
  EXPECT_EQ(m, 1.24F);
}

TEST(Miscellaneous, minCheckType)
{
  constexpr short s = 153;
  constexpr int   i = 12;
  constexpr auto m = cpptools::min(s, 7LL, i);
  EXPECT_TRUE((std::is_same_v<std::remove_const_t<decltype(m)>, long long>));
}
 
TEST(Miscellaneous, maxCheckValue)
{
  constexpr short s = 153;
  constexpr int   i = 12;
  constexpr auto m = cpptools::max(s, 1.24F, i);
  EXPECT_EQ(m, 153);
}

TEST(Miscellaneous, maxCheckType)
{
  constexpr short s = 153;
  constexpr int   i = 12;
  constexpr auto m = cpptools::max(s, 7LL, i);
  EXPECT_TRUE((std::is_same_v<std::remove_const_t<decltype(m)>, long long>));
}

TEST(Miscellaneous, AlignUp)
{
  constexpr std::size_t alignment = 7ULL;

  EXPECT_EQ(cpptools::alignUp(0ULL, alignment), 0ULL);

  for(std::size_t n = 0ULL; n < 1000; ++n)
  {
    const auto lowerBound = n * alignment + 1ULL;
    const auto expectedAlignment = (n + 1ULL) * alignment;
    for(std::size_t i = lowerBound; i < lowerBound + alignment; ++i)
    {
      EXPECT_EQ(cpptools::alignUp(i, alignment), expectedAlignment);
    }
  }
}

TEST(Miscellaneous, AlignUpTemplatedVersion)
{
  constexpr std::size_t alignment = 7ULL;

  EXPECT_EQ(cpptools::alignUp<alignment>(0ULL), 0ULL);

  for(std::size_t n = 0ULL; n < 1000ULL; ++n)
  {
    const auto lowerBound = n * alignment + 1ULL;
    const auto expectedAlignment = (n + 1ULL) * alignment;
    for(std::size_t i = lowerBound; i < lowerBound + alignment; ++i)
    {
      EXPECT_EQ(cpptools::alignUp<alignment>(i), expectedAlignment);
    }
  }
}

TEST(Miscellaneous, AlignUpTemplatedVersionAlignmentPowerOfTwo)
{
  constexpr std::size_t alignment = 16ULL;

  EXPECT_EQ(cpptools::alignUp<alignment>(0ULL), 0ULL);

  for(std::size_t n = 0ULL; n < 1000ULL; ++n)
  {
    const auto lowerBound = n * alignment + 1ULL;
    const auto expectedAlignment = (n + 1ULL) * alignment;
    for(std::size_t i = lowerBound; i < lowerBound + alignment; ++i)
    {
      EXPECT_EQ(cpptools::alignUp<alignment>(i), expectedAlignment);
    }
  }
}

TEST(Miscellaneous, getMapKeys)
{
  const int mapSize = 31;
  std::map<int, int> m;
  std::vector<int> expected;
  expected.reserve(mapSize);
  const int first = 152;
  for(int i = first; i < first + mapSize; ++i)
  {
    m[i] = 156;
    expected.push_back(i);
  }

  const auto keys = cpptools::getMapKeys(m);
  EXPECT_EQ(keys, expected);
}

TEST(Miscellaneous, numberOfDecimalPlacesDouble)
{
  const double value = 31.146;
  constexpr std::size_t expected = 3ULL;
  EXPECT_EQ(cpptools::numberOfDecimalPlaces(value), expected);
}

TEST(Miscellaneous, numberOfDecimalPlacesFloat)
{
  const float value = 1042.2365F;
  constexpr std::size_t expected = 4ULL;
  EXPECT_EQ(cpptools::numberOfDecimalPlaces(value), expected);
}

TEST(Miscellaneous, numberOfDecimalPlacesInt)
{
  const int value = 31146;
  constexpr std::size_t expected = 0ULL;
  EXPECT_EQ(cpptools::numberOfDecimalPlaces(value), expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTMISCELLANEOUS_H_159342320523450356628748210722901825509637 
