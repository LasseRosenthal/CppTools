/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringUtilsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.11.2020 
 */ 
 
#ifndef STRINGUTILSTEST_H_13800171819432574426032132733182456383016 
#define STRINGUTILSTEST_H_13800171819432574426032132733182456383016 
 
 
// includes
#include <Utils/StringUtils.h>

using namespace std::string_literals;

TEST(StringUtils, splitEmpty)
{
  const auto s = L""s;
  const auto tokens = stringUtils::split5(s, L' ');
  const std::vector<std::wstring> expected{L""s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringUtils, splitOneEntry)
{
  const auto s = L"Hello"s;
  const auto tokens = stringUtils::split5(s, L' ');
  const std::vector<std::wstring> expected{L"Hello"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringUtils, splitTwoEntries)
{
  const auto s = L"Hello, Yo"s;
  const auto tokens = stringUtils::split5(s, L',');
  const std::vector<std::wstring> expected{L"Hello"s, L" Yo"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringUtils, split)
{
  const auto s = L"split this string with lots of text into substrings"s;
  const auto tokens = stringUtils::split3(s, L' ');
  const std::vector<std::wstring> expected{L"split"s, L"this"s, L"string"s, L"with"s, L"lots"s, L"of"s, L"text"s, L"into"s, L"substrings"s};
  EXPECT_EQ(tokens, expected);
}
 
 
/** 
 * @class 
 * @brief 
 */ 
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGUTILSTEST_H_13800171819432574426032132733182456383016 
