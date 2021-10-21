/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringJoinerTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    10.09.2021 
 */ 
 
#ifndef STRINGJOINERTEST_H_315741838020296184792806021084216842786229731 
#define STRINGJOINERTEST_H_315741838020296184792806021084216842786229731 
 
 
// includes
#include <Meta/StringJoiner.h>

 
using namespace std::string_view_literals;
 
struct Hello {
  static constexpr auto value = L"Hello"sv;
};

struct Empty {
  static constexpr auto value = L" "sv;
};

struct World {
  static constexpr auto value = L"World!"sv;
};

static constexpr auto str1 = "This is"sv;
static constexpr auto str2 = " a "sv;
static constexpr auto str3 = "test"sv;
static constexpr auto str4 = "!"sv;


TEST(StringJoiner, JoinThreeWStrings)
{
  constexpr auto expected = L"Hello World!"sv;
  constexpr auto joinedString = meta::JoinedString<Hello::value, Empty::value, World::value>;
  constexpr auto size = joinedString.size();
  EXPECT_EQ(joinedString, expected);
  EXPECT_EQ(size, 12ULL);
}

TEST(StringJoiner, JoinFourStrings)
{
  constexpr auto expected = "This is a test!"sv;
  constexpr auto joinedString = meta::JoinedString<str1, str2, str3, str4>;
  constexpr auto size = joinedString.size();
  EXPECT_EQ(joinedString, expected);
  EXPECT_EQ(size, 15ULL);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGJOINERTEST_H_315741838020296184792806021084216842786229731 
