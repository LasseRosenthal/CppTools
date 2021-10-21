/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitProxyTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    14.06.2021 
 */ 
 
#ifndef BITPROXYTEST_H_94091414124933114025798853917629538524858 
#define BITPROXYTEST_H_94091414124933114025798853917629538524858 
 
 
// includes
#include <Bitwise/BitProxy.h>

#include <sstream>

using namespace std::string_literals;
 

TEST(BitProxy, PunchMaskSizeOneNoOffset)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 1ULL;
  using myBitProxy = bws::BitProxy<type, true, size, startBit>;

  constexpr auto myMask = myBitProxy::punchMask;

  const auto expected = "00000001"s;
  std::stringstream sstr;
  bws::binaryRep(myMask, sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(BitProxy, testSingleBitConst)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 1ULL;
  using myBitProxy = bws::BitProxy<type, true, size, startBit>;

  const type data = 13; // 00001101

  constexpr myBitProxy proxy1(&data, 0ULL);
  constexpr myBitProxy proxy2(&data, 1ULL);
  constexpr myBitProxy proxy3(&data, 2ULL);
  constexpr myBitProxy proxy4(&data, 3ULL);
  constexpr myBitProxy proxy5(&data, 4ULL);

  constexpr bool isBitSet1 = proxy1.isSet();
  constexpr bool isBitSet2 = proxy2.isSet();
  constexpr bool isBitSet3 = proxy3.isSet();
  constexpr bool isBitSet4 = proxy4.isSet();
  constexpr bool isBitSet5 = proxy5.isSet();

  EXPECT_TRUE(isBitSet1);
  EXPECT_FALSE(isBitSet2);
  EXPECT_TRUE(isBitSet3);
  EXPECT_TRUE(isBitSet4);
  EXPECT_FALSE(isBitSet5);

}

TEST(BitProxy, testSingleBitNonConst)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 1ULL;
  using myBitProxy = bws::BitProxy<type, false, size, startBit>;

  type data = 13; // 00001101

  myBitProxy proxy(&data, 0ULL);

  bool isBitSet = proxy.isSet();
  EXPECT_TRUE(isBitSet);

  proxy = false;
  EXPECT_FALSE(proxy);
}

TEST(BitProxy, setBit)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 8ULL;
  using myBitProxy = bws::BitProxy<type, false, size, startBit>;

  type data = 13; // 00001101

  myBitProxy proxy(&data, 0ULL);

  EXPECT_TRUE(proxy.isSet());
  proxy.set(false);
  EXPECT_FALSE(proxy.isSet());

  const auto expected = "00001100"s;
  std::stringstream sstr;
  bws::binaryRep(proxy.value(), sstr);
  EXPECT_EQ(sstr.str(), expected);

  proxy.advance(7ULL);
  EXPECT_FALSE(proxy.isSet());
  proxy = true;
  EXPECT_TRUE(proxy.isSet());
  sstr.str("");
  bws::binaryRep(proxy.value(), sstr);
  EXPECT_EQ(sstr.str(), "10001100"s);
}

TEST(BitProxy, toggleBit)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 8ULL;
  using myBitProxy = bws::BitProxy<type, false, size, startBit>;

  type data = 13; // 00001101

  myBitProxy proxy1(&data, 0ULL);

  proxy1.flip();
  EXPECT_FALSE(static_cast<bool>(proxy1));

  const auto expected = "00001100"s;
  std::stringstream sstr;
  bws::binaryRep(proxy1.value(), sstr);
  EXPECT_EQ(sstr.str(), expected);
}

TEST(BitProxy, copyConstructor)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 8ULL;
  using myBitProxy = bws::BitProxy<type, false, size, startBit>;

  type data = 13; // 00001101

  myBitProxy proxy1(&data, 1ULL);

  proxy1.flip();
  EXPECT_TRUE(static_cast<bool>(proxy1));

  const auto expected = "00001111"s;
  std::stringstream sstr;
  bws::binaryRep(proxy1.value(), sstr);
  EXPECT_EQ(sstr.str(), expected);
  EXPECT_EQ(data, type(15));

  myBitProxy proxy2(proxy1);
  proxy2.flip();
  sstr.str("");
  bws::binaryRep(proxy2.value(), sstr);
  EXPECT_EQ(sstr.str(), "00001101"s);
  EXPECT_EQ(data, type(13));
}

TEST(BitProxy, assignment)
{
  using type = std::uint8_t;
  constexpr std::size_t startBit = 0ULL;
  constexpr std::size_t size = 8ULL;
  using myBitProxy = bws::BitProxy<type, false, size, startBit>;

  type data = 13; // 00001101

  myBitProxy proxy1(&data, 1ULL);

  proxy1.flip();
  EXPECT_TRUE(static_cast<bool>(proxy1));

  const auto expected = "00001111"s;
  std::stringstream sstr;
  bws::binaryRep(proxy1.value(), sstr);
  EXPECT_EQ(sstr.str(), expected);
  EXPECT_EQ(data, type(15));

  myBitProxy proxy2(&data, 0ULL);
  proxy2.flip();
  sstr.str("");
  bws::binaryRep(proxy2.value(), sstr);
  EXPECT_EQ(sstr.str(), "00001110"s);
  EXPECT_EQ(data, type(14));

  proxy2 = proxy1;
  proxy2.flip();
  sstr.str("");
  bws::binaryRep(proxy2.value(), sstr);
  EXPECT_EQ(sstr.str(), "00001100"s);
  EXPECT_EQ(data, type(12));
}
 

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITPROXYTEST_H_94091414124933114025798853917629538524858 
