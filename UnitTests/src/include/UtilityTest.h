/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    UtilityTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    07.07.2020 
 */ 
 
#ifndef UTILITYTEST_H_13222257352987832546118517611551297452281 
#define UTILITYTEST_H_13222257352987832546118517611551297452281 
 
 
// includes
#include "Meta/Utility.h"

#include <string>
 
 
using namespace meta;


/**
 * @brief  tests of IsPowerOfTwo.
 */
TEST(Utility, IsPowerOfTwo)
{
  EXPECT_TRUE((meta::IsPowerOfTwo<1ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<2ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<4ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<8ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<16ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<32ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<64ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<128ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<256ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<512ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<1024ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<2048ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<4096ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<8192ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<16384ULL>));
  EXPECT_TRUE((meta::IsPowerOfTwo<32768ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<1ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<4ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<8ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<16ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<32ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<64ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<128ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<256ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<512ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<1024ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<2048ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<4096ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<8192ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<16384ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<32768ULL - 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<2ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<4ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<8ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<16ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<32ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<64ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<128ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<256ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<512ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<1024ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<2048ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<4096ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<8192ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<16384ULL + 1ULL>));
  EXPECT_FALSE((meta::IsPowerOfTwo<32768ULL + 1ULL>));
}

/**
 * @brief  tests of IsOneOf.
 */
TEST(Utility, isIntegralExpectTrue)
{
  EXPECT_TRUE((meta::isArithmetic<const double, int, float, const double, long>));
}

TEST(Utility, isIntegralExpectFalse)
{
  EXPECT_FALSE((meta::isArithmetic<const double, int, float, char, std::wstring, const double, long>));
}

TEST(Utility, allSameExpectTrue)
{
  EXPECT_TRUE((meta::allSame<const double, const double, const double, const double>));
}

TEST(Utility, allSameExpectFalse)
{
  EXPECT_FALSE((meta::allSame<const double, double, const double, const double>));
}

TEST(Utility, IsOneOfExpectTrue)
{
  EXPECT_TRUE((meta::isOneOf<const double, int, float, char, std::wstring, const double, long>));
}

TEST(Utility, IsOneOfTwoTypesExpectTrue)
{
  EXPECT_TRUE((meta::isOneOf<const double, const double>));
}

TEST(Utility, IsOneOfTwoTypesExpectFalse)
{
  EXPECT_FALSE((meta::isOneOf<const double, int>));
}

TEST(Utility, IsOneOfReferenceTypeExpectTrue)
{
  EXPECT_TRUE((meta::isOneOf<const double&, int, float, char, std::wstring, const double&, long>));
}

TEST(Utility, IsOneOfExpectFalse)
{
  EXPECT_FALSE((meta::isOneOf<const double, int, float, char, std::wstring, double, long>));
}

TEST(Utility, AllConvertibleExpectTrue)
{
  EXPECT_TRUE((meta::AllConvertible<double, int, float, char, long>));
}

TEST(Utility, AllConvertibleExpectTrueOnlyOneType)
{
  EXPECT_TRUE((meta::AllConvertible<std::string>));
}

TEST(Utility, AllConvertibleExpectFalse)
{
  EXPECT_FALSE((meta::AllConvertible<double, std::string, float, char, long>));
}



 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UTILITYTEST_H_13222257352987832546118517611551297452281 
