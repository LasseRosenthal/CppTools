/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    UnitTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    26.11.2020 
 */ 
 
#ifndef UNITTEST_H_19645270133266128954221751440023422238912390 
#define UNITTEST_H_19645270133266128954221751440023422238912390 
 
 
// includes
#include <Unit/Unit.h>
 
using namespace unit;
using namespace unit::literals;
 

TEST(Unit, DefaultConstructor)
{
  seconds sec;
  EXPECT_EQ(sec.value(), 0.0);
}
TEST(Unit, Constructor)
{
  seconds sec{1.4};
  EXPECT_EQ(sec.value(), 1.4);
}

TEST(Unit, ConversionDaysToSeconds)
{
  constexpr auto d = 1.0_d;
  seconds sec{d};
  constexpr double expectedValue = 86400.0;
  EXPECT_EQ(sec.value(), expectedValue);
}

TEST(Unit, ConversionYardsToMeters)
{
  constexpr meters yardInMeters = 1.0_yd;
  constexpr double expectedValue = 0.9144;
  EXPECT_EQ(yardInMeters.value(), expectedValue);
}

TEST(Unit, ConversionSecondsToDays)
{
  constexpr auto   s = 108000.0_s;
  days          d{s};
  constexpr double expectedValue = 1.25;
  EXPECT_EQ(d.value(), expectedValue);
}

TEST(Unit, ConversionSecondsToYears)
{
  constexpr auto   s = 1.0_s;
  years            y{s};
  constexpr double expectedValue = 1.0 / 31556952.0;
  EXPECT_EQ(y.value(), expectedValue);
}

TEST(Unit, ConversionWithScaling)
{
  using myVelocityUnit = unit::Velocity<double, std::ratio<3600>, std::kilo, std::ratio<3, 2>>;

  auto v         = 12.0_kmh;
  myVelocityUnit u = v;
  EXPECT_EQ(u.value(), 12.0 / 1.5);
}


TEST(Unit, AdditionSeconds)
{
  auto s1 = 3.0_s;
  s1 += 2.0_s;
  constexpr double expectedValue = 5.0;
  EXPECT_EQ(s1.value(), expectedValue);
}

TEST(Unit, AdditionSecondsWithDays)
{
  auto s1 = 3.0_s;
  s1 += 1.0_d;
  constexpr double expectedValue = 86403.0;
  EXPECT_EQ(s1.value(), expectedValue);
}

TEST(Unit, AdditionSecondsWithDaysFreeOperator)
{
  constexpr auto   s             = 3.256_s + 1.0_d;
  constexpr double expectedValue = 86403.256;
  EXPECT_EQ(s.value(), expectedValue);
}

TEST(Unit, SubtractionMileWithMeters)
{
  constexpr meters met = 1.0_mile - 2.5_m;
  constexpr double expectedValue = 1609.344 - 2.5;
  EXPECT_EQ(met.value(), expectedValue);
}


TEST(Unit, MultiplicationWithScalar)
{
  auto s1 = 3.2_s;
  s1 *= 2.0;
  constexpr double expectedValue = 6.4;
  EXPECT_EQ(s1.value(), expectedValue);
}

TEST(Unit, DivisionByScalar)
{
  auto s1 = 3.2_s;
  s1 /= 2.0;
  constexpr double expectedValue = 1.6;
  EXPECT_EQ(s1.value(), expectedValue);
}

TEST(Unit, DivisionOfUnitsYieldingNumericalValue)
{
  auto value = 200.0_kmh / 10.0_kmh;
  EXPECT_TRUE((std::is_same_v<decltype(value), double>));
}

TEST(Unit, DivisionOfUnitsNoConversion)
{
  constexpr auto   mPerS         = 200.0_m / 10.0_s;
  constexpr double expectedValue = 20.0;
  EXPECT_EQ(mPerS.value(), expectedValue);
}
TEST(Unit, DivisionOfUnitsOneConversion)
{
  constexpr kilometersPerHour kmh           = 253.0_m / 10.0_s;
  constexpr double            expectedValue = 2.53 * 36.0;
  EXPECT_EQ(kmh.value(), expectedValue);
}

TEST(Unit, DivisionOfUnitsMultipleConversions)
{
  constexpr mile              len           = 253.0_m;
  constexpr weeks             t             = 10.0_s;
  constexpr kilometersPerHour kmh           = len / t;
  constexpr double            expectedValue = 2.53 * 36.0;
  EXPECT_EQ(kmh.value(), expectedValue);
}

TEST(Unit, InvertSecondTestValue)
{
  constexpr auto freq = 1.0 / 2.0_s;
  EXPECT_EQ(freq.value(), 0.5);
}

TEST(Unit, InvertSecondTestType)
{
  auto freq = 1.0 / 2.0_s;
  EXPECT_TRUE((std::is_same_v<decltype(freq), hertz>));
}


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITTEST_H_19645270133266128954221751440023422238912390 
