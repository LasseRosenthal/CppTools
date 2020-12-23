/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitFieldTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    06.10.2020 
 */ 
 
#ifndef BITFIELDTEST_H_10697265841863131580121343099140651261122718 
#define BITFIELDTEST_H_10697265841863131580121343099140651261122718 
 
 
// includes
#include <Utils/BitField.h>

#include <algorithm>
#include <string>
#include <sstream>
 
using namespace std::string_literals;
using namespace cpptools;
 
TEST(BitField, types)
{
  using bitField = BitField<9ULL>;
  EXPECT_TRUE((std::is_same_v<bitField::value_type, std::uint16_t>));
}

TEST(BitField, conversionOperator)
{
  using bitField = BitField<3ULL>;
  constexpr bitField b{6};
  constexpr int v = b;
  EXPECT_EQ(v, 6);
}

TEST(BitField, accessOperator)
{
  using bitField = BitField<3ULL, 2ULL>;
  constexpr bitField b{0b0000'0010};
  
  EXPECT_FALSE(b[0]);
  EXPECT_TRUE(b[1]);
  EXPECT_FALSE(b[2]);
}

TEST(BitField, comparisonOperator)
{
  using bitField = BitField<4ULL>;
  constexpr bitField b1{0b0110'0101};
  constexpr bitField b2{0b1101'0101};
  EXPECT_TRUE(b1 == b2);
}

TEST(BitField, BitWiseNot)
{
  using bitField = BitField<4ULL, 3ULL>;
  constexpr bitField b1{0b0110'0101};
  constexpr auto     b2 = ~b1;
  constexpr bitField expected{0b0110'1010};

  EXPECT_TRUE(b2 == expected);
}

TEST(BitField, AndOperator)
{
  using bitField = BitField<4ULL, 3ULL>;
  constexpr bitField b1{0b0110'0101};
  constexpr bitField b2{0b1101'1010};
  constexpr auto     b3 = b1 & b2;
  constexpr bitField expected{0b0110'0000};

  EXPECT_TRUE(b3 == expected);
}

TEST(BitField, AndOperatorModifyObject)
{
  using bitField = BitField<4ULL, 3ULL>;
  bitField b1{0b0110'0101};
  constexpr bitField b2{0b1101'1010};
  b1 &= b2;
  constexpr bitField expected{0b0110'0000};

  EXPECT_TRUE(b1 == expected);
}

TEST(BitField, OrOperator)
{
  using bitField = BitField<4ULL>;
  constexpr bitField b1{0b0110'0101};
  constexpr bitField b2{0b1101'0011};
  constexpr auto     b3 = b1 | b2;
  constexpr bitField expected{0b0110'0111};

  EXPECT_TRUE(b3 == expected);
}

TEST(BitField, OrOperatorModifyObject)
{
  using bitField = BitField<4ULL>;
  bitField b1{0b0110'0101};
  constexpr bitField b2{0b1101'0011};
  b1 |= b2;
  constexpr bitField expected{0b0110'0111};

  EXPECT_TRUE(b1 == expected);
}

TEST(BitField, ExOrOperator)
{
  using bitField = BitField<4ULL>;
  constexpr bitField b1{0b0110'0001};
  constexpr bitField b2{0b1101'1011};
  constexpr auto     b3 = b1 ^ b2;
  constexpr bitField expected{0b0110'1010};

  EXPECT_TRUE(b3 == expected);
}

TEST(BitField, ExOrOperatorModifyObject)
{
  using bitField = BitField<4ULL>;
  bitField b1{0b0110'0001};
  constexpr bitField b2{0b1101'1011};
  b1 ^= b2;
  constexpr bitField expected{0b0110'1010};

  EXPECT_TRUE(b1 == expected);
}

TEST(BitField, streamIntoOstream)
{
  using bitField = BitField<3ULL, 2ULL>;
  constexpr bitField b{0b0000'0110};

  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "110"s);
}

TEST(BitField, BitProxySetBitToTrue)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0011};
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "011"s);

  auto bitProxy = b.getBitProxy(2ULL);
  bitProxy      = true;
  sstr.str("");
  sstr << b;
  EXPECT_EQ(sstr.str(), "111"s);
}

TEST(BitField, BitProxySetBitToFalse)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0111};
  auto bitProxy = b.getBitProxy(0ULL);
  bitProxy      = false;
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "110"s);
}

TEST(BitField, BitProxyFlipOneBit)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0111};
  auto bitProxy = b.getBitProxy(1ULL);
  bitProxy.flip();
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "101"s);
}

TEST(BitField, BitProxyFlipZeroBit)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0101};
  auto bitProxy = b.getBitProxy(1ULL);
  bitProxy.flip();
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "111"s);
}

TEST(BitField, BitProxyIsSet)
{
  using bitField = BitField<3ULL, 2ULL>;
  constexpr bitField b{0b0000'0101};

  constexpr auto bitProxy0 = b.getBitProxy(0ULL);

  constexpr bool isZeroBitSet = bitProxy0;
  EXPECT_TRUE(isZeroBitSet);
  constexpr bool bitProxy1 = b[1ULL];
  EXPECT_FALSE(bitProxy1);
  constexpr auto bitProxy2 = b.getBitProxy(2ULL);
  EXPECT_TRUE(bitProxy2.isSet());
}

TEST(BitField, accessViaBeginIterator)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0010};
  
  EXPECT_FALSE(b[0]);
  EXPECT_TRUE(b[1]);
  EXPECT_FALSE(b[2]);

  auto beg = b.begin();
  *beg     = true;
  EXPECT_TRUE(b[0]);
}

TEST(BitField, accessViaBeginIteratorAfterIncrement)
{
  using bitField = BitField<3ULL, 0ULL>;
  bitField b{0b0000'0111};
  
  EXPECT_TRUE(b[0]);
  EXPECT_TRUE(b[1]);
  EXPECT_TRUE(b[2]);

  auto beg = b.begin();
  ++beg;
  *beg     = false;
  EXPECT_FALSE(b[1]);
  ++beg;
  *beg     = false;
  EXPECT_FALSE(b[2]);
}

TEST(BitField, accessViaBeginIndexOperator)
{
  using bitField = BitField<3ULL, 0ULL>;
  bitField b{0b0000'0111};
  
  EXPECT_TRUE(b[0]);
  EXPECT_TRUE(b[1]);
  EXPECT_TRUE(b[2]);

  for(std::size_t i{}; i < b.size(); ++i)
  {
    b[i] = false;
  }

  for(std::size_t i{}; i < b.size(); ++i)
  {
    EXPECT_FALSE(b[i]);
  }
}

TEST(BitField, accessViaBeginIndexOperatorMaxSize)
{
  using bitField = BitField<8ULL>;
  bitField b{0b0101'1010};

  for(std::size_t i{}; i < b.size(); ++i)
  {
    b[i] = true;
  }

  for(std::size_t i{}; i < b.size(); ++i)
  {
    EXPECT_TRUE(b[i]);
  }
}

TEST(BitField, accessViaIteratorArrowOperator)
{
  using bitField = BitField<3ULL, 0ULL>;
  constexpr bitField b{0b0000'0011};
  
  constexpr auto it = b.begin();

  EXPECT_TRUE(it->isSet());
  auto it2 = it + 1;
  EXPECT_TRUE(it2->isSet());
  auto it3 = it + 2;
  EXPECT_FALSE(it3->isSet());
}

TEST(BitField, rangeBasedForLoop)
{
  using bitField = BitField<3ULL, 0ULL>;
  bitField b{0b0000'0111};
  
  EXPECT_TRUE(b[0]);
  EXPECT_TRUE(b[1]);
  EXPECT_TRUE(b[2]);

  for(const auto bv : b)
  {
    EXPECT_TRUE(bv);
  }
}

TEST(BitField, additionOperator)
{
  using bitField = BitField<7ULL, 0ULL>;
  bitField b{0b0101'0011};
  
  EXPECT_TRUE(b[0]);
  EXPECT_TRUE(b[1]);
  EXPECT_FALSE(b[2]);
  EXPECT_FALSE(b[3]);
  EXPECT_TRUE(b[4]);
  EXPECT_FALSE(b[5]);
  EXPECT_TRUE(b[6]);

  auto it1 = b.begin();

  auto it2 = it1 + 1LL;
  EXPECT_TRUE(*it2);
  it2 = it1 + 2LL;
  EXPECT_FALSE(*it2);
  it2 = 3 + it1;
  EXPECT_FALSE(*it2);
}

TEST(BitField, indexAccessIterator)
{
  using bitField = BitField<7ULL, 0ULL>;
  bitField b{0b0101'0011};
  
  auto it = b.begin();

  EXPECT_TRUE(it[0]);
  EXPECT_TRUE(it[1]);
  EXPECT_FALSE(it[2]);
  EXPECT_FALSE(it[3]);
  EXPECT_TRUE(it[4]);
  EXPECT_FALSE(it[5]);
  EXPECT_TRUE(it[6]);
}

TEST(BitField, distanceBetweenIterators)
{
  using bitField = BitField<7ULL, 0ULL>;
  bitField b{0b0101'0011};

  auto it = b.begin();
  EXPECT_EQ(it - it, 0LL);

  constexpr long long n = 5LL;
  auto it1 = it + n;
  EXPECT_EQ(it1 - it, n);
  EXPECT_EQ(it - it1, -n);

  auto it2 = it + (it1 - it);
  EXPECT_EQ(it2, it1);
}

TEST(BitField, findIf)
{
  using bitField = BitField<7ULL, 0ULL>;
  bitField b{0b0101'0011};
  auto firstFalse = std::find_if(b.begin(), b.end(), [](const auto& val) { return val == false; });
  EXPECT_EQ(std::distance(b.begin(), firstFalse), 2LL);
}

union Flags {
 
  std::uint16_t raw;
  BitField<6ULL, 0ULL> f0;
  BitField<6ULL, 6ULL> f1;
  BitField<4ULL, 12ULL> f2;

  Flags(std::uint16_t v3)
    : f2{v3}
  {}
};

TEST(BitField, BitfieldsInUnion)
{
  Flags flags(0b0000'0000'1100'0100);

  flags.f0 = 0b0000'0111;
  flags.f1 = 63;
  //0b1111'0000'1111'1111;
  flags.f2 = 0b0000'0000'0000'0000;

  EXPECT_EQ(static_cast<std::uint8_t>(flags.f0), 7);
  EXPECT_EQ(static_cast<std::uint16_t>(flags.f1), 63);
  EXPECT_EQ(static_cast<std::uint16_t>(flags.f2), 0);

}
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELDTEST_H_10697265841863131580121343099140651261122718 
