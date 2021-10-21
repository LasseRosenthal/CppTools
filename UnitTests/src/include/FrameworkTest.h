/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FrameworkTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    10.02.2021 
 */ 
 
#ifndef FRAMEWORKTEST_H_325684923109141345318162367523415282947428 
#define FRAMEWORKTEST_H_325684923109141345318162367523415282947428 
 
 
// includes
#include "TestFrameworks.h"


TEST(Framework, isInitalized)
{
  bool b = false;
  std::vector<std::string> msgs;

  const auto testFramework = cpptools::Framework::create<cpptools::TestFramework>(b, msgs);
  EXPECT_TRUE(testFramework->isInitialized());
}


TEST(Framework, destructorIsCalled)
{
  bool b = false;
  std::vector<std::string> msgs;
  {
    const auto testFramework = cpptools::Framework::create<cpptools::TestFramework>(b, msgs);
  }
  EXPECT_TRUE(b);
}

TEST(Framework, failingConstructor)
{
  bool b = false;
  std::vector<std::string> msgs;
  const auto testFramework = cpptools::Framework::create<cpptools::TestFramework>(b, msgs, true);
  EXPECT_FALSE(testFramework->isInitialized());
}


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FRAMEWORKTEST_H_325684923109141345318162367523415282947428 
