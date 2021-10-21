/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitwiseTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.08.2021 
 */ 
 
#ifndef BITWISETEST_H_14946563455302696463502556210747172213383 
#define BITWISETEST_H_14946563455302696463502556210747172213383 
 
 
// includes
#include <Bitwise/Bitwise.h>

#include <string>
#include <sstream>

using namespace std::string_literals;
 
 
TEST(Bitwise, BinaryRepresentationShort8BitIntZero)
{
  constexpr std::uint8_t val = 0;
  const auto expected = "00000000"s;
  std::stringstream sstr;
  bws::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, BinaryRepresentationShort8BitIntOne)
{
  constexpr std::uint8_t val = 1;
  const auto expected = "00000001"s;
  std::stringstream sstr;
  bws::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, BinaryRepresentation16BitInt)
{
  constexpr std::uint16_t val = 4971;
  const auto expected = "00010011 01101011"s;
  std::stringstream sstr;
  bws::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, BinaryRepresentation64BitInt)
{
  constexpr std::uint64_t val = 5'239'785'412;
  const auto expected = "00000000 00000000 00000000 00000001 00111000 01010000 11000111 11000100"s;
  std::stringstream sstr;
  bws::binaryRep(val, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, punchMaskSizeOneNoOffset)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 1ULL;
  constexpr auto myMask = bws::punchMask<type>(size, startBit);

  const auto expected = "00000001"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, punchMaskSizeOneMSBSet)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 7ULL;
  constexpr std::size_t size = 1ULL;
  constexpr auto myMask = bws::punchMask<type>(size, startBit);

  const auto expected = "10000000"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, punchMaskSize6Offset1)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 1ULL;
  constexpr std::size_t size = 6ULL;
  constexpr auto myMask = bws::punchMask<type>(size, startBit);

  const auto expected = "01111110"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, punchMaskAllBitsSet)
{
  using type = std::uint16_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 16ULL;
  constexpr auto myMask = bws::punchMask<type>(size, startBit);

  const auto expected = "11111111 11111111"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, punchMaskNoBitsSet)
{
  using type = std::uint16_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 0ULL;
  constexpr auto myMask = bws::punchMask<type>(size, startBit);

  const auto expected = "00000000 00000000"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, punchMaskSizeExceedingTypeSize)
{
  using type = std::uint16_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 23ULL;
  constexpr auto myMask = bws::punchMask<type>(size, startBit);

  const auto expected = "11111111 11111111"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(Bitwise, countBitsUInt8ExpectZero)
{
  constexpr std::uint8_t v = 0b00000000;
  constexpr std::size_t expected = 0ULL;
  constexpr auto c = bws::countBits(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, countBitsUInt16ExpectOne)
{
  constexpr std::uint16_t v = 0b00000000'10000000;
  constexpr std::size_t expected = 1ULL;
  const auto c = bws::countBits(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, countBitsIntExpectTwo)
{
  constexpr std::uint32_t v = 0b00001010'00000000'00000000'00000000;
  constexpr std::size_t expected = 2ULL;
  const auto c = bws::countBits(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, countBitsIntExpectFive)
{
  constexpr std::uint32_t v = 0b00001010'00000000'00000000'00001101;
  constexpr std::size_t expected = 5ULL;
  const auto c = bws::countBits(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, countBitsSignedIntExpectFive)
{
  constexpr std::int32_t v = 0b00001010'00000000'00000000'00001101;
  constexpr std::size_t expected = 5ULL;
  const auto c = bws::countBits(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, countBitsUInt64ExpectAll)
{
  constexpr std::uint64_t v = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
  constexpr std::size_t expected = 64ULL;
  const auto c = bws::countBits(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, firstBitSetExpectZero)
{
  constexpr std::uint64_t v = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
  constexpr std::size_t expected = 0ULL;
  const auto c = bws::firstBitSet(v);
  EXPECT_EQ(c, expected);
}
#if 0

TEST(Bitwise, firstBitSetExpect63)
{
  constexpr std::uint64_t v = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
  constexpr std::size_t expected = 63ULL;
  const auto c = bws::firstBitSet(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, firstBitSet)
{
  constexpr std::int32_t v = 0b00001010'00000000'00000000'00001100;
  constexpr std::size_t expected = 2ULL;
  const auto c = bws::firstBitSet(v);
  EXPECT_EQ(c, expected);
}

TEST(Bitwise, firstBitSetNonOneIsSet)
{
  constexpr std::int32_t v = 0b00000000'00000000'00000000'00000000;
  constexpr std::size_t expected = 64ULL;
  const auto c = bws::firstBitSet(v);
  EXPECT_EQ(c, expected);
}
#endif
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITWISETEST_H_14946563455302696463502556210747172213383 
