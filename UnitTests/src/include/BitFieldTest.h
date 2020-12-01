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

TEST(BitField, streamIntoOstream)
{
  using bitField = BitField<3ULL, 2ULL>;
  constexpr bitField b{0b0000'0110};

  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "00011000"s);
}

TEST(BitField, BitProxySetBitToTrue)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0011};
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "00001100"s);

  auto bitProxy = b.getBitProxy(2ULL);
  bitProxy      = true;
  sstr.str("");
  sstr << b;
  EXPECT_EQ(sstr.str(), "00011100"s);
}

TEST(BitField, BitProxySetBitToFalse)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0111};
  auto bitProxy = b.getBitProxy(0ULL);
  bitProxy      = false;
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "00011000"s);
}

TEST(BitField, BitProxyFlipOneBit)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0111};
  auto bitProxy = b.getBitProxy(0ULL);
  bitProxy.flip();
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "00011000"s);
}

TEST(BitField, BitProxyFlipZeroBit)
{
  using bitField = BitField<3ULL, 2ULL>;
  bitField b{0b0000'0101};
  auto bitProxy = b.getBitProxy(1ULL);
  bitProxy.flip();
  std::stringstream sstr;
  sstr << b;
  EXPECT_EQ(sstr.str(), "00011100"s);
}

TEST(BitField, BitProxyIsSet)
{
  using bitField = BitField<3ULL, 2ULL>;
  constexpr bitField b{0b0000'0101};

  constexpr auto bitProxy0 = b.getBitProxy(0ULL);

  bool isZeroBitSet = bitProxy0;
  EXPECT_TRUE(isZeroBitSet);
  constexpr auto bitProxy1 = b.getBitProxy(1ULL);
  EXPECT_FALSE(bitProxy1.isSet());
  constexpr auto bitProxy2 = b.getBitProxy(2ULL);
  EXPECT_TRUE(bitProxy2.isSet());
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELDTEST_H_10697265841863131580121343099140651261122718 
