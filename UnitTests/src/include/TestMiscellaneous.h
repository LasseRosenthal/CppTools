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

TEST(Miscellaneous, BinaryRepresentationShort8BitIntZero)
{
  constexpr std::uint8_t val = 0;
  const std::string expected{"00000000"};
  std::stringstream sstr;
  cpptools::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Miscellaneous, BinaryRepresentationShort8BitIntOne)
{
  constexpr std::uint8_t val = 1;
  const std::string expected{"00000001"};
  std::stringstream sstr;
  cpptools::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Miscellaneous, BinaryRepresentation16BitInt)
{
  constexpr std::uint16_t val = 4971;
  const std::string expected{"00010011 01101011"};
  std::stringstream sstr;
  cpptools::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Miscellaneous, BinaryRepresentation64BitInt)
{
  constexpr std::uint64_t val = 5'239'785'412;
  const std::string expected{"00000000 00000000 00000000 00000001 00111000 01010000 11000111 11000100"};
  std::stringstream sstr;
  cpptools::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTMISCELLANEOUS_H_159342320523450356628748210722901825509637 
