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
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // OSUTILITYTEST_H_3127812452190431197170043118278001483819772 
