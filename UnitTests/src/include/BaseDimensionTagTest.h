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
  using myDim = DimensionTagGenerator<0>;
  EXPECT_TRUE(myDim::hasRationalExponent);
}

TEST(BaseDimensions, TagGeneratorExpectNonRationalExponent)
{
  using myDim = DimensionTagGenerator<6, std::ratio<3,2>>;
  EXPECT_FALSE(myDim::hasRationalExponent);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BASEDIMENSIONTAGTEST_H_335483821791023250245492102920911307639 
