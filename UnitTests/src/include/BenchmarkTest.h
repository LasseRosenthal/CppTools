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
#include <Benchmark/CPUTimer.h>

#include <string>
#include <vector>

using namespace std::string_literals;
 

TEST(CPUTimer, DefaultConstructor)
{
  EXPECT_NO_THROW(
    benchmark::CPUTimer cpuTimer;
  );
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
  EXPECT_EQ(watch.loggingMessage(), L""s);
}

TEST(Benchmark, StopWatchLoggingMessageCustom)
{
  benchmark::StopWatch<> watch(std::wcout, L"StopWatchLoggingMessageCustom"s);
  EXPECT_EQ(watch.loggingMessage(), L"StopWatchLoggingMessageCustom"s);
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
