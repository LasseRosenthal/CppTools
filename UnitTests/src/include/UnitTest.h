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
#include <Unit/Units.h>

#include <sstream>

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

TEST(Unit, streamOperatorBits)
{
  constexpr auto    bit = 1.24_GB;
  std::stringstream ostr;
  ostr << bit;
  std::string expected = "1.24 GB";
  EXPECT_EQ(ostr.str(), expected);
}

TEST(Unit, AssignNewValue)
{
  auto s = 1.24_s;
  constexpr double newValue = 2.975;
  s = newValue;

  EXPECT_EQ(s.value(), newValue);
}

TEST(Unit, floor)
{
  constexpr auto pix      = 45.12_pix;
  constexpr auto pixFloor = unit::floor(pix);
  constexpr auto pixCeil  = unit::ceil(pix);

  EXPECT_EQ(pixFloor.value(), 45.0);
  EXPECT_EQ(pixCeil.value(), 46.0);
}

TEST(Unit, equalityOperatorExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr auto m2 = 1.35_m;

  const bool equal = m1 == m2;
  EXPECT_TRUE(equal);
}

TEST(Unit, equalityOperatorExpectFalse)
{
  constexpr auto m1 = 1.351_m;
  constexpr auto m2 = 1.35101_m;

  const bool equal = m1 == m2;
  EXPECT_FALSE(equal);
}

TEST(Unit, equalityOperatorWithScalarLastExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr double m2 = 1.35;

  const bool equal = m1 == m2;
  EXPECT_TRUE(equal);
}

TEST(Unit, equalityOperatorWithScalarLastExpectFalse)
{
  constexpr auto m1 = 1.351_m;
  constexpr double m2 = 1.35101;

  const bool equal = m1 == m2;
  EXPECT_FALSE(equal);
}

TEST(Unit, equalityOperatorWithScalarFirstExpectTrue)
{
  constexpr double m1 = 1.35;
  constexpr auto m2 = 1.35_m;

  const bool equal = m1 == m2;
  EXPECT_TRUE(equal);
}

TEST(Unit, equalityOperatorWithScalarFirstExpectFalse)
{
  constexpr double m1 = 1.35101;
  constexpr auto m2 = 1.351_m;

  const bool equal = m1 == m2;
  EXPECT_FALSE(equal);
}


TEST(Unit, lessOperatorExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr auto m2 = 1.36_m;
  constexpr bool less = m1 < m2;
  EXPECT_TRUE(less);
}

TEST(Unit, lessOperatorExpectFalse)
{
  constexpr auto m1 = 1.372_m;
  constexpr auto m2 = 1.362_m;
  constexpr bool less = m1 < m2;
  EXPECT_FALSE(less);
}

TEST(Unit, lessOrEqualOperatorExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr auto m2 = 1.35_m;
  constexpr bool less = m1 <= m2;
  EXPECT_TRUE(less);
}

TEST(Unit, lessOrEqualOperatorExpectFalse)
{
  constexpr auto m1 = 1.37_m;
  constexpr auto m2 = 1.36_m;
  constexpr bool less = m1 <= m2;
  EXPECT_FALSE(less);
}

TEST(Unit, greaterOperatorExpectTrue)
{
  constexpr auto m1 = 1.37_m;
  constexpr auto m2 = 1.36_m;
  constexpr bool less = m1 > m2;
  EXPECT_TRUE(less);
}

TEST(Unit, greaterOperatorExpectFalse)
{
  constexpr auto m1 = 1.352_m;
  constexpr auto m2 = 1.362_m;
  constexpr bool less = m1 > m2;
  EXPECT_FALSE(less);
}

TEST(Unit, greaterOrEqualOperatorExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr auto m2 = 1.35_m;
  constexpr bool less = m1 >= m2;
  EXPECT_TRUE(less);
}

TEST(Unit, greaterOrEqualOperatorExpectFalse)
{
  constexpr auto m1 = 1.34_m;
  constexpr auto m2 = 1.36_m;
  constexpr bool less = m1 >= m2;
  EXPECT_FALSE(less);
}

TEST(Unit, lessOperatorWithScalarLastExpectTrue)
{
  constexpr auto m1 = 1.34_m;
  constexpr double m2 = 1.35;
  constexpr bool less = m1 < m2;
  EXPECT_TRUE(less);
}

TEST(Unit, lessOperatorWithScalarLastExpectFalse)
{
  constexpr auto m1 = 1.34_m;
  constexpr double m2 = 1.33;
  constexpr bool less = m1 < m2;
  EXPECT_FALSE(less);
}

TEST(Unit, lessOrEqualOperatorWithScalarLastExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr double m2 = 1.35;
  constexpr bool less = m1 <= m2;
  EXPECT_TRUE(less);
}

TEST(Unit, lessOrEqualOperatorWithScalarLastExpectFalse)
{
  constexpr auto m1 = 1.34_m;
  constexpr double m2 = 1.33;
  constexpr bool less = m1 <= m2;
  EXPECT_FALSE(less);
}

TEST(Unit, greaterOperatorWithScalarLastExpectTrue)
{
  constexpr auto m1 = 1.37_m;
  constexpr double m2 = 1.35;
  constexpr bool less = m1 > m2;
  EXPECT_TRUE(less);
}

TEST(Unit, greaterOperatorWithScalarLastExpectFalse)
{
  constexpr auto m1 = 1.33_m;
  constexpr double m2 = 1.33;
  constexpr bool less = m1 > m2;
  EXPECT_FALSE(less);
}

TEST(Unit, greaterOrEqualOperatorWithScalarLastExpectTrue)
{
  constexpr auto m1 = 1.35_m;
  constexpr double m2 = 1.35;
  constexpr bool less = m1 >= m2;
  EXPECT_TRUE(less);
}

TEST(Unit, greaterOrEqualOperatorWithScalarLastExpectFalse)
{
  constexpr auto m1 = 1.34_m;
  constexpr double m2 = 1.35;
  constexpr bool less = m1 >= m2;
  EXPECT_FALSE(less);
}

TEST(Unit, lessOperatorWithScalarFirstExpectTrue)
{
  constexpr double m1 = 1.33;
  constexpr auto m2 = 1.34_m;
  constexpr bool less = m1 < m2;
  EXPECT_TRUE(less);
}

TEST(Unit, lessOperatorWithScalarFirstExpectFalse)
{
  constexpr double m1 = 1.39;
  constexpr auto m2 = 1.34_m;
  constexpr bool less = m1 < m2;
  EXPECT_FALSE(less);
}

TEST(Unit, lessOrEqualOperatorWithScalarFirstExpectTrue)
{
  constexpr double m1 = 1.35;
  constexpr auto m2 = 1.35_m;
  constexpr bool less = m1 <= m2;
  EXPECT_TRUE(less);
}

TEST(Unit, lessOrEqualOperatorWithScalarFirstExpectFalse)
{
  constexpr double m1 = 1.341;
  constexpr auto m2 = 1.34_m;
  constexpr bool less = m1 <= m2;
  EXPECT_FALSE(less);
}

TEST(Unit, greaterOperatorWithScalarFirstExpectTrue)
{
  constexpr double m1 = 1.372;
  constexpr auto m2 = 1.37_m;
  constexpr bool less = m1 > m2;
  EXPECT_TRUE(less);
}

TEST(Unit, greaterOperatorWithScalarFirstExpectFalse)
{
  constexpr double m1 = 1.33;
  constexpr auto m2 = 1.33_m;
  constexpr bool less = m1 > m2;
  EXPECT_FALSE(less);
}

TEST(Unit, greaterOrEqualOperatorWithScalarFirstExpectTrue)
{
  constexpr double m1 = 1.35;
  constexpr auto m2 = 1.35_m;
  constexpr bool less = m1 >= m2;
  EXPECT_TRUE(less);
}

TEST(Unit, greaterOrEqualOperatorWithScalarFirstExpectFalse)
{
  constexpr double m1 = 1.35;
  constexpr auto m2 = 1.36_m;
  constexpr bool less = m1 >= m2;
  EXPECT_FALSE(less);
}





TEST(Unit, round)
{
  const auto pix1Round = unit::round(45.12_pix);
  EXPECT_EQ(pix1Round.value(), 45.0);

  const auto pix2Round = unit::round(45.5_pix);
  EXPECT_EQ(pix2Round.value(), 46.0);

  const auto pix3Round = unit::round(95.4999_pix);
  EXPECT_EQ(pix3Round.value(), 95.0);

  const auto pix4Round = unit::round(95.501_pix);
  EXPECT_EQ(pix4Round.value(), 96.0);
}

TEST(Unit, ConversionInchToMeter)
{
  constexpr auto   oneInch = 1.0_in;
  constexpr meters m       = oneInch;
  EXPECT_EQ(m.value(), 0.0254);
}

TEST(Unit, ConversionPointsToInch)
{
  constexpr auto oneInchInPoints = 72.0_pt;
  constexpr inch m               = oneInchInPoints;
  EXPECT_EQ(m.value(), 1.0);
}

TEST(Unit, CalculationsWithResolution)
{
  constexpr auto res = 2540.0_dpi;

  constexpr auto         pt              = 72.0_pt;
  constexpr unit::pixels pixelsFromPoint = pt * res;
  constexpr unit::pixels pixelsFromInch  = 1.0_in * res;

  EXPECT_EQ(pixelsFromPoint.value(), 2540.0);
  EXPECT_EQ(pixelsFromInch.value(), 2540.0);

  constexpr double       ptValue = 11.2;
  constexpr unit::points p{ptValue};
  constexpr unit::pixels pixelsFromPoint2 = p * res;

  EXPECT_EQ(pixelsFromPoint2.value(), ptValue * (2540.0 / 72.0));

  constexpr double       numPixels = 28.56;
  constexpr unit::points i         = (unit::pixels{numPixels}) / res;
  EXPECT_EQ(i.value(), 72.0 * (numPixels / res.value()));
}

TEST(Unit, RadToDegree)
{
  constexpr auto    rad = 1.0_rad;
  constexpr degrees deg = rad;

  constexpr double expected1 = 57.2957795130823208;
  EXPECT_EQ(deg.value(), expected1);

  constexpr auto    halfCircleDegree  = 180.0_deg;
  constexpr radians halfCircleRadians = halfCircleDegree;
  constexpr double  expected2         = 3.1415926535897932;
  EXPECT_EQ(halfCircleRadians.value(), expected2);
}

TEST(Unit, ConversionDaysToSeconds)
{
  constexpr auto   d = 1.0_d;
  seconds          sec{d};
  constexpr double expectedValue = 86400.0;
  EXPECT_EQ(sec.value(), expectedValue);
}

TEST(Unit, ConversionYardsToMeters)
{
  constexpr meters yardInMeters  = 1.0_yd;
  constexpr double expectedValue = 0.9144;
  EXPECT_EQ(yardInMeters.value(), expectedValue);
}

TEST(Unit, ConversionSecondsToDays)
{
  constexpr auto   s = 108000.0_s;
  days             d{s};
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

  auto           v = 12.0_kmh;
  myVelocityUnit u = v;
  EXPECT_EQ(u.value(), 12.0 / 1.5);
}

TEST(Unit, ConversionSquareMetersToSquareDecimeters)
{
  constexpr auto             fac = 1346.12;
  constexpr auto             sm  = fac * 1.0_qm;
  constexpr squaredecimeters sdm = sm;
  EXPECT_EQ(sdm.value(), fac * 100.0);
}

TEST(Unit, ConversionSquareInchToSquareCentimeters)
{
  constexpr auto              fac = 3.156;
  constexpr auto              sm  = fac * 1.0_qin;
  constexpr squarecentimeters sdm = sm;
  EXPECT_EQ(sdm.value(), fac * 6.4516);
}

TEST(Unit, ConversionAcreToSquareCentimeters)
{
  constexpr auto              fac = 1.0;
  constexpr auto              ac  = 1.0_ac;
  constexpr squarecentimeters sdm = ac;
  EXPECT_EQ(sdm.value(), fac * 40468564.224);
}

TEST(Unit, Resolution)
{
  constexpr auto res      = 1200.0_dpi;
  constexpr auto pix      = res * 1050.0_mm;
  constexpr int  expected = 49607;
  EXPECT_EQ(std::ceil(pix.value()), expected);
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
  constexpr meters met           = 1.0_mi - 2.5_m;
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

TEST(Unit, MultiplicationWithScalarFreeOperatorUnitFirst)
{
  constexpr auto   s1            = 3.2_s;
  constexpr double factor        = 3.4;
  constexpr auto   s2            = s1 * factor;
  constexpr double expectedValue = factor * 3.2;
  EXPECT_EQ(s2.value(), expectedValue);
}

TEST(Unit, MultiplicationWithScalarFreeOperatorScalarFirst)
{
  constexpr auto   s1            = 3.2_s;
  constexpr double factor        = 3.4;
  constexpr auto   s2            = factor * s1;
  constexpr double expectedValue = factor * 3.2;
  EXPECT_EQ(s2.value(), expectedValue);
}

TEST(Unit, DivisionByScalarFreeOperator)
{
  constexpr auto   s1            = 3.2_s;
  constexpr double factor        = 3.4;
  constexpr auto   s2            = s1 / factor;
  constexpr double expectedValue = 3.2 / factor;
  EXPECT_EQ(s2.value(), expectedValue);
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

TEST(Unit, InvertMilliSecondTestType)
{
  auto freq = 1.0 / 2.0_ms;
  EXPECT_TRUE((std::is_same_v<decltype(freq), kilohertz>));
}


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif   // UNITTEST_H_19645270133266128954221751440023422238912390
