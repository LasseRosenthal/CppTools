/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FrameworkStackTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    16.09.2021 
 */ 
 
#ifndef FRAMEWORKSTACKTEST_H_256261925955929137102442781322630739523070 
#define FRAMEWORKSTACKTEST_H_256261925955929137102442781322630739523070 
 
 
// includes
#include <Utils/FrameworkStack.h>

#include "TestFrameworks.h"
 
 
TEST(FrameworkStack, DestructionOrder)
{
  bool b = false;
  std::vector<std::string> msgs;

  {
    cpptools::FrameworkStack myFrameworks;
    myFrameworks.push<cpptools::TestFramework>(b, msgs);
    myFrameworks.push<cpptools::TestFramework2>(b, msgs);
    EXPECT_TRUE(myFrameworks.good());
  }

  std::vector<std::string> expected{"TestFramework1 initialized", "TestFramework2 initialized",
                                    "TestFramework2 terminated", "TestFramework1 terminated"};

  EXPECT_EQ(msgs, expected);
}

TEST(FrameworkStack, FailingInitialization)
{
  bool b = false;
  std::vector<std::string> msgs;

  cpptools::FrameworkStack myFrameworks;
  const bool               success = myFrameworks.push<cpptools::TestFramework>(b, msgs, true);

  EXPECT_FALSE(myFrameworks.good());
  EXPECT_FALSE(success);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FRAMEWORKSTACKTEST_H_256261925955929137102442781322630739523070 
