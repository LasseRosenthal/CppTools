/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    DateTimeTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    07.06.2021 
 */ 
 
#ifndef DATETIMETEST_H_2817064271252527931402116052509841630889 
#define DATETIMETEST_H_2817064271252527931402116052509841630889 
 
 
// includes
#include <Benchmark/DateTime.h>
#include <Benchmark/WinHighResClock.h>

#include <iostream>

using namespace std::chrono_literals;
using namespace std::string_literals;
 
 

TEST(DateTime, getCurrentDateTime)
{
  const auto dateTime = date::getCurrentDateTime();

  std::cout << dateTime.date << std::endl;
  std::cout << dateTime.time << std::endl;

  EXPECT_TRUE(true);
}

TEST(DateTime, timePointTo)
{
  using clock = std::chrono::system_clock;
  using timePoint = clock::time_point;
  const timePoint tp{1'623'068'375'145ms};

  const auto date = date::timePointToDate<clock>(tp);
  const auto time = date::timePointToTime<clock>(tp);

  std::wstringstream wstrm;
  wstrm << date;
  const auto dateStr = wstrm.str();
  wstrm.str(L"");
  wstrm << time;
  const auto timeStr = wstrm.str();

  EXPECT_EQ(dateStr, L"2021-06-07"s);
  EXPECT_EQ(timeStr, L"14:19:35.145"s);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DATETIMETEST_H_2817064271252527931402116052509841630889 
