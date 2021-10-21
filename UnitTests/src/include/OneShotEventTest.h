/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    OneShotEventTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.05.2021 
 */ 
 
#ifndef ONESHOTEVENTTEST_H_267361486036621701611929158802870317432028 
#define ONESHOTEVENTTEST_H_267361486036621701611929158802870317432028 
 
 
// includes
#include "Person.h"

#include <ConcurrencyTools/OneShotEvent.h>

#include <chrono>
#include <stdexcept>
#include <thread>

namespace cct = cctools;

using namespace std::chrono_literals;
using namespace std::string_literals;
 

TEST(NonSharedOneShotEvent, voidEventNoException)
{
  cct::NonSharedOneShotEvent<void> ev;

  int  n = 0;
  auto producerThread = std::async(std::launch::async, [&ev, &n] {
    std::this_thread::sleep_for(2s);
    n = 34;
    ev.notify();
  });

  ev.get();

  EXPECT_EQ(n, 34);
}

TEST(SharedOneShotEvent, voidEventNoException)
{
  cct::SharedOneShotEvent<void> ev;

  int  n              = 0;
  auto producerThread = std::async(std::launch::async, [&ev, &n] {
    std::this_thread::sleep_for(4s);
    n = 34;
    ev.notify();
  });

  bool w1 = false;
  bool w2 = false;

  auto waitingThread1 = std::async(std::launch::async, [&ev, &w1] {
    ev.wait();
    w1 = true;
  });

  auto waitingThread2 = std::async(std::launch::async, [&ev, &w2] {
    ev.wait();
    w2 = true;
  });

  waitingThread1.wait();
  waitingThread2.wait();

  EXPECT_EQ(n, 34);
  EXPECT_TRUE(w1);
  EXPECT_TRUE(w2);
}

TEST(NonSharedOneShotEvent, voidEventThrowException)
{
  cct::NonSharedOneShotEvent<void> ev;

  auto producerThread = std::async(std::launch::async, [&ev] {
    std::this_thread::sleep_for(2s);
    try
    {
      throw std::runtime_error("test exception");
    }
    catch(...)
    {
      ev.setException(std::current_exception());
    }
  });

  try
  {
    ev.get();
  }
  catch(std::exception const& ex)
  {
    const char* msg = ex.what();
    EXPECT_STREQ(msg, "test exception");
  }
}

TEST(NonSharedOneShotEvent, nonVoidEventNoException)
{
  cct::NonSharedOneShotEvent<test::Person> ev;

  auto producerThread = std::async(std::launch::async, [&ev] {
    std::this_thread::sleep_for(2s);
    test::Person p{25, "Paul"s};
    ev.notify(std::move(p));
  });

  const auto p = ev.get();

  EXPECT_EQ(p.getAge(), 25);
  EXPECT_EQ(p.getName(), "Paul"s);
}

TEST(SharedOneShotEvent, nonVoidEventNoException)
{
  cct::SharedOneShotEvent<test::Person> ev;

  auto producerThread = std::async(std::launch::async, [&ev] {
    std::this_thread::sleep_for(2s);
    test::Person p{25, "Paul"s};
    ev.notify(std::move(p));
  });

  test::Person p1;
  test::Person p2;

  auto waitingThread1 = std::async(std::launch::async, [&ev, &p1] {
    p1 = ev.get();
  });

  auto waitingThread2 = std::async(std::launch::async, [&ev, &p2] {
    p2 = ev.get();
  });

  waitingThread1.wait();
  waitingThread2.wait();

  EXPECT_EQ(p1.getAge(), 25);
  EXPECT_EQ(p1.getName(), "Paul"s);
  EXPECT_EQ(p2.getAge(), 25);
  EXPECT_EQ(p2.getName(), "Paul"s);
}

TEST(NonSharedOneShotEvent, nonVoidEventThrowException)
{
  cct::NonSharedOneShotEvent<test::Person> ev;

  auto producerThread = std::async(std::launch::async, [&ev] {
    std::this_thread::sleep_for(2s);
    try
    {
      throw std::runtime_error("test exception");
    }
    catch(...)
    {
      ev.setException(std::current_exception());
    }
  });

  try
  {
    const auto p = ev.get();
  }
  catch(std::exception const& ex)
  {
    const char* msg = ex.what();
    EXPECT_STREQ(msg, "test exception");
  }
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ONESHOTEVENTTEST_H_267361486036621701611929158802870317432028 
