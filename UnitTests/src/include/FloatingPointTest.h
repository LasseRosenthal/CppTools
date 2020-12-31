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
#include <Utils/miscellaneous.h>

#include <iostream>
#include <limits>
#include <sstream>

using namespace std::string_literals;
 
 
TEST(FloatingPoint, ZeroFloat)
{
  constexpr cpptools::FloatingPoint<float> f;
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.characteristic(), 0);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, ZeroDouble)
{
  constexpr cpptools::FloatingPoint<double> f;
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.characteristic(), 0);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, negativeZeroFloat)
{
  constexpr cpptools::FloatingPoint<float> f(-0.0F);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.characteristic(), 0);
  EXPECT_EQ(f.signBit(), 1);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, negativeZeroDouble)
{
  constexpr cpptools::FloatingPoint<double> f(-0.0);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.characteristic(), 0);
  EXPECT_EQ(f.signBit(), 1);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, isZeroNegativeFloat)
{
  constexpr cpptools::FloatingPoint<float> f(-0.0F);
  EXPECT_TRUE(f.isNegative());
  EXPECT_FALSE(f.isPositive());
  EXPECT_TRUE(f.isZero());
}

TEST(FloatingPoint, isZeroNegativeDouble)
{
  constexpr cpptools::FloatingPoint<double> f(-0.0);
  EXPECT_TRUE(f.isNegative());
  EXPECT_FALSE(f.isPositive());
  EXPECT_TRUE(f.isZero());
}

TEST(FloatingPoint, isZeroPositiveFloat)
{
  constexpr cpptools::FloatingPoint<float> f(0.0F);
  EXPECT_FALSE(f.isNegative());
  EXPECT_TRUE(f.isPositive());
  EXPECT_TRUE(f.isZero());
}

TEST(FloatingPoint, isZeroPositiveDouble)
{
  constexpr cpptools::FloatingPoint<double> f(0.0);
  EXPECT_FALSE(f.isNegative());
  EXPECT_TRUE(f.isPositive());
  EXPECT_TRUE(f.isZero());
}

TEST(FloatingPoint, OneFloat)
{
  constexpr cpptools::FloatingPoint<float> f(1.0F);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.characteristic(), 127);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, OneDouble)
{
  constexpr cpptools::FloatingPoint<double> f(1.0);
  EXPECT_EQ(f.mantissa(), 0);
  EXPECT_EQ(f.characteristic(), 1023);
  EXPECT_EQ(f.signBit(), 0);
}

TEST(FloatingPoint, expectNegativeTrueFloat)
{
  constexpr cpptools::FloatingPoint<float> f(-134.0687F);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, expectNegativeTrueDouble)
{
  constexpr cpptools::FloatingPoint<double> f(-134.0687);
  EXPECT_TRUE(f.isNegative());
}

TEST(FloatingPoint, expectNegativeFalseFloat)
{
  constexpr cpptools::FloatingPoint<float> f(134.0687F);
  EXPECT_FALSE(f.isNegative());
}

TEST(FloatingPoint, expectNegativeFalseDouble)
{
  constexpr cpptools::FloatingPoint<double> f(134.0687);
  EXPECT_FALSE(f.isNegative());
}

TEST(FloatingPoint, ZeroPointTwo)
{
  constexpr cpptools::FloatingPoint<float> f(0.2F);
  EXPECT_EQ(f.mantissa(), 0x4CCCCD);
  EXPECT_EQ(f.characteristic(), 124);
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

TEST(FloatingPoint, normalFloat)
{
  constexpr float zero = 0.0F;
  cpptools::FloatingPoint<float> f(1365, 1, 1);
  EXPECT_TRUE(f.isNormal());
  EXPECT_EQ(f.exponent(), -126);
}

TEST(FloatingPoint, subNormalFloat)
{
  constexpr float zero = 0.0F;
  cpptools::FloatingPoint<float> f(1365, 0, 1);
  EXPECT_TRUE(f.isSubNormal());
  EXPECT_EQ(f.exponent(), -126);
}

TEST(FloatingPoint, streamOperator)
{
  constexpr float zero = 0.0F;
  cpptools::FloatingPoint<float> f(0, 0, 1);

  std::stringstream sstr;
  sstr << f;
  const auto expected = "1 00000000 00000000000000000000000";
  EXPECT_EQ(sstr.str(), expected);
}

TEST(FloatingPoint, maxValFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto maxVal = FloatT::maxVal();

  std::cout << maxVal << '\n';

  auto maxValInt = static_cast<intType>(maxVal);
  cpptools::binaryRep(maxValInt, std::cout);
  std::cout << '\n';

  auto m         = (std::numeric_limits<type>::max)();

  auto maxIntExpected = *reinterpret_cast<intType*>(&m);

  cpptools::binaryRep(maxIntExpected, std::cout);
  std::cout << '\n';

  EXPECT_EQ(maxValInt, maxIntExpected);
  EXPECT_EQ(static_cast<type>(maxVal), (std::numeric_limits<type>::max)());
}

TEST(FloatingPoint, maxValDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto maxVal = FloatT::maxVal();
  auto maxValInt = static_cast<intType>(maxVal);
  auto m         = (std::numeric_limits<type>::max)();
  auto maxIntExpected = *reinterpret_cast<intType*>(&m);

  EXPECT_EQ(maxValInt, maxIntExpected);
  EXPECT_EQ(static_cast<type>(maxVal), (std::numeric_limits<type>::max)());
}

TEST(FloatingPoint, minValFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto minValNormal      = FloatT::minValNormal();
  auto minValInt         = static_cast<intType>(minValNormal);
  auto minValExpected    = (std::numeric_limits<type>::min)();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValNormal), minValExpected);
}

TEST(FloatingPoint, minValDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto minValNormal      = FloatT::minValNormal();
  auto minValInt         = static_cast<intType>(minValNormal);
  auto minValExpected    = (std::numeric_limits<type>::min)();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValNormal), minValExpected);
}

TEST(FloatingPoint, minValSubnormalFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto minValSubNormal   = FloatT::minValSubNormal();
  auto minValInt         = static_cast<intType>(minValSubNormal);
  auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValSubNormal), minValExpected);
}

TEST(FloatingPoint, minValSubnormalDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto minValSubNormal   = FloatT::minValSubNormal();
  auto minValInt         = static_cast<intType>(minValSubNormal);
  auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValSubNormal), minValExpected);
}

TEST(FloatingPoint, lowestFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto lowestVal            = FloatT::lowest();
  auto lowestValInt         = static_cast<intType>(lowestVal);
  auto lowestValExpected    = std::numeric_limits<type>::lowest();
  auto lowestValIntExpected = *reinterpret_cast<intType*>(&lowestValExpected);

  EXPECT_EQ(lowestValInt, lowestValIntExpected);
  EXPECT_EQ(static_cast<type>(lowestVal), lowestValExpected);
}

TEST(FloatingPoint, lowestDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  auto lowestVal            = FloatT::lowest();
  auto lowestValInt         = static_cast<intType>(lowestVal);
  auto lowestValExpected    = std::numeric_limits<type>::lowest();
  auto lowestValIntExpected = *reinterpret_cast<intType*>(&lowestValExpected);

  EXPECT_EQ(lowestValInt, lowestValIntExpected);
  EXPECT_EQ(static_cast<type>(lowestVal), lowestValExpected);
}

TEST(FloatingPoint, incrementZeroFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  FloatT zero(0.0F);
  ++zero;

  auto minValInt         = static_cast<intType>(zero);
  auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(zero), minValExpected);
}

TEST(FloatingPoint, incrementZeroDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  FloatT zero(0.0);
  ++zero;

  auto minValInt         = static_cast<intType>(zero);
  auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(zero), minValExpected);
}

TEST(FloatingPoint, incrementNegativeZeroFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  FloatT zero(-0.0F);
  ++zero;

  auto minValInt         = static_cast<intType>(zero);
  auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(zero), minValExpected);
}

TEST(FloatingPoint, incrementNegativeZeroDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  FloatT zero(-0.0);
  zero++;

  auto minValInt         = static_cast<intType>(zero);
  auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto minValIntExpected = *reinterpret_cast<intType*>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(zero), minValExpected);
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FLOATINGPOINTTEST_H_20818109791325315054228722271510173308867116 
