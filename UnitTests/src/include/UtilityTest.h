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
#include <vector>
 
 
using namespace meta;


/**
 * @brief  tests of IsPowerOfTwo.
 */
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

TEST(Utility, TypeOf)
{
  EXPECT_TRUE((std::is_same_v<meta::TypeOf<1ULL, short, double const, char>, double const>));
}

/**
 * @brief  tests of IsOneOf.
 */
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

struct Dummy {
  auto funNonConst(std::string, double const&) -> int;
  void funConst(std::vector<short>, double const, int) const;
};

TEST(Utility, FunctionTraitsNonConstMemberFunction)
{
  using funTraits = meta::FunctionTraits<decltype(&Dummy::funNonConst)>;

  EXPECT_EQ(funTraits::arity, 2ULL);
  EXPECT_TRUE((std::is_same_v<funTraits::result_type, int>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<0ULL>, std::string>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<1ULL>, double const&>));
  EXPECT_FALSE(funTraits::is_const_method);
}

TEST(Utility, FunctionTraitsConstMemberFunction)
{
  using funTraits = meta::FunctionTraits<decltype(&Dummy::funConst)>;

  EXPECT_EQ(funTraits::arity, 3ULL);
  EXPECT_TRUE((std::is_same_v<funTraits::result_type, void>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<0ULL>, std::vector<short>>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<1ULL>, double>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<2ULL>, int>));
  EXPECT_TRUE(funTraits::is_const_method);
}

TEST(Utility, FunctionTraitsLambda)
{
  auto myFun      = [](std::string const&, int&) -> bool { return true; };
  using funTraits = meta::FunctionTraits<decltype(myFun)>;

  EXPECT_EQ(funTraits::arity, 2ULL);
  EXPECT_TRUE((std::is_same_v<funTraits::result_type, bool>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<0ULL>, std::string const&>));
  EXPECT_TRUE((std::is_same_v<funTraits::argument_type<1ULL>, int&>));
  EXPECT_TRUE(funTraits::is_const_method);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UTILITYTEST_H_13222257352987832546118517611551297452281 
