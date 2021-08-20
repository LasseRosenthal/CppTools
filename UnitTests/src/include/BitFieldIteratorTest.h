/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitFieldIteratorTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.08.2021 
 */ 
 
#ifndef BITFIELDITERATORTEST_H_283239471735313606322223077919139429016049 
#define BITFIELDITERATORTEST_H_283239471735313606322223077919139429016049 
 
 
// includes
#include <Bitwise/BitFieldIterator.h>

#include <array>
#include <memory>
#include <sstream>
#include <string>

using namespace std::string_literals;
 

TEST(BitFieldIterator, dereferencing)
{
  constexpr std::array<std::uint8_t, 2ULL> data{0b11010101, 0b11000011};
  constexpr auto bitIterBegin = bws::cbegin(data);
  EXPECT_TRUE(static_cast<bool>(*bitIterBegin));
}

TEST(BitFieldIterator, comparison)
{
  constexpr std::array<std::uint8_t, 2ULL> data{0b11010101, 0b11000011};
  constexpr auto bitIterBegin = bws::cbegin(data);
  constexpr auto bitIterEnd = bws::cend(data);
  constexpr auto bitIterEndCopy = bws::cend(data);

  constexpr bool isSmaller = bitIterBegin < bitIterEnd;
  constexpr bool isGreater = bitIterEnd > bitIterBegin;
  constexpr bool isSelfEqual1 = bitIterBegin == bitIterBegin;
  constexpr bool isSelfEqual2 = bitIterEnd == bitIterEndCopy;
  constexpr bool isEqual = bitIterBegin == bitIterEnd;

  EXPECT_TRUE(isSmaller);
  EXPECT_TRUE(isGreater);
  EXPECT_TRUE(isSelfEqual1);
  EXPECT_TRUE(isSelfEqual2);
  EXPECT_FALSE(isEqual);
}

TEST(BitFieldIterator, randomAccess)
{
  constexpr std::array<std::uint8_t, 2ULL> data{0b11010101, 0b11000011};
  constexpr auto bitIterBegin = bws::cbegin(data);
  constexpr auto bitIterPlus2 = bitIterBegin + 2LL;
  constexpr auto bitIterEnd = bitIterBegin + 90LL;
  constexpr auto bitIterBegin2 = bitIterPlus2 + (-56LL);
  constexpr auto bitIterBegin3 = bitIterPlus2 + (-1LL);
  constexpr auto bitIterBegin4 = bitIterPlus2 - 1LL;

  EXPECT_TRUE(static_cast<bool>(*bitIterBegin));
  EXPECT_TRUE(static_cast<bool>(*bitIterPlus2));
  EXPECT_TRUE(bitIterEnd == bws::cend(data));
  EXPECT_TRUE(bitIterBegin2 == bitIterBegin);
  EXPECT_TRUE(bitIterBegin3 == bitIterBegin4);
  EXPECT_TRUE(bws::cend(data) - 16LL == bws::cbegin(data));

  EXPECT_TRUE(*(bws::cend(data) - 1LL));
  EXPECT_TRUE(*(bws::cend(data) - 2LL));
  EXPECT_FALSE(*(bws::cend(data) - 3LL));
  EXPECT_FALSE(*(bws::cend(data) - 4LL));
  EXPECT_FALSE(*(bws::cend(data) - 5LL));
  EXPECT_FALSE(*(bws::cend(data) - 6LL));
  EXPECT_TRUE(*(bws::cend(data) - 7LL));
  EXPECT_TRUE(*(bws::cend(data) - 8LL));
  EXPECT_TRUE(*(bws::cend(data) - 9LL));
  EXPECT_TRUE(*(bws::cend(data) - 10LL));
  EXPECT_FALSE(*(bws::cend(data) - 11LL));
  EXPECT_TRUE(*(bws::cend(data) - 12LL));
  EXPECT_FALSE(*(bws::cend(data) - 13LL));
  EXPECT_TRUE(*(bws::cend(data) - 14LL));
  EXPECT_FALSE(*(bws::cend(data) - 15LL));
  EXPECT_TRUE(*(bws::cend(data) - 16LL));
}

TEST(BitFieldIterator, iteratorDifference)
{
  constexpr std::array<std::uint8_t, 2ULL> data{ 0b11010101, 0b11000011 };
  constexpr auto begin = bws::cbegin(data);
  constexpr auto end = bws::cend(data);

  EXPECT_EQ(end - begin, 16LL);
  EXPECT_EQ(begin - end, -16LL);

  constexpr auto diff1 = (begin + 13LL) - (begin + 4LL);
  constexpr auto diff2 = (begin + 4LL) - (begin + 12LL);

  EXPECT_EQ(diff1, 9LL);
  EXPECT_EQ(diff2, -8LL);
}
 
TEST(BitFieldIterator, iterationOverArray)
{
  constexpr std::array<std::uint8_t, 2ULL> data{ 0b11010101, 0b11000011 };

  std::stringstream str;
  for(auto it = bws::cbegin(data); it != bws::cend(data); ++it)
  {
    str << *it;
  }

  const auto expected = "1010101111000011"s;
  EXPECT_EQ(str.str(), expected);
}

TEST(BitFieldIterator, reverseIterationOverArray)
{
  constexpr std::array<std::uint8_t, 2ULL> data{ 0b11010101, 0b11000011 };
  std::stringstream str;
  for(auto it = bws::crbegin(data); it != bws::crend(data); ++it)
  {
    str << *it;
  }

  const auto expected = "1100001111010101"s;
  EXPECT_EQ(str.str(), expected);
}

TEST(BitFieldIterator, iterationDynamicArray)
{
  const auto data = std::make_unique<std::uint16_t[]>(2);
  data[0] = 0b1001101010110101;
  data[1] = 0b0100111001010110;

  auto begin = bws::BitFieldIterator<std::uint16_t>{data.get(), 0ULL, 32ULL};
  auto end   = bws::BitFieldIterator<std::uint16_t>{data.get(), 32ULL, 32ULL};

  std::stringstream str;
  for(; begin != end; ++begin)
  {
    str << *begin;
  }

  EXPECT_EQ(str.str(), "10101101010110010110101001110010"s);
}


TEST(BitFieldIterator, indexedAccessStaticArray)
{
  constexpr std::array<std::uint8_t, 2ULL> data{ 0b11010101, 0b11000011 };
  constexpr auto begin = bws::cbegin(data);

  constexpr auto b1 = begin[1];
  constexpr auto b2 = begin[2];
  constexpr bool isB2True = b2;

  EXPECT_FALSE(b1);
  EXPECT_TRUE(isB2True);
}

TEST(BitFieldIterator, indexedAccessDynamicArray)
{
  auto data = std::make_unique<std::uint16_t[]>(2);
  data[0] = 0b1001101010110101;
  data[1] = 0b0100111001010110;

  auto begin = bws::BitFieldIterator<std::uint16_t>{data.get(), 0ULL, 32ULL};
  std::stringstream str;
  for(std::size_t i{}; i < 32ULL; ++i)
  {
    str << begin[i];
  }

  EXPECT_EQ(str.str(), "10101101010110010110101001110010"s);

  for(std::size_t i{}; i < 32ULL; ++i)
  {
    begin[i] = false;
  }

  str.str("");
  for(std::size_t i{}; i < 32ULL; ++i)
  {
    const auto b = begin[i];
    str << begin[i];
  }

  EXPECT_EQ(str.str(), "00000000000000000000000000000000"s);
  EXPECT_EQ(data[0], 0);
  EXPECT_EQ(data[1], 0);
}

TEST(BitFieldIterator, reverseIterationDynamicArray)
{
  const auto data = std::make_unique<std::uint16_t[]>(2);
  data[0] = 0b1001101010110101;
  data[1] = 0b0100111001010110;

  auto rbegin = std::make_reverse_iterator(bws::BitFieldIterator<std::uint16_t>{data.get(), 32ULL, 32ULL});
  auto rend   = std::make_reverse_iterator(bws::BitFieldIterator<std::uint16_t>{data.get(), 0ULL, 32ULL});

  std::stringstream str;
  for(; rbegin != rend; ++rbegin)
  {
    str << *rbegin;
  }

  EXPECT_EQ(str.str(), "01001110010101101001101010110101"s);
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELDITERATORTEST_H_283239471735313606322223077919139429016049 
