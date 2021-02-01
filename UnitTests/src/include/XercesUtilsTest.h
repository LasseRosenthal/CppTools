/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    XercesUtilsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.01.2021 
 */ 
 
#ifndef XERCESUTILSTEST_H_3000518017122002883215805212592230343766657 
#define XERCESUTILSTEST_H_3000518017122002883215805212592230343766657 
 
 
// includes
#include <XercesUtils/XercesInitializer.h>
#include <XercesUtils/XercesString.h>

#include <sstream>
 

using namespace std::string_literals;
using namespace xcu::literals;

static xcu::XercesInitializer xercesInitializer;
 

TEST(XercesUtils, wcharEnabled)
{
  EXPECT_TRUE(xcu::wcharEnabled);
}

TEST(XercesString, emptyExpectTrue)
{
  xcu::XercesString s;
  EXPECT_TRUE(s.empty());
}

TEST(XercesString, emptyExpectFalse)
{
  xcu::XercesString s{"thisIsMyString"s};
  EXPECT_FALSE(s.empty());
}

TEST(XercesString, sizeNonEmpty)
{
  xcu::XercesString s{"thisIsMyString"s};
  constexpr std::size_t expectedSize = 14ULL;
  EXPECT_EQ(s.size(), expectedSize);
}

TEST(XercesString, constructFromConstChar)
{
  xcu::XercesString s{"thisIsMyString"};
  constexpr std::size_t expectedSize = 14ULL;
  EXPECT_EQ(s.size(), expectedSize);
}

TEST(XercesString, toStringNonEmpty)
{
  const std::string s1{"thisIsMyString"};
  xcu::XercesString xs{s1};
  EXPECT_EQ(xs.toString<std::string>(), s1);
}

TEST(XercesString, toStringEmptyValid)
{
  const std::string s1;
  xcu::XercesString xs{s1};
  EXPECT_EQ(xs.toString<std::string>(), s1);
}

TEST(XercesString, toStringEmptyNotValid)
{
  xcu::XercesString xs;
  EXPECT_EQ(xs.toString<std::string>(), ""s);
}

TEST(XercesString, toWStringNonEmpty)
{
  const std::wstring s1{L"thisIsMyString"};
  xcu::XercesString xs{s1};
  EXPECT_EQ(xs.toString<std::wstring>(), s1);
}

TEST(XercesString, toWStringEmptyValid)
{
  const std::wstring s1;
  xcu::XercesString xs{s1};
  EXPECT_EQ(xs.toString<std::wstring>(), s1);
}

TEST(XercesString, toWStringEmptyNotValid)
{
  xcu::XercesString xs;
  EXPECT_EQ(xs.toString<std::wstring>(), L""s);
}

TEST(XercesString, streamIntoStringSream)
{
  const std::string s1{"thisIsMyString"};
  xcu::XercesString xs{s1};
  std::stringstream str;
  str << xs;
  EXPECT_EQ(str.str(), s1);
}

TEST(XercesString, append)
{
  const std::string s1{"thisIsMyString"};
  xcu::XercesString xs1{"thisIs"s};
  xcu::XercesString xs2{"MyString"s};

  xs1.append(xs2);

  std::stringstream str;
  str << xs1;
  EXPECT_EQ(str.str(), s1);
}

TEST(XercesString, concatenate)
{
  using namespace xcu;
  using namespace xcu::literals;

  const std::string s1{"thisIsMyString"};
  xcu::XercesString xs1{"thisIs"s};
  xcu::XercesString xs2{"MyString"s};

  const auto xs3 = "thisIs"_xs + xs2;

  std::stringstream str;
  str << xs3;
  EXPECT_EQ(str.str(), s1);
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // XERCESUTILSTEST_H_3000518017122002883215805212592230343766657 
