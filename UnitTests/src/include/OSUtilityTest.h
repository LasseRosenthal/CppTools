/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    OSUtilityTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    05.05.2021 
 */ 
 
#ifndef OSUTILITYTEST_H_3127812452190431197170043118278001483819772 
#define OSUTILITYTEST_H_3127812452190431197170043118278001483819772 
 
 
// includes
#include <Utils/OSUtility.h>
 
using namespace std::string_literals;
using namespace std::string_view_literals;


TEST(OSUtility, getExistingEnvironmentVariableStringParameter)
{
  auto env = osutility::getEnvironmentVariable(L"SystemRoot"s);
  EXPECT_EQ(env, LR"(C:\WINDOWS)");
}

TEST(OSUtility, getExistingEnvironmentVariableStringViewParameter)
{
  auto env = osutility::getEnvironmentVariable(L"SystemRoot");
  EXPECT_EQ(env, LR"(C:\WINDOWS)");
}

TEST(OSUtility, getNonExistingEnvironmentVariableExpectRuntimeError)
{
  EXPECT_THROW(
    auto env = osutility::getEnvironmentVariable(L"dummy"s),
    osutility::environmentvariableNotFound
  );
}

TEST(OSUtility, existsEnvironmentVariableExpectTrue)
{
  const auto exists = osutility::existsEnvironmentVariable(L"SystemRoot"s);
  EXPECT_TRUE(exists);
}

TEST(OSUtility, existsEnvironmentVariableExpectFalse)
{
  const auto exists = osutility::existsEnvironmentVariable(L"dummy"s);
  EXPECT_FALSE(exists);
}

TEST(OSUtility, setEnvironmentVariable)
{
  const auto envName = L"dummyTestvar"s;
  const auto envContent = L"dummyContent"s;
  const auto success = osutility::setEnvironmentVariable(envName, envContent);
  EXPECT_TRUE(success);
  EXPECT_TRUE(osutility::existsEnvironmentVariable(envName));

  const auto envGetContent = osutility::getEnvironmentVariable(envName);
  EXPECT_EQ(envGetContent, envContent);
}

TEST(OSUtility, getExistingEnvironmentVariableOnlyOneCall)
{
  const auto envName = L"dummy2"s;
  const auto envContent = L"1"s;
  const auto success = osutility::setEnvironmentVariable(envName, envContent);
  EXPECT_TRUE(success);
  EXPECT_TRUE(osutility::existsEnvironmentVariable(envName));

  const auto envGetContent = osutility::getEnvironmentVariable(envName);
  EXPECT_EQ(envGetContent, envContent);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // OSUTILITYTEST_H_3127812452190431197170043118278001483819772 
