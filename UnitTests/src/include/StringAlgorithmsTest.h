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

#include <cstring>
#include <cwchar>

using namespace std::string_literals;
using namespace std::string_view_literals;


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

TEST(StringAlgorithms, splitViewEmptyString)
{
  std::wstring_view s;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring_view> expected;
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitNoDelimiter)
{
  const auto s = L"StringWithoutDelimiter"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithoutDelimiter"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewNoDelimiter)
{
  const auto s = L"StringWithoutDelimiter"sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L' ' == c;});
  const std::vector expected{L"StringWithoutDelimiter"sv};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitOnlyDelimiter)
{
  const auto s = L"++++++"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L'+' == c;});
  const std::vector<std::wstring> expected;
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewOnlyDelimiter)
{
  const auto s = L"++++++"sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L'+' == c;});
  const std::vector<std::wstring_view> expected;
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitDelimiterAtBegin)
{
  const auto s = L" StringWithoutDelimiter"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithoutDelimiter"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewDelimiterAtBegin)
{
  const auto s = " StringWithoutDelimiter"sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return ' ' == c;});
  const std::vector expected{"StringWithoutDelimiter"sv};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitDelimiterAtEnd)
{
  const auto s = L"StringWithoutDelimiter "s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithoutDelimiter"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewDelimiterAtEnd)
{
  const auto s = L"StringWithoutDelimiter "sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L' ' == c;});
  const std::vector expected{L"StringWithoutDelimiter"sv};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitDelimiterAtBeginAndEnd)
{
  const auto s = L"     StringWithManyDelimiters   "s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWithManyDelimiters"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewDelimiterAtBeginAndEnd)
{
  const auto s = L"     StringWithManyDelimiters   "sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L' ' == c;});
  const std::vector expected{L"StringWithManyDelimiters"sv};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitAdjacentDelimiter)
{
  const auto s = L"StringWith  TwoDelimiters"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c;});
  const std::vector<std::wstring> expected{L"StringWith"s, L"TwoDelimiters"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewAdjacentDelimiter)
{
  const auto s = L"StringWith  TwoDelimiters"sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L' ' == c;});
  const std::vector expected{L"StringWith"sv, L"TwoDelimiters"sv};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, split)
{
  const auto s = L"   split;this  string with lots     of;;;;text  into substrings   "s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return L' ' == c || L';' == c;});
  const std::vector<std::wstring> expected{L"split"s, L"this"s, L"string"s, L"with"s, L"lots"s, L"of"s, L"text"s, L"into"s, L"substrings"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitView)
{
  const auto s = L"   split;this  string with lots     of;;;;text  into substrings   "sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L' ' == c || L';' == c;});
  const std::vector expected{L"split"sv, L"this"sv, L"string"sv, L"with"sv, L"lots"sv, L"of"sv, L"text"sv, L"into"sv, L"substrings"sv};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitCommaSeparatedList)
{
  const auto s = "B,C,M,Y,X,Z"s;
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return ',' == c;});
  const std::vector<std::string> expected{"B"s, "C"s, "M"s, "Y"s, "X"s, "Z"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitCommaSeparatedListTakingCharPointer)
{
  const char* s = "B,C,M,Y,X,Z";
  const auto tokens = stringAlgorithms::split(s, [](auto c){ return ',' == c;});
  const std::vector expected{"B"s, "C"s, "M"s, "Y"s, "X"s, "Z"s};
  EXPECT_EQ(tokens, expected);
}

TEST(StringAlgorithms, splitViewCommaSeparatedList)
{
  const auto s = L"B,C,M,Y,X,Z"sv;
  const auto tokens = stringAlgorithms::splitView(s, [](auto c){ return L',' == c;});
  const std::vector expected{L"B"sv, L"C"sv, L"M"sv, L"Y"sv, L"X"sv, L"Z"sv};
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

TEST(StringAlgorithms, stringToMBstring)
{
  const auto s = "Hallo"s;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), s.c_str());

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyStringToMBstring)
{
  const auto s = ""s;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), s.c_str());

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, stringViewToMBstring)
{
  const auto s = "Hallo"sv;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "Hallo");

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyStringViewToMBstring)
{
  const auto s = ""sv;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "");

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, wstringToMBstring)
{
  const auto s = L"Hallo"s;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "Hallo");

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyWStringToMBstring)
{
  const auto s = L""s;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "");

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, wstringViewToMBstring)
{
  const auto s = L"Hallo"sv;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "Hallo");

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyWStringViewToMBstring)
{
  const auto s = L""sv;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<char[]> mbstr(stringAlgorithms::toMBstring(s));
  EXPECT_STREQ(mbstr.get(), "");

  const auto len = std::strlen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, stringToWCstring)
{
  const auto s = "Hallo"s;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"Hallo");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyStringToWCstring)
{
  const auto s = ""s;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, stringViewToWCstring)
{
  const auto s = "Hallo"sv;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"Hallo");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyStringViewToWCstring)
{
  const auto s = ""sv;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, wstringToWCstring)
{
  const auto s = L"Hallo"s;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"Hallo");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyWStringToWCstring)
{
  const auto s = L""s;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, wstringViewToWCstring)
{
  const auto s = L"Hallo"sv;
  const std::size_t expectedStrSize = 5ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"Hallo");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
}

TEST(StringAlgorithms, emptyWStringViewToWCstring)
{
  const auto s = L""sv;
  const std::size_t expectedStrSize = 0ULL;
  std::unique_ptr<wchar_t[]> mbstr(stringAlgorithms::toWCstring(s));
  EXPECT_STREQ(mbstr.get(), L"");

  const auto len = std::wcslen(mbstr.get());
  EXPECT_EQ(len, expectedStrSize);
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

TEST(StringAlgorithms, wstringViewToString)
{
  const auto s2 = stringAlgorithms::wstringToString(L"asgctgkztikzniuzhucjczhjukujhniukgiuiukgftzghtzhiugtgiz");
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

TEST(StringAlgorithms, stringViewToWstring)
{
  const auto s2 = stringAlgorithms::stringToWstring("asgctgkztikzniuzhucjczhjukujhniukgiuiukgftzghtzhiugtgiz");
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

TEST(StringAlgorithms, enclosedStringOneDelimiter)
{
  const auto s1 = "%NumWorkers%"s;
  const auto expected = "NumWorkers"s;
  const auto subStr = stringAlgorithms::enclosedString(s1, [](const char c){ return c == '%';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, enclosedStringViewOneDelimiter)
{
  const auto s1 = "%NumWorkers%"sv;
  const auto expected = "NumWorkers"sv;
  const auto subStr = stringAlgorithms::enclosedStringView(s1, [](const char c){ return c == '%';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, enclosedStringTwoDelimiters)
{
  const auto s1 = "XpathComponent[46]"s;
  const auto expected = "46"s;
  const auto subStr = stringAlgorithms::enclosedString(s1, [](const char c){ return c == '[' || c == ']';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, enclosedStringViewTwoDelimiters)
{
  const auto s1 = L"XpathComponent[46]"sv;
  const auto expected = L"46"sv;
  const auto subStr = stringAlgorithms::enclosedString(s1, [](const wchar_t c){ return c == L'[' || c == L']';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromLeftConstLValue)
{
  const auto s1 = "---++NumWorkers__"s;
  const auto expected = "NumWorkers__"s;
  const auto subStr = stringAlgorithms::cropLeft(s1, [](const char c){ return c == '-' || c == '+';});
  EXPECT_EQ(s1, "---++NumWorkers__"s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromLeftNonConstLValue)
{
  auto s1 = "---++NumWorkers__"s;
  const auto expected = "NumWorkers__"s;
  const auto subStr = stringAlgorithms::cropLeft(s1, [](const char c){ return c == '-' || c == '+';});
  EXPECT_EQ(s1, expected);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromLeftNonConstLValueUseWithoutReturnValue)
{
  auto s1 = "---++NumWorkers__"s;
  const auto expected = "NumWorkers__"s;
  stringAlgorithms::cropLeft(s1, [](const char c){ return c == '-' || c == '+';});
  EXPECT_EQ(s1, expected);
}

TEST(StringAlgorithms, removeFromLeftRValue)
{
  auto s1 = "---++NumWorkers__"s;
  const auto expected = "NumWorkers__"s;
  const auto subStr = stringAlgorithms::cropLeft(std::move(s1), [](const char c){ return c == '-' || c == '+';});
  EXPECT_EQ(s1, ""s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromLeftPureRValue)
{
  auto getString = []() { return "---++NumWorkers__"s;};

  const auto expected = "NumWorkers__"s;
  const auto subStr = stringAlgorithms::cropLeft(getString(), [](const char c){ return c == '-' || c == '+';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromRightConstLValue)
{
  const auto s1 = "---++NumWorkers__##"s;
  const auto expected = "---++NumWorkers"s;
  const auto subStr = stringAlgorithms::cropRight(s1, [](const char c){ return c == '_' || c == '#';});
  EXPECT_EQ(s1, "---++NumWorkers__##"s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromRightNonConstLValue)
{
  auto s1 = "---++NumWorkers__##"s;
  const auto expected = "---++NumWorkers"s;
  const auto subStr = stringAlgorithms::cropRight(s1, [](const char c){ return c == '_' || c == '#';});
  EXPECT_EQ(s1, expected);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromRightNonConstLValueUseWithoutReturnValue)
{
  auto s1 = "---++NumWorkers__##"s;
  const auto expected = "---++NumWorkers"s;
  stringAlgorithms::cropRight(s1, [](const char c){ return c == '_' || c == '#';});
  EXPECT_EQ(s1, expected);
}

TEST(StringAlgorithms, removeFromRightRValue)
{
  auto s1 = "---++NumWorkers__##"s;
  const auto expected = "---++NumWorkers"s;
  const auto subStr = stringAlgorithms::cropRight(std::move(s1), [](const char c){ return c == '_' || c == '#';});
  EXPECT_EQ(s1, ""s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, removeFromRightPureRValue)
{
  auto getString = []() { return "---++NumWorkers__##"s;};

  const auto expected = "---++NumWorkers"s;
  const auto subStr = stringAlgorithms::cropRight(getString(), [](const char c){ return c == '_' || c == '#';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, cropBothSidesConstLValue)
{
  const auto s1 = "---++NumWorkers__##"s;
  const auto expected = "NumWorkers"s;
  const auto subStr = stringAlgorithms::cropBothSides(s1, [](const char c){ return c == '_' || c == '#' || c == '-' || c == '+';});
  EXPECT_EQ(s1, "---++NumWorkers__##"s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, cropBothSidesNonConstLValue)
{
  auto s1 = "---++NumWorkers__##"s;
  const auto expected = "NumWorkers"s;
  const auto subStr = stringAlgorithms::cropBothSides(s1, [](const char c){ return c == '_' || c == '#' || c == '-' || c == '+';});
  EXPECT_EQ(s1, expected);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, cropBothSidesNonConstRValue)
{
  auto s1 = "---++NumWorkers__##"s;
  const auto expected = "NumWorkers"s;
  const auto subStr = stringAlgorithms::cropBothSides(std::move(s1), [](const char c){ return c == '_' || c == '#' || c == '-' || c == '+';});
  EXPECT_EQ(s1, ""s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, cropBothSidesPureRValue)
{
  auto getString = []() { return "---++NumWorkers__##"s; };
  const auto expected = "NumWorkers"s;
  const auto subStr = stringAlgorithms::cropBothSides(getString(), [](const char c){ return c == '_' || c == '#' || c == '-' || c == '+';});
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padLeftConstLValue)
{
  const auto s1 = "thisIsAString"s;
  const auto expected = "       thisIsAString"s;
  const auto subStr = stringAlgorithms::padLeft(s1, ' ', 20);
  EXPECT_EQ(s1, "thisIsAString"s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padLeftNonConstLValueUseWithoutReturnValue)
{
  auto s1 = "thisIsAString"s;
  const auto expected = "       thisIsAString"s;
  stringAlgorithms::padLeft(s1, ' ', 20);
  EXPECT_EQ(s1, expected);
}

TEST(StringAlgorithms, padLeftRValue)
{
  auto s1 = "thisIsAString"s;
  const auto expected = "       thisIsAString"s;
  const auto subStr = stringAlgorithms::padLeft(std::move(s1), ' ', 20);
  EXPECT_EQ(s1, ""s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padLeftPureRValue)
{
  auto getString = []() { return "thisIsAString"s;};
  const auto expected = "       thisIsAString"s;
  const auto subStr = stringAlgorithms::padLeft(getString(), ' ', 20);
  EXPECT_EQ(subStr, expected);
}





TEST(StringAlgorithms, padRightConstLValue)
{
  const auto s1 = "thisIsAString"s;
  const auto expected = "thisIsAString       "s;
  const auto subStr = stringAlgorithms::padRight(s1, ' ', 20);
  EXPECT_EQ(s1, "thisIsAString"s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padRightNonConstLValueUseWithoutReturnValue)
{
  auto s1 = "thisIsAString"s;
  const auto expected = "thisIsAString       "s;
  stringAlgorithms::padRight(s1, ' ', 20);
  EXPECT_EQ(s1, expected);
}

TEST(StringAlgorithms, padRightRValue)
{
  auto s1 = "thisIsAString"s;
  const auto expected = "thisIsAString       "s;
  const auto subStr = stringAlgorithms::padRight(std::move(s1), ' ', 20);
  EXPECT_EQ(s1, ""s);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padRightPureRValue)
{
  auto getString = []() { return "thisIsAString"s;};
  const auto expected = "thisIsAString       "s;
  const auto subStr = stringAlgorithms::padRight(getString(), ' ', 20);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padBothSidesPureRValueEmptyString)
{
  auto getString = []() { return ""s;};
  const auto expected = "------------------"s;
  const auto subStr = stringAlgorithms::padBothSides(getString(), '-', 18);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padBothSidesPureRValueStringTooLong)
{
  auto getString = []() { return "++++++++++++"s;};
  const auto expected = "++++++++++++"s;
  const auto subStr = stringAlgorithms::padBothSides(getString(), '-', 8);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padBothSidesPureRValue)
{
  auto getString = []() { return "String"s;};
  const auto expected = "------String------"s;
  const auto subStr = stringAlgorithms::padBothSides(getString(), '-', 18);
  EXPECT_EQ(subStr, expected);
}

TEST(StringAlgorithms, padBothSidesPureRValuePadRightMore)
{
  auto getString = []() { return "String1"s;};
  const auto expected = "-----String1------"s;
  const auto subStr = stringAlgorithms::padBothSides(getString(), '-', 18);
  EXPECT_EQ(subStr, expected);
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGALGORITHMSTEST_H_403526704178051903712996286031604270221873 
