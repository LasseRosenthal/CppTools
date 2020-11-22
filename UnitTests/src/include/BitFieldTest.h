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

#include <sstream>
 

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

  const std::string expected{"00011000"};
  std::stringstream sstr;
  sstr << b;

  EXPECT_EQ(sstr.str(), expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELDTEST_H_10697265841863131580121343099140651261122718 
