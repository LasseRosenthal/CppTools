/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringAlgorithmsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.12.2020 
 */ 
 
#ifndef STRINGALGORITHMSTEST_H_403526704178051903712996286031604270221873 
#define STRINGALGORITHMSTEST_H_403526704178051903712996286031604270221873 
 
 
// includes
#include <Utils/StringAlgorithms.h>

using namespace std::string_literals;


TEST(StringAlgorithms, lexicalCastStringToInt)
{
  auto number = stringAlgorithms::lexicalCast<int>(L"143"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), int>));
  EXPECT_EQ(number, 143);
}

TEST(StringAlgorithms, lexicalCastStringToLong)
{
  auto number = stringAlgorithms::lexicalCast<long>(L"143"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), long>));
  EXPECT_EQ(number, 143L);
}

TEST(StringAlgorithms, lexicalCastStringToUnsignedLong)
{
  auto number = stringAlgorithms::lexicalCast<unsigned long>(L"143"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), unsigned long>));
  EXPECT_EQ(number, 143UL);
}

TEST(StringAlgorithms, lexicalCastStringToLongLong)
{
  auto number = stringAlgorithms::lexicalCast<long long>(L"143"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), long long>));
  EXPECT_EQ(number, 143LL);
}

TEST(StringAlgorithms, lexicalCastStringToUnsignedLongLong)
{
  auto number = stringAlgorithms::lexicalCast<unsigned long long>(L"143"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), unsigned long long>));
  EXPECT_EQ(number, 143ULL);
}

TEST(StringAlgorithms, lexicalCastStringToFloat)
{
  auto number = stringAlgorithms::lexicalCast<float>(L"12.43"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), float>));
  EXPECT_EQ(number, 12.43F);
}

TEST(StringAlgorithms, lexicalCastStringToDouble)
{
  auto number = stringAlgorithms::lexicalCast<double>(L"12.43"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), double>));
  EXPECT_EQ(number, 12.43);
}

TEST(StringAlgorithms, lexicalCastStringToLongDouble)
{
  auto number = stringAlgorithms::lexicalCast<long double>(L"12.43"s);
  EXPECT_TRUE((std::is_same_v<decltype(number), long double>));
  EXPECT_EQ(number, 12.43L);
}

TEST(StringAlgorithms, lexicalCastStringToBoolExpectTrue)
{
  auto b = stringAlgorithms::lexicalCast<bool>("true"s);
  EXPECT_TRUE((std::is_same_v<decltype(b), bool>));
  EXPECT_TRUE(b);
}

TEST(StringAlgorithms, lexicalCastStringToBoolExpectFalse)
{
  auto b = stringAlgorithms::lexicalCast<bool>("false"s);
  EXPECT_TRUE((std::is_same_v<decltype(b), bool>));
  EXPECT_FALSE(b);
}

TEST(StringAlgorithms, lexicalCastWStringToBoolExpectTrue)
{
  auto b = stringAlgorithms::lexicalCast<bool>(L"true"s);
  EXPECT_TRUE((std::is_same_v<decltype(b), bool>));
  EXPECT_TRUE(b);
}

TEST(StringAlgorithms, lexicalCastWStringToBoolExpectFalse)
{
  auto b = stringAlgorithms::lexicalCast<bool>(L"false"s);
  EXPECT_TRUE((std::is_same_v<decltype(b), bool>));
  EXPECT_FALSE(b);
}

TEST(StringAlgorithms, lexicalCastIntToString)
{
  auto number = stringAlgorithms::lexicalCast<char>(134);
  EXPECT_TRUE((std::is_same_v<decltype(number), std::string>));
  EXPECT_EQ(number, "134"s);
}

TEST(StringAlgorithms, lexicalCastDoubleToString1)
{
  auto number = stringAlgorithms::lexicalCast<char>(13414.1347623);
  EXPECT_TRUE((std::is_same_v<decltype(number), std::string>));
  EXPECT_EQ(number, "13414.1347623"s);
}

TEST(StringAlgorithms, lexicalCastDoubleToString2)
{
  auto number = stringAlgorithms::lexicalCast<char>(13.90076);
  EXPECT_TRUE((std::is_same_v<decltype(number), std::string>));
  EXPECT_EQ(number, "13.90076"s);
}

TEST(StringAlgorithms, lexicalCastBoolToString)
{
  const auto boolString = stringAlgorithms::lexicalCast<wchar_t>(false);
  EXPECT_EQ(boolString, L"false"s);
}

TEST(StringAlgorithms, splitEmptyString)
{
  std::wstring s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected;
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitNoDelimiter)
{
  const auto s = L"StringWithoutDelimiter"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithoutDelimiter"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitOnlyDelimiter)
{
  const auto s = L"++++++"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L'+' == c;});
  const std::vector<std::wstring> expected;
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitDelimiterAtBegin)
{
  const auto s = L" StringWithoutDelimiter"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithoutDelimiter"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitDelimiterAtEnd)
{
  const auto s = L"StringWithoutDelimiter "s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithoutDelimiter"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitDelimiterAtBeginAndEnd)
{
  const auto s = L"     StringWithManyDelimiters   "s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithManyDelimiters"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitAdjacentDelimiter)
{
  const auto s = L"StringWith  TwoDelimiters"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWith"s, L"TwoDelimiters"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, split)
{
  const auto s = L"   split;this  string with lots     of;;;;text  into substrings   "s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c || L';' == c;});
  const std::vector<std::wstring> expected{L"split"s, L"this"s, L"string"s, L"with"s, L"lots"s, L"of"s, L"text"s, L"into"s, L"substrings"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, strLengthString)
{
  const auto s = "Hallo"s;
  EXPECT_EQ(stringAlgorithms::strLength(s), 5ULL);
}

TEST(StringAlgorithms, strLengthWstring)
{
  const auto s = L"Hallo"s;
  EXPECT_EQ(stringAlgorithms::strLength(s), 5ULL);
}

TEST(StringAlgorithms, cstrLengthString)
{
  const auto s = "Hallo";
  EXPECT_EQ(stringAlgorithms::cstrLength(s), 5ULL);
}

TEST(StringAlgorithms, cstrLengthWstring)
{
  const auto s = L"Hallo";
  EXPECT_EQ(stringAlgorithms::cstrLength(s), 5ULL);
}

TEST(StringAlgorithms, toMBstringString)
{
  const auto s = "Hallo"s;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), s.c_str());
}

TEST(StringAlgorithms, toMBstringWString)
{
  const auto s = L"Hallo"s;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "Hallo");
}

TEST(StringAlgorithms, wstringToString)
{
  const auto s1 = L"asgctgkztikzniuzhucjczhjukujhniukgiuiukgftzghtzhiugtgiz"s;
  const auto s2 = stringAlgorithms::wstringToString(s1);
  const auto expected = "asgctgkztikzniuzhucjczhjukujhniukgiuiukgftzghtzhiugtgiz"s;
  EXPECT_EQ(s2, expected);
}

TEST(StringAlgorithms, stringToWstring)
{
  const auto s1 = "asgctgkztikzniuzhucjczhjukujhniukgiuiukgftzghtzhiugtgiz"s;
  const auto s2 = stringAlgorithms::stringToWstring(s1);
  const auto expected = L"asgctgkztikzniuzhucjczhjukujhniukgiuiukgftzghtzhiugtgiz"s;
  EXPECT_EQ(s2, expected);
}

TEST(StringAlgorithms, beginsWithCompareWithEmptyString)
{
  const auto s1 = "TestString"s;
  const auto s2 = ""s;
  EXPECT_TRUE(stringAlgorithms::beginsWith(s1, s2));
}

TEST(StringAlgorithms, beginsWithExpectFalse)
{
  const auto s1 = "TestString"s;
  const auto s2 = " Test"s;
  EXPECT_FALSE(stringAlgorithms::beginsWith(s1, s2));
}

TEST(StringAlgorithms, beginsWithExpectTrue)
{
  const auto s1 = "TestString"s;
  const auto s2 = "TestS"s;
  EXPECT_TRUE(stringAlgorithms::beginsWith(s1, s2));
}

TEST(StringAlgorithms, beginsWithExpectTrueIdenticalStrings)
{
  const auto s1 = "TestString"s;
  const auto s2 = "TestString"s;
  EXPECT_TRUE(stringAlgorithms::beginsWith(s1, s2));
}

TEST(StringAlgorithms, endsWithCompareWithEmptyString)
{
  const auto s1 = "TestString"s;
  const auto s2 = ""s;
  EXPECT_TRUE(stringAlgorithms::endsWith(s1, s2));
}

TEST(StringAlgorithms, endsWithExpectFalse)
{
  const auto s1 = "TestString"s;
  const auto s2 = "ring "s;
  EXPECT_FALSE(stringAlgorithms::endsWith(s1, s2));
}

TEST(StringAlgorithms, endsWithExpectTrue)
{
  const auto s1 = "TestString"s;
  const auto s2 = "estString"s;
  EXPECT_TRUE(stringAlgorithms::endsWith(s1, s2));
}

TEST(StringAlgorithms, endsWithExpectTrueIdenticalStrings)
{
  const auto s1 = "TestString"s;
  const auto s2 = "TestString"s;
  EXPECT_TRUE(stringAlgorithms::endsWith(s1, s2));
}

TEST(StringAlgorithms, substringBetweenDelimiters)
{
  const auto s1 = "%NumWorkers%"s;
  const auto expected = "NumWorkers"s;
  const auto subStr = stringAlgorithms::substringBetweenDelimiters(s1, [](const char c){ return c == '%';});
  EXPECT_EQ(subStr, expected);
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGALGORITHMSTEST_H_403526704178051903712996286031604270221873 
