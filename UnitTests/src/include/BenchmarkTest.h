/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BenchmarkTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    18.08.2020 
 */ 
 
#ifndef BENCHMARKTEST_H_358310025165681174212319571680418102274 
#define BENCHMARKTEST_H_358310025165681174212319571680418102274 
 
 
// includes 
#include <Benchmark/StopWatch.h>

#include <string>
#include <vector>

using namespace std::string_literals;
 

TEST(Benchmark, UnitRepresentationWeeksWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::weeks, wchar_t>::value), L"week"s);
}

TEST(Benchmark, unitStringMethodWeeksWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::weeks::zero())), L"week"s);
}

TEST(Benchmark, UnitRepresentationDaysWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::days, wchar_t>::value), L"d"s);
}

TEST(Benchmark, unitStringMethodDaysWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::days::zero())), L"d"s);
}

TEST(Benchmark, UnitRepresentationHoursWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::hours, wchar_t>::value), L"h"s);
}

TEST(Benchmark, unitStringMethodHoursWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::hours::zero())), L"h"s);
}

TEST(Benchmark, UnitRepresentationMinutesWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::minutes, wchar_t>::value), L"min"s);
}

TEST(Benchmark, unitStringMethodMinutesWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::minutes::zero())), L"min"s);
}

TEST(Benchmark, UnitRepresentationSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::seconds, wchar_t>::value), L"s"s);
}

TEST(Benchmark, unitStringMethodSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::seconds::zero())), L"s"s);
}

TEST(Benchmark, UnitRepresentationMilliSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::milliSeconds, wchar_t>::value), L"ms"s);
}

TEST(Benchmark, unitStringMethodMilliSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::milliSeconds::zero())), L"ms"s);
}

TEST(Benchmark, UnitRepresentationMicroSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::microSeconds, wchar_t>::value), L"mus"s);
}

TEST(Benchmark, unitStringMethodMicroSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::microSeconds::zero())), L"mus"s);
}

TEST(Benchmark, UnitRepresentationNanoSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::nanoSeconds, wchar_t>::value), L"ns"s);
}

TEST(Benchmark, unitStringMethodNanoSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::nanoSeconds::zero())), L"ns"s);
}

TEST(Benchmark, UnitRepresentationPicoSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::picoSeconds, wchar_t>::value), L"ps"s);
}

TEST(Benchmark, unitStringMethodPicoSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::picoSeconds::zero())), L"ps"s);
}

TEST(Benchmark, UnitRepresentationFemtoSecondsChar)
{
  EXPECT_EQ(std::string(benchmark::DurationString<benchmark::femtoSeconds, char>::value), "fs"s);
}

TEST(Benchmark, unitStringMethodFemtoSecondsChar)
{
  EXPECT_EQ(std::string(benchmark::unitString<benchmark::femtoSeconds, char>(benchmark::femtoSeconds::zero())), "fs"s);
}
 
TEST(Benchmark, UnitRepresentationFemtoSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::DurationString<benchmark::femtoSeconds, wchar_t>::value), L"fs"s);
}

TEST(Benchmark, unitStringMethodFemtoSecondsWchar)
{
  EXPECT_EQ(std::wstring(benchmark::unitString(benchmark::femtoSeconds::zero())), L"fs"s);
}

TEST(Benchmark, WinHighResClockMeasurement)
{
  benchmark::WinHighResClock cl;
  const auto tp1 = cl.now();

  std::vector<double> v;
  for (int i = 0; i < 123456; ++i)
  {
    v.push_back(i*12.3);
  }
  const auto tp2 = cl.now();
  const auto dur = std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1);

  EXPECT_GT(dur.count(), 0);
}

TEST(Benchmark, StopWatchStateReset)
{
  benchmark::StopWatch<> watch;
  EXPECT_EQ(watch.getState(), benchmark::StopWatch<>::state::reset);
}

TEST(Benchmark, StopWatchStateRunning)
{
  benchmark::StopWatch<> watch;
  watch.start();
  EXPECT_EQ(watch.getState(), benchmark::StopWatch<>::state::running);
}

TEST(Benchmark, StopWatchStateStopped)
{
  benchmark::StopWatch<> watch;
  watch.start();
  watch.stop();
  EXPECT_EQ(watch.getState(), benchmark::StopWatch<>::state::stopped);
}

TEST(Benchmark, StopWatchStateResetAfterStart)
{
  benchmark::StopWatch<> watch;
  watch.start();
  watch.stop();
  watch.reset();
  EXPECT_EQ(watch.getState(), benchmark::StopWatch<>::state::reset);
}

TEST(Benchmark, StopWatchLoggingMessageDefault)
{
  benchmark::StopWatch<> watch;
  EXPECT_STREQ(watch.loggingMessage(), L"StopWatch");
}

TEST(Benchmark, StopWatchLoggingMessageCustom)
{
  benchmark::StopWatch<> watch(std::wcout, L"StopWatchLoggingMessageCustom");
  EXPECT_STREQ(watch.loggingMessage(), L"StopWatchLoggingMessageCustom");
}

TEST(Benchmark, StopWatchLoggingIntoStringStream)
{
  std::wstringstream logStrm;
  benchmark::StopWatch<std::chrono::high_resolution_clock> watch(logStrm, L"LogIntoStringStream");
  watch.log<benchmark::milliSeconds>(4);
  EXPECT_EQ(std::wstring(L"LogIntoStringStream : 0.0000ms\n"), logStrm.str());
}

TEST(Benchmark, StopWatchMeasurement)
{
  benchmark::StopWatch<> watch;
  watch.start();

  std::vector<double> v;
  for (int i = 0; i < 123456; ++i)
  {
    v.push_back(i*12.3);
  }
  watch.stop();
  const auto dur = watch.getDuration();

  EXPECT_GT(dur.count(), 0);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BENCHMARKTEST_H_358310025165681174212319571680418102274 
