/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    WatchdogTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.05.2021 
 */ 
 
#ifndef WATCHDOGTEST_H_72830613170815328308142613202965235985 
#define WATCHDOGTEST_H_72830613170815328308142613202965235985 
 
 
// includes
#include <ConcurrencyTools/Watchdog.h>
 
 
TEST(Watchdog, voidEventNoException)
{
 

  EXPECT_TRUE(true);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // WATCHDOGTEST_H_72830613170815328308142613202965235985 
