/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ThreadPoolTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    27.04.2021 
 */ 
 
#ifndef THREADPOOLTEST_H_929259185112118831465588811619215055156 
#define THREADPOOLTEST_H_929259185112118831465588811619215055156 
 
 
// includes
#include "Person.h"

#include <ConcurrencyTools/ThreadPool.h>

#include <mutex>
#include <thread>
#include <vector>


using namespace std::chrono_literals;


TEST(WaitableThreadPool, size)
{
  cctools::WaitableThreadPool<cctools::queuePolicy::prioritized> pool(5);
  EXPECT_EQ(pool.size(), 5);
}

TEST(NonWaitableThreadPool, size)
{
  cctools::NonWaitableThreadPool<cctools::queuePolicy::prioritized> pool(5);
  EXPECT_EQ(pool.size(), 5);
}


TEST(WaitableThreadPool, submitOneTaskCheckResult)
{
  cctools::WaitableThreadPool<cctools::queuePolicy::prioritized> pool(5);

  int  i   = 12;
  auto fut = pool.submit([i] { return i + 13; });
  const auto result = fut.get();
  EXPECT_EQ(result, 25);
}

TEST(WaitableThreadPool, submitOneTaskReturngPersonCheckResult)
{
  cctools::WaitableThreadPool<cctools::queuePolicy::prioritized> pool(5);

  int        i      = 12;
  auto       fut    = pool.submit([i] { return test::Person(45, "Jill"); });
  const auto result = fut.get();
  EXPECT_EQ(result.getAge(), 45);
  EXPECT_EQ(result.getName(), "Jill");
}

TEST(NonWaitableThreadPool, submitOneTask)
{
  cctools::NonWaitableThreadPool<cctools::queuePolicy::prioritized> pool(5);

  int i = 0;
  pool.submit([&i] {
    i = 12;
  });

  std::this_thread::sleep_for(1s);

  EXPECT_EQ(i, 12);
}

TEST(WaitableThreadPool, submitSeveralTasksCheckResult)
{
  cctools::WaitableThreadPool<cctools::queuePolicy::prioritized> pool(5);

  const int i = 12;
  const int nrTasks = 134;
  std::vector<std::future<int>> results;
  results.reserve(nrTasks);
  for(int t = 0; t < nrTasks; ++t)
  {
    results.push_back(pool.submit([i, t] { return i + t; }));
  }

  for(int t = 0; t < nrTasks; ++t)
  {
    const auto result = results[t].get();
    const int expectedResult = i + t;
    EXPECT_EQ(result, expectedResult);
  }
}

#if 0
TEST(NonWaitableThreadPool, submitSeveralTasksCheckPriority)
{
  DBG_METHOD(NonWaitableThreadPool::submitOneTask);
  cctools::WaitableThreadPool<cctools::queuePolicy::prioritized> pool(1);

  const int        numTasks = 10;
  std::vector<int> v;
  std::vector<std::future<int>> futs;
  std::mutex       mut;
  futs.reserve(numTasks);
  v.reserve(numTasks);

  constexpr auto waitingTime = 200ms;

  for(int i = 0; i < numTasks; ++i)
  {
    futs.push_back(pool.submit(
      [&mut, &v, i, waitingTime] {
        std::this_thread::sleep_for(waitingTime);
        std::lock_guard lock(mut);
        v.push_back(i);
        return 1;
      },
      i));
  }

  for(auto& fut : futs)
  {
    auto r = fut.get();
  }

  EXPECT_EQ(v[0], 0);
  for(int i = 1; i < numTasks; ++i)
  {
    EXPECT_EQ(v[i], numTasks - i);
  }
}
#endif
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // THREADPOOLTEST_H_929259185112118831465588811619215055156 
