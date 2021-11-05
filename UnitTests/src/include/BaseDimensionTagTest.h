/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BaseDimensionTagTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    17.11.2020 
 */ 
 
#ifndef BASEDIMENSIONTAGTEST_H_335483821791023250245492102920911307639 
#define BASEDIMENSIONTAGTEST_H_335483821791023250245492102920911307639 
 
 
// includes
#include <Unit/BaseDimensions.h>
 
using namespace unit::dimension;
 
TEST(BaseDimensions, TagGeneratorExpectRationalExponent)
{
  using myDim = BaseDimensionTag<0>;
  EXPECT_TRUE(myDim::hasRationalExponent);
}

TEST(BaseDimensions, TagGeneratorExpectNonRationalExponent)
{
  using myDim = BaseDimensionTag<6, std::ratio<3,2>>;
  EXPECT_FALSE(myDim::hasRationalExponent);
}

TEST(BaseDimensions, NthRoot)
{
  constexpr std::size_t N = 2ULL;
  constexpr long long num = 2LL;
  constexpr long long den = 3LL;

  using myDim = BaseDimensionTag<6, std::ratio<num, den>>;

  using generatedType = unit::dimension::NthRootOfBaseTag<myDim, N>;
  using expectedType = BaseDimensionTag<6, std::ratio<1, 3>>;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BASEDIMENSIONTAGTEST_H_335483821791023250245492102920911307639 
