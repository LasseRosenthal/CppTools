/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FloatingPointTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    23.12.2020 
 */ 
 
#ifndef FLOATINGPOINTTEST_H_20818109791325315054228722271510173308867116 
#define FLOATINGPOINTTEST_H_20818109791325315054228722271510173308867116 
 
 
// includes
#include <Utils/FloatingPoint.h>
 
 
TEST(FloatingPoint, ZeroFloat)
{
  cpptools::FloatingPoint<float> f;
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.exponent(), 0);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, ZeroDouble)
{
  cpptools::FloatingPoint<double> f;
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.exponent(), 0);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, negativeZeroFloat)
{
  cpptools::FloatingPoint<float> f(-0.0F);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.exponent(), 0);
  EXPECT_EQ(f.signBit(), 1);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, negativeZeroDouble)
{
  cpptools::FloatingPoint<double> f(-0.0);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.exponent(), 0);
  EXPECT_EQ(f.signBit(), 1);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, OneFloat)
{
  cpptools::FloatingPoint<float> f(1.0F);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.exponent(), 127);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, OneDouble)
{
  cpptools::FloatingPoint<double> f(1.0);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.exponent(), 1023);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, expectNegativeTrueFloat)
{
  cpptools::FloatingPoint<float> f(-134.0687F);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, expectNegativeTrueDouble)
{
  cpptools::FloatingPoint<double> f(-134.0687);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, expectNegativeFalseFloat)
{
  cpptools::FloatingPoint<float> f(134.0687F);
  EXPECT_FALSE(f.isNegative());
}

TEST(FloatingPoint, expectNegativeFalseDouble)
{
  cpptools::FloatingPoint<double> f(134.0687);
  EXPECT_FALSE(f.isNegative());
}

TEST(FloatingPoint, ZeroPointTwo)
{
  cpptools::FloatingPoint<float> f(0.2F);
  EXPECT_EQ(f.mantissa(), 0x4CCCCD);
  EXPECT_EQ(f.exponent(), 124);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, ExpectInfinityFloat)
{
  float zero = 0.0F;
  cpptools::FloatingPoint<float> f(1.0F / zero);
  EXPECT_TRUE(f.isInfinity());
}

TEST(FloatingPoint, ExpectInfinityDouble)
{
  double zero = 0.0;
  cpptools::FloatingPoint<double> f(1.0 / zero);
  EXPECT_TRUE(f.isInfinity());
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FLOATINGPOINTTEST_H_20818109791325315054228722271510173308867116 
