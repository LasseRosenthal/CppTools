/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    RatioUtilsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.11.2020 
 */ 
 
#ifndef RATIOUTILSTEST_H_23937178279930216031622411838288212494131040 
#define RATIOUTILSTEST_H_23937178279930216031622411838288212494131040 
 
 
// includes 
#include <Meta/RatioUtils.h> 
 
using namespace meta;


TEST(RatioUtils, AsDecimalT)
{
  constexpr double expected = 17.0 / 7.0;
  constexpr auto dec = AsDecimal<std::ratio<17, 7>>;
  EXPECT_EQ(dec, expected);
}

TEST(RatioUtils, Negative)
{
  EXPECT_TRUE((std::ratio_equal_v<Negative<std::ratio<4143, 30003>>, std::ratio<-4143, 30003>>));
}

TEST(RatioUtils, IsIntegralRatioExpectTrue)
{
  EXPECT_TRUE((IsIntegralRatio<std::ratio<14, 7>>));
}

TEST(RatioUtils, IsIntegralRatioExpectFalse)
{
  EXPECT_FALSE((IsIntegralRatio<std::ratio<18, 7>>));
}

TEST(RatioUtils, HasZeroNominatorExpectTrue)
{
  EXPECT_TRUE((IsIntegralRatio<std::ratio<0, 7>>));
}

TEST(RatioUtils, HasZeroNominatorExpectFalse)
{
  EXPECT_FALSE((IsIntegralRatio<std::ratio<18, 7>>));
}

TEST(RatioUtils, RatioPowPositiveExponent)
{
  EXPECT_TRUE((std::ratio_equal_v<RatioPow<std::ratio<4, 3>, 4>, std::ratio<256, 81>>));
}

TEST(RatioUtils, RatioPowNegativeExponent)
{
  EXPECT_TRUE((std::ratio_equal_v<RatioPow<std::ratio<4, 3>, -4>, std::ratio<81, 256>>));
}

TEST(RatioUtils, RatioPowExponentEqualsZero)
{
  EXPECT_TRUE((std::ratio_equal_v<RatioPow<std::ratio<4, 3>, 0>, std::ratio<1>>));
}

TEST(RatioUtils, Invert)
{
  EXPECT_TRUE((std::ratio_equal_v<Invert<std::ratio<4143, 30003>>, std::ratio<30003, 4143>>));
}

TEST(RatioUtils, RatioPower)
{
  constexpr auto val = RatioPower<std::ratio<12, 5>, std::ratio<5, 3>>;
  EXPECT_EQ(val, 4.302148556694878120397582733477);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // RATIOUTILSTEST_H_23937178279930216031622411838288212494131040 
