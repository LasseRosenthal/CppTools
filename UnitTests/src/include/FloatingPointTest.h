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
  constexpr cpptools::FloatingPoint<float> f(1365, 1, 1);
  EXPECT_TRUE(f.isNormal());
  EXPECT_EQ(f.exponent(), -126);
}

TEST(FloatingPoint, subNormalFloat)
{
  constexpr float zero = 0.0F;
  constexpr cpptools::FloatingPoint<float> f(1365, 0, 1);
  EXPECT_TRUE(f.isSubNormal());
  EXPECT_EQ(f.exponent(), -126);
}

TEST(FloatingPoint, streamOperator)
{
  constexpr float zero = 0.0F;
  constexpr cpptools::FloatingPoint<float> f(0, 0, 1);

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

  constexpr auto maxVal            = FloatT::maxVal();
  constexpr auto maxValInt         = static_cast<intType>(maxVal);
  constexpr auto maxValExpected    = (std::numeric_limits<type>::max)();
  auto           maxValIntExpected = *reinterpret_cast<intType const* const>(&maxValExpected);

  EXPECT_EQ(maxValInt, maxValIntExpected);
  EXPECT_EQ(static_cast<type>(maxVal), (std::numeric_limits<type>::max)());
}

TEST(FloatingPoint, maxValDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto maxVal            = FloatT::maxVal();
  constexpr auto maxValInt         = static_cast<intType>(maxVal);
  constexpr auto maxValExpected    = (std::numeric_limits<type>::max)();
  auto           maxValIntExpected = *reinterpret_cast<intType const* const>(&maxValExpected);

  EXPECT_EQ(maxValInt, maxValIntExpected);
  EXPECT_EQ(static_cast<type>(maxVal), (std::numeric_limits<type>::max)());
}

TEST(FloatingPoint, minValFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto minValNormal      = FloatT::minValNormal();
  constexpr auto minValInt         = static_cast<intType>(minValNormal);
  constexpr auto minValExpected    = (std::numeric_limits<type>::min)();
  auto           minValIntExpected = *reinterpret_cast<intType const* const>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValNormal), minValExpected);
}

TEST(FloatingPoint, minValDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto minValNormal      = FloatT::minValNormal();
  constexpr auto minValInt         = static_cast<intType>(minValNormal);
  constexpr auto minValExpected    = (std::numeric_limits<type>::min)();
  auto           minValIntExpected = *reinterpret_cast<intType const* const>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValNormal), minValExpected);
}

TEST(FloatingPoint, minValSubnormalFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto minValSubNormal   = FloatT::minValSubNormal();
  constexpr auto minValInt         = static_cast<intType>(minValSubNormal);
  constexpr auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto           minValIntExpected = *reinterpret_cast<intType const* const>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValSubNormal), minValExpected);
}

TEST(FloatingPoint, minValSubnormalDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto minValSubNormal   = FloatT::minValSubNormal();
  constexpr auto minValInt         = static_cast<intType>(minValSubNormal);
  constexpr auto minValExpected    = std::numeric_limits<type>::denorm_min();
  auto           minValIntExpected = *reinterpret_cast<intType const* const>(&minValExpected);

  EXPECT_EQ(minValInt, minValIntExpected);
  EXPECT_EQ(static_cast<type>(minValSubNormal), minValExpected);
}

TEST(FloatingPoint, lowestFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto lowestVal            = FloatT::lowest();
  constexpr auto lowestValInt         = static_cast<intType>(lowestVal);
  constexpr auto lowestValExpected    = std::numeric_limits<type>::lowest();
  const auto     lowestValIntExpected = *reinterpret_cast<intType const* const>(&lowestValExpected);

  EXPECT_EQ(lowestValInt, lowestValIntExpected);
  EXPECT_EQ(static_cast<type>(lowestVal), lowestValExpected);
}

TEST(FloatingPoint, lowestDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  using intType = FloatT::intType;

  constexpr auto lowestVal            = FloatT::lowest();
  constexpr auto lowestValInt         = static_cast<intType>(lowestVal);
  constexpr auto lowestValExpected    = std::numeric_limits<type>::lowest();
  const auto     lowestValIntExpected = *reinterpret_cast<intType const* const>(&lowestValExpected);

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

TEST(FloatingPoint, infinityFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr auto inf = FloatT::infinity();
  EXPECT_TRUE(inf.isInfinity());
}

TEST(FloatingPoint, infinityDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr auto inf = FloatT::infinity();
  EXPECT_TRUE(inf.isInfinity());
}

TEST(FloatingPoint, nextOfZeroFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT zero;
  auto nextOfZero = zero.next();
  constexpr auto minValSubNormal = FloatT::minValSubNormal();
  EXPECT_EQ(static_cast<type>(nextOfZero), static_cast<type>(minValSubNormal));
}

TEST(FloatingPoint, nextOfZeroDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT zero(-0.0);
  auto nextOfZero = zero.next();
  constexpr auto minValSubNormal = FloatT::minValSubNormal();
  EXPECT_EQ(static_cast<type>(nextOfZero), static_cast<type>(minValSubNormal));
}

TEST(FloatingPoint, previousOfZeroFloat)
{
  using type   = float;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT zero;
  auto             previousOfZero = zero.previous();
  constexpr auto   minValSubNormal = FloatT::minValSubNormal();
  EXPECT_EQ(-1.0F*static_cast<type>(previousOfZero), static_cast<type>(minValSubNormal));
  EXPECT_EQ(FloatT::distanceInULP(previousOfZero, minValSubNormal), 2);
}

TEST(FloatingPoint, previousOfZeroDouble)
{
  using type   = double;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT zero(-0.0);
  auto             previousOfZero = zero.previous();
  constexpr auto   minValSubNormal = FloatT::minValSubNormal();
  EXPECT_EQ(-1.0*static_cast<type>(previousOfZero), static_cast<type>(minValSubNormal));
  EXPECT_EQ(FloatT::distanceInULP(previousOfZero, minValSubNormal), 2);
}


TEST(FloatingPoint, distanceInULPBothZeroFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT f1(0.0F);
  constexpr FloatT f2(-0.0F);
  const auto distance = FloatT::distanceInULP(f1, f2);
  const std::size_t expectedDistance = 0ULL;
  EXPECT_EQ(distance, expectedDistance);
}

TEST(FloatingPoint, distanceInULPBothZeroDouble)
{
  using type = double;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT f1(0.0F);
  constexpr FloatT f2(-0.0F);
  const auto distance = FloatT::distanceInULP(f1, f2);
  const std::size_t expectedDistance = 0ULL;
  EXPECT_EQ(distance, expectedDistance);
}


TEST(FloatingPoint, distanceInULPZeroToNextFloat)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  constexpr FloatT f1 = FloatT::minValSubNormal();
  constexpr FloatT f2(-0.0F);
  const auto distance = FloatT::distanceInULP(f1, f2);
  const std::size_t expectedDistance = 1ULL;
  EXPECT_EQ(distance, expectedDistance);
}

TEST(FloatingPoint, distanceInULP)
{
  using type = float;
  using FloatT = cpptools::FloatingPoint<type>;
  FloatT f1;
  FloatT f2;

  for(int i = 0; i < 10265; ++i)
  {
    ++f1;
  }
  for(int i = 0; i < 134; ++i)
  {
    ++f2;
  }
  const auto distance = FloatT::distanceInULP(f1, f2);
  const std::size_t expectedDistance = 10265ULL - 134ULL;
  EXPECT_EQ(distance, expectedDistance);
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FLOATINGPOINTTEST_H_20818109791325315054228722271510173308867116 
