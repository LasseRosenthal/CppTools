/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ThreadsafeQueueTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    17.03.2021 
 */ 
 
#ifndef THREADSAFEQUEUETEST_H_59633034376908910457162452029331560795 
#define THREADSAFEQUEUETEST_H_59633034376908910457162452029331560795 
 
 
// includes
#include "Person.h"
#include <ConcurrencyTools/ThreadsafeQueue.h>
 
#include <chrono>
#include <future>


using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace cctools;


TEST(ThreadsafeQueue, emptyQueue)
{
  Queue<int> queue;
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafeQueue, size)
{
  Queue<int> queue;
  queue.push(23);
  queue.push(42);
  EXPECT_EQ(queue.size(), 2ULL);
}

TEST(ThreadsafeQueue, moveConstructor)
{
  Queue<int> queue1;
  queue1.push(23);
  queue1.push(42);

  Queue<int> queue2(std::move(queue1));

  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue2.size(), 2ULL);
}

TEST(ThreadsafeQueue, moveAssignment)
{
  Queue<int> queue1;
  queue1.push(23);
  queue1.push(42);

  Queue<int> queue2;
  queue2.push(1);
  queue2 = std::move(queue1);
  queue2.push(45);

  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue2.size(), 3ULL);

  int v;
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 23);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 42);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 45);
}

TEST(ThreadsafeQueue, copyAssignment)
{
  Queue<int> queue1;
  queue1.push(23);
  queue1.push(42);

  Queue<int> queue2;
  queue2.push(1);
  queue2 = queue1;
  queue2.push(45);

  EXPECT_EQ(queue1.size(), 2ULL);
  EXPECT_EQ(queue2.size(), 3ULL);

  int v;
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 23);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 42);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 45);
}

TEST(ThreadsafeQueue, tryPopEmptyQueue)
{
  Queue<int> queue;
  int val = 234;
  const bool success = queue.tryPop(val);

  EXPECT_FALSE(success);
  EXPECT_EQ(val, 234);
}

TEST(ThreadsafeQueue, tryPopNonEmptyQueue)
{
  Queue<int> queue;
  queue.push(17);
  int val = 234;
  const bool success = queue.tryPop(val);

  EXPECT_TRUE(success);
  EXPECT_EQ(val, 17);
}

TEST(ThreadsafeQueue, emplace)
{
  Queue<test::Person> queue;
  queue.emplace(43, "Bob"s);
  queue.emplace(13, "Joe"s);

  test::Person p;
  queue.waitAndPop(p);

  EXPECT_EQ(p.getAge(), 43);
  EXPECT_EQ(p.getName(), "Bob"s);
  EXPECT_EQ(queue.size(), 1ULL);
}

TEST(ThreadsafeQueue, waitAndPop)
{
  Queue<int> queue;
  queue.push(23);
  queue.push(42);

  int val = 14;
  queue.waitAndPop(val);

  EXPECT_EQ(val, 23);
  EXPECT_EQ(queue.size(), 1ULL);
}

TEST(ThreadsafeQueue, tryPopForExpectSuccess)
{
  Queue<test::Person> queue;

  auto producer = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(2s); queue.emplace(78, "Bob"s);}
  );

  test::Person p;
  bool success = queue.tryPopFor(p, 4s);

  producer.get();

  EXPECT_TRUE(success);
  EXPECT_EQ(p.getAge(), 78);
  EXPECT_EQ(p.getName(), "Bob"s);
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafeQueue, tryPopForExpectFailure)
{
  Queue<test::Person> queue;

  auto producer = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(3s); queue.emplace(78, "Bob"s);}
  );

  test::Person p;
  bool success = queue.tryPopFor(p, 1s);

  producer.get();

  EXPECT_FALSE(success);
  EXPECT_EQ(p.getAge(), 1);
  EXPECT_EQ(p.getName(), "Anonymous"s);
  EXPECT_EQ(queue.size(), 1ULL);
}

TEST(ThreadsafeQueue, waitAndPopAfterStop)
{
  Queue<int> queue;
  queue.push(23);
  queue.push(42);

  int val = 14;
  queue.stopQueue();
  queue.waitAndPop(val);

  EXPECT_EQ(val, 14);
  EXPECT_EQ(queue.size(), 2ULL);
}

#if 1
TEST(ThreadsafeQueue, waitAndPopEmptyQueueStopped)
{
  Queue<int> queue;

  auto stopThread = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(2s); queue.stopQueue();}
  );

  int val = 14;
  queue.waitAndPop(val);

  stopThread.get();

  EXPECT_EQ(1,1);
}

TEST(ThreadsafeQueue, tryPopForEmptyQueueStopped)
{
  Queue<int> queue;

  auto stopThread = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(2s); queue.stopQueue();}
  );

  int val;
  bool success = queue.tryPopFor(val, 1s);

  stopThread.get();

  EXPECT_FALSE(success);
}

TEST(ThreadsafeQueue, pushCapacityExceededStopQueueDuringPush)
{
  Queue<int> queue(0);

  auto stopThread = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(4s); queue.stopQueue();}
  );

  queue.push(24);

  stopThread.get();

  EXPECT_TRUE(true);
}

TEST(ThreadsafeQueue, pushCapacityExceededQueueStopped)
{
  Queue<int> queue(0);
  queue.stopQueue();
  queue.push(24);

  EXPECT_TRUE(true);
}

TEST(ThreadsafeQueue, tryPushCapacityExceeded)
{
  Queue<int> queue(1);
  queue.push(23);
  const bool success = queue.tryPush(24);

  EXPECT_FALSE(success);
}

TEST(ThreadsafeQueue, tryPushForCapacityExceededRemoveElementInBetween)
{
  Queue<int> queue(2);
  queue.push(23);
  queue.push(97);

  auto removeThread = std::async(std::launch::async,
    [&] {
      std::this_thread::sleep_for(2s);
      int i;
      queue.waitAndPop(i);
      std::this_thread::sleep_for(2s);
      queue.waitAndPop(i);
    }
  );

  int v = 12;
  const bool success1 = queue.tryPushFor(24, 5s);
  const bool success2 = queue.tryPushFor(v, 5s);
  removeThread.get();

  EXPECT_TRUE(success1);
  EXPECT_TRUE(success2);
}

#endif
 
TEST(ThreadsafeQueue, containsExpectTrue)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);

  const bool p = queue.contains([](test::Person const& p) { return p.getName() == "Bob"s; });
  EXPECT_TRUE(p);
}

TEST(ThreadsafeQueue, containsExpectFalse)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);

  const bool p = queue.contains([](test::Person const& p) { return p.getAge() == 76; });
  EXPECT_FALSE(p);
}

TEST(ThreadsafeQueue, hasTopPropertyExpecTrue)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);

  const bool p = queue.hasTopProperty([](test::Person const& p) { return p.getName() == "Bob"s;});
  EXPECT_TRUE(p);
}

TEST(ThreadsafeQueue, hasTopPropertyExpecFalse)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);

  const bool p = queue.hasTopProperty([](test::Person const& p) { return p.getAge() > 98;});
  EXPECT_FALSE(p);
}

TEST(ThreadsafeQueue, removeIf)
{
  Queue<test::Person> queue(2);
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);

  auto pushThread = std::async(std::launch::async, [&] { queue.emplace(81, "Bill"s); });

  std::this_thread::sleep_for(2s);

  const auto r = queue.removeIf([](auto const& p) {return p.getAge() == 56;});
  pushThread.get();
  test::Person p;
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 78);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 81);
  EXPECT_EQ(r, 1ULL);
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafeQueue, extractIfReturnValue)
{
  Queue<test::Person> queue(3);
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  bool tryPush = queue.tryPush(test::Person{12, "Johann"s});
  EXPECT_FALSE(tryPush);
  EXPECT_EQ(queue.size(), 3ULL);

  std::future<void> pushFut = std::async(
    std::launch::async,
    [&queue] {
      queue.push(test::Person{11, "Theodor"s});
    }
  );

  test::Person p;
  std::future<bool> extractFut = std::async(
    std::launch::async,
    [&p, &queue] {
      std::this_thread::sleep_for(2s);
      return queue.extractIf(p, [](auto const& p) { return p.getAge() == 56; });
    }
  );

  pushFut.get();
  bool extracted = extractFut.get();
    
  EXPECT_EQ(queue.size(), 3ULL);
  EXPECT_TRUE(extracted);
  EXPECT_EQ(p.getAge(), 56);
  EXPECT_EQ(p.getName(), "Joe"s);
}

TEST(ThreadsafeQueue, extractIf)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  auto elems = queue.extractIf([](auto const& p) {return p.getAge() > 50 && p.getAge() < 80;});
  auto p1 = elems.begin();
  EXPECT_EQ(p1->getAge(), 78);
  ++p1;
  EXPECT_EQ(p1->getAge(), 56);

  test::Person p;
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 81);
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafeQueue, forEach)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  std::vector<int> ages;
  ages.reserve(queue.size());
  queue.forEach([&ages](const test::Person& p) { ages.push_back(p.getAge()); });

  std::vector<int> expected{78, 56, 81};
  EXPECT_EQ(ages, expected);
}

TEST(ThreadsafeQueue, visitTopNonEmptyQueue)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  const auto age = queue.visitTop([](const test::Person& p) { return p.getAge(); });

  EXPECT_EQ(age, 78);
}

TEST(ThreadsafeQueue, visitTopEmptyQueueDefaultValue)
{
  Queue<test::Person> queue;

  const auto age = queue.visitTop([](const test::Person& p) { return p.getAge(); }, 123);
  EXPECT_EQ(age, 123);
}

TEST(ThreadsafeQueue, visitTopVoidNonEmptyQueue)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  int age = 0;
  queue.visitTop([&age](const test::Person& p) { age = p.getAge(); });

  EXPECT_EQ(age, 78);
}

TEST(ThreadsafeQueue, visitTopVoidEmptyQueueExpectException)
{
  Queue<test::Person> queue;

  int age = 0;
  EXPECT_THROW(queue.visitTop([&age](const test::Person& p) { age = p.getAge(); }), std::out_of_range);
}

TEST(ThreadsafeQueue, extractVisitReinsertExpectValue)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  auto age = 
  queue.extractVisitReinsert(
    [](const test::Person& p) { return p.getAge(); },
    [](const test::Person& p) { return p.getName() == "Joe"s; }
  );

  EXPECT_TRUE(age.has_value());
  EXPECT_EQ(*age, 56);

  test::Person p;
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 78);
  EXPECT_EQ(p.getName(), "Bob"s);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 81);
  EXPECT_EQ(p.getName(), "Bill"s);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 56);
  EXPECT_EQ(p.getName(), "Joe"s);
}

TEST(ThreadsafeQueue, extractVisitReinsertExpectNoValue)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);

  auto age = queue.extractVisitReinsert([](const test::Person& p) { return p.getAge(); },
                                        [](const test::Person& p) { return p.getName() == "Jo"s; });

  EXPECT_FALSE(age.has_value());
}
 
TEST(ThreadsafeQueue, extractVisitVoidReinsert)
{
  Queue<test::Person> queue;
  queue.emplace(78, "Bob"s);
  queue.emplace(56, "Joe"s);
  queue.emplace(81, "Bill"s);
  queue.emplace(86, "Judy"s);

  std::vector<std::string> names;
  names.reserve(3);
  const auto numHosts = queue.extractVisitReinsert([&names](const test::Person& p) { return names.push_back(p.getName()); },
                                                   [](const test::Person& p) { return 70 <= p.getAge() && p.getAge() <= 90; });

  EXPECT_EQ(numHosts, 3);
  EXPECT_EQ(names[0], "Bob"s);
  EXPECT_EQ(names[1], "Bill"s);
  EXPECT_EQ(names[2], "Judy"s);
}
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // THREADSAFEQUEUETEST_H_59633034376908910457162452029331560795 
