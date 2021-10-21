/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BenchmarkUtilityTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.06.2021 
 */ 
 
#ifndef BENCHMARKUTILITYTEST_H_6152116425830578131744174762588105132059 
#define BENCHMARKUTILITYTEST_H_6152116425830578131744174762588105132059 
 
 
// includes
#include <Benchmark/Utility.h>

#include <DebugTraceFramework/DebugStream.h>
 
using namespace std::chrono_literals;
using namespace std::string_literals;
 
TEST(BenchmarksUtility, sliceDurationIntegralVersionHours)
{
  using namespace benchmark::literals;

  auto sec = 7340s;
  const auto hours = benchmark::sliceDuration<std::chrono::hours>(sec);

  EXPECT_EQ(sec, 140s);
  EXPECT_EQ(hours, 2h);
}

TEST(BenchmarksUtility, sliceDurationIntegralVersionMinutes)
{
  using namespace benchmark::literals;

  auto sec = 7340s;
  const auto minutes = benchmark::sliceDuration<std::chrono::minutes>(sec);

  EXPECT_EQ(sec, 20s);
  EXPECT_EQ(minutes, 122min);
}

TEST(BenchmarksUtility, sliceDurationFloatingPointVersionHours)
{
  using namespace benchmark::literals;

  auto sec = 7340.5_s;
  const auto hours = benchmark::sliceDuration<benchmark::hours>(sec);

  EXPECT_EQ(sec, 140.5_s);
  EXPECT_EQ(hours, 2_h);
}

TEST(BenchmarksUtility, DecomposeDurationIntegralVersion)
{
  using namespace benchmark::literals;

  auto musec = 7'340'082ms;

  const auto [hours, minutes, second] = benchmark::decomposeDuration<std::chrono::hours, std::chrono::minutes, std::chrono::seconds>(musec);

  EXPECT_EQ(hours, 2h);
  EXPECT_EQ(minutes, 2min);
  EXPECT_EQ(second, 20s);
  EXPECT_EQ(musec, 82ms);
}

TEST(BenchmarksUtility, DecomposeDurationIntegralVersion2)
{
  using namespace benchmark::literals;

  auto musec = 7'340'082ms;

  const auto [hours, second] = benchmark::decomposeDuration<std::chrono::hours, std::chrono::seconds>(musec);

  EXPECT_EQ(hours, 2h);
  EXPECT_EQ(second, 140s);
  EXPECT_EQ(musec, 82ms);
}

TEST(BenchmarksUtility, convertToTimeString)
{
  using namespace benchmark::literals;
  using clock = std::chrono::system_clock;

  dbgf::WDebugStream s{__FUNCTION__, __FILE__};

  const auto timePoint = clock::now();
  std::time_t ttCreation = 1'623'068'375LL; // clock::to_time_t(timePoint);
  std::tm          timeStruc;

  localtime_s(&timeStruc, &ttCreation);

  const auto timeString = benchmark::convertToTimeString<wchar_t>(timeStruc);

  EXPECT_EQ(timeString, L"14:19:35"s);

  s << timeString << std::endl;
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BENCHMARKUTILITYTEST_H_6152116425830578131744174762588105132059 
