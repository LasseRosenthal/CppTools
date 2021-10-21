/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ThreadsafePriorityQueueTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    18.03.2021 
 */ 
 
#ifndef THREADSAFEPRIORITYQUEUETEST_H_116503081019092186411993525727974467920746 
#define THREADSAFEPRIORITYQUEUETEST_H_116503081019092186411993525727974467920746 
 
 
// includes 
#include "Person.h"
#include <ConcurrencyTools/ThreadsafeQueue.h>
 
#include <chrono>
#include <future>


using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace cctools;

TEST(ThreadsafePriorityQueue, emptyQueue)
{
  PriorityQueue<int> queue;
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafePriorityQueue, size)
{
  PriorityQueue<int> queue;
  queue.push(23);
  queue.push(42);
  EXPECT_EQ(queue.size(), 2ULL);
}

TEST(ThreadsafePriorityQueue, moveConstructor)
{
  PriorityQueue<int> queue1;
  queue1.push(23);
  queue1.push(42);

  PriorityQueue<int> queue2(std::move(queue1));

  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue2.size(), 2ULL);
}

TEST(ThreadsafePriorityQueue, moveAssignment)
{
  PriorityQueue<int> queue1;
  queue1.push(23);
  queue1.push(42);

  PriorityQueue<int> queue2;
  queue2.push(1);
  queue2 = std::move(queue1);
  queue2.push(45);

  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue2.size(), 3ULL);

  int v;
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 45);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 42);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 23);
}

TEST(ThreadsafePriorityQueue, copyAssignment)
{
  PriorityQueue<int> queue1;
  queue1.push(23);
  queue1.push(42);

  PriorityQueue<int> queue2;
  queue2.push(1);
  queue2 = queue1;
  queue2.push(45);

  EXPECT_EQ(queue1.size(), 2ULL);
  EXPECT_EQ(queue2.size(), 3ULL);

  int v;
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 45);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 42);
  queue2.waitAndPop(v);
  EXPECT_EQ(v, 23);
}

TEST(ThreadsafePriorityQueue, tryPopEmptyQueue)
{
  PriorityQueue<int> queue;
  int val = 234;
  const bool success = queue.tryPop(val);

  EXPECT_FALSE(success);
  EXPECT_EQ(val, 234);
}

TEST(ThreadsafePriorityQueue, tryPopNonEmptyQueue)
{
  PriorityQueue<int> queue;
  queue.push(17);
  int val = 234;
  const bool success = queue.tryPop(val);

  EXPECT_TRUE(success);
  EXPECT_EQ(val, 17);
}

TEST(ThreadsafePriorityQueue, waitAndPop)
{
  PriorityQueue<int> queue;
  queue.push(23);
  queue.push(42);

  int val = 14;
  queue.waitAndPop(val);

  EXPECT_EQ(val, 42);
  EXPECT_EQ(queue.size(), 1ULL);
}

TEST(ThreadsafePriorityQueue, tryPopForExpectSuccess)
{
  PriorityQueue<test::Person> queue;

  auto producer = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(2s); queue.push(test::Person(78, "Bob"s));}
  );

  test::Person p;
  bool success = queue.tryPopFor(p, 4s);

  producer.get();

  EXPECT_TRUE(success);
  EXPECT_EQ(p.getAge(), 78);
  EXPECT_EQ(p.getName(), "Bob"s);
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafePriorityQueue, tryPopForExpectFailure)
{
  PriorityQueue<test::Person> queue;

  auto producer = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(3s); queue.push(test::Person(78, "Bob"s));}
  );

  test::Person p;
  bool success = queue.tryPopFor(p, 1s);

  producer.get();

  EXPECT_FALSE(success);
  EXPECT_EQ(p.getAge(), 1);
  EXPECT_EQ(p.getName(), "Anonymous"s);
  EXPECT_EQ(queue.size(), 1ULL);
}

TEST(ThreadsafePriorityQueue, waitAndPopAfterStop)
{
  PriorityQueue<int> queue;
  queue.push(23);
  queue.push(42);

  int val = 14;
  queue.stopQueue();
  queue.waitAndPop(val);

  EXPECT_EQ(val, 14);
  EXPECT_EQ(queue.size(), 2ULL);
}


#if 1
TEST(ThreadsafePriorityQueue, waitAndPopEmptyQueueStopped)
{
  PriorityQueue<int> queue;

  auto stopThread = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(2s); queue.stopQueue();}
  );

  int val = 14;
  queue.waitAndPop(val);

  stopThread.get();

  EXPECT_EQ(1,1);
}

TEST(ThreadsafePriorityQueue, tryPopForEmptyQueueStopped)
{
  PriorityQueue<int> queue;

  auto stopThread = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(2s); queue.stopQueue();}
  );

  int val;
  bool success = queue.tryPopFor(val, 1s);

  stopThread.get();

  EXPECT_FALSE(success);
}

TEST(ThreadsafePriorityQueue, pushCapacityExceededStopQueueDuringPush)
{
  PriorityQueue<int> queue(0);

  auto stopThread = std::async(std::launch::async,
    [&](){ std::this_thread::sleep_for(4s); queue.stopQueue();}
  );

  queue.push(24);

  stopThread.get();

  EXPECT_TRUE(true);
}

TEST(ThreadsafePriorityQueue, pushCapacityExceededQueueStopped)
{
  PriorityQueue<int> queue(0);
  queue.stopQueue();
  queue.push(24);

  EXPECT_TRUE(true);
}

TEST(ThreadsafePriorityQueue, tryPushCapacityExceeded)
{
  PriorityQueue<int> queue(1);
  queue.push(23);
  const bool success = queue.tryPush(24);

  EXPECT_FALSE(success);
}

TEST(ThreadsafePriorityQueue, tryPushForCapacityExceededRemoveElementInBetween)
{
  PriorityQueue<int> queue(2);
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

TEST(ThreadsafePriorityQueue, containsExpectTrue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});

  const bool p = queue.contains([](test::Person const& p) { return p.getName() == "Bob"s; });
  EXPECT_TRUE(p);
}

TEST(ThreadsafePriorityQueue, containsExpectFalse)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});

  const bool p = queue.contains([](test::Person const& p) { return p.getAge() == 76; });
  EXPECT_FALSE(p);
}

TEST(ThreadsafePriorityQueue, hasTopPropertyExpecTrue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});

  const bool p = queue.hasTopProperty([](test::Person const& p) { return p.getName() == "Bob"s;});
  EXPECT_TRUE(p);
}

TEST(ThreadsafePriorityQueue, hasTopPropertyExpecFalse)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});

  const bool p = queue.hasTopProperty([](test::Person const& p) { return p.getAge() > 98;});
  EXPECT_FALSE(p);
}

TEST(ThreadsafePriorityQueue, removeIf)
{
  PriorityQueue<test::Person, std::greater<test::Person>, std::list> queue(3);
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{67, "Jane"s});


  auto pushThread = std::async(std::launch::async, [&] { queue.push(test::Person{81, "Bill"s}); });

  std::this_thread::sleep_for(2s);

  const auto r = queue.removeIf([](auto const& p) { return p.getAge() < 70;});
  pushThread.get();
  test::Person p;
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 78);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 81);
  EXPECT_EQ(r, 2ULL);
  EXPECT_TRUE(queue.empty());
}

TEST(ThreadsafePriorityQueue, extractIfReturnValue)
{
  PriorityQueue<test::Person> queue(3);
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  bool tryPush = queue.tryPush(test::Person{12, "Johann"s});
  EXPECT_FALSE(tryPush);
  EXPECT_EQ(queue.size(), 3ULL);

  std::future<void> pushFut = std::async(
    std::launch::async,
    [&queue] { queue.push(test::Person{11, "Theodor"s}); }
  );

  test::Person      p;
  std::future<bool> extractFut = std::async(std::launch::async, [&p, &queue] {
    std::this_thread::sleep_for(2s);
    return queue.extractIf(p, [](auto const& p) { return p.getAge() == 56; });
  });

  pushFut.get();
  bool extracted = extractFut.get();

  EXPECT_EQ(queue.size(), 3ULL);
  EXPECT_TRUE(extracted);
  EXPECT_EQ(p.getAge(), 56);
  EXPECT_EQ(p.getName(), "Joe"s);
}

TEST(ThreadsafePriorityQueue, extractIf)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

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

TEST(ThreadsafePriorityQueue, extractVisitReinsertReturnValue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  test::Person p;
  const auto name = queue.extractVisitReinsert(
    [](test::Person& p) { p.setAge(23); return p.getName();},
    [](test::Person const& p) { return p.getAge() >= 80;}
  );

  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 78);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 56);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 23);
  EXPECT_EQ(p.getName(), "Bill"s);
  EXPECT_EQ(name, "Bill"s);
}

TEST(ThreadsafePriorityQueue, extractVisitReinsertReturnValueNullopt)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  const auto name = queue.extractVisitReinsert(
    [](test::Person& p) { p.setAge(23); return p.getName();},
    [](test::Person const& p) { return p.getAge() >= 90;}
  );

  EXPECT_FALSE(name.has_value());

  test::Person p;
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 81);
  EXPECT_EQ(p.getName(), "Bill"s);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 78);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 56);
}

TEST(ThreadsafePriorityQueue, forEach)
{
  PriorityQueue<test::Person, std::greater<test::Person>> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  std::vector<int> ages;
  ages.reserve(queue.size());
  queue.forEach([&ages](const test::Person& p) { ages.push_back(p.getAge()); });

  std::vector<int> expected{56, 78, 81};
  EXPECT_EQ(ages, expected);
}

TEST(ThreadsafePriorityQueue, visitTopNonEmptyQueue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  const auto age = queue.visitTop([](const test::Person& p) { return p.getAge(); });

  EXPECT_EQ(age, 81);
}

TEST(ThreadsafePriorityQueue, visitTopEmptyQueueDefaultValue)
{
  PriorityQueue<test::Person> queue;

  const auto age = queue.visitTop([](const test::Person& p) { return p.getAge(); }, 123);
  EXPECT_EQ(age, 123);
}

TEST(ThreadsafePriorityQueue, visitTopVoidNonEmptyQueue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  int age = 0;
  queue.visitTop([&age](const test::Person& p) { age = p.getAge(); });

  EXPECT_EQ(age, 81);
}

TEST(ThreadsafePriorityQueue, visitTopVoidEmptyQueueExpectException)
{
  PriorityQueue<test::Person> queue;

  int age = 0;
  EXPECT_THROW(queue.visitTop([&age](const test::Person& p) { age = p.getAge(); }), std::out_of_range);
}

TEST(ThreadsafePriorityQueue, extractVisitReinsertExpectValue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  auto age = queue.extractVisitReinsert([](const test::Person& p) { return p.getAge(); },
                                        [](const test::Person& p) { return p.getName() == "Joe"s; });

  EXPECT_TRUE(age.has_value());
  EXPECT_EQ(*age, 56);

  test::Person p;
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 81);
  EXPECT_EQ(p.getName(), "Bill"s);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 78);
  EXPECT_EQ(p.getName(), "Bob"s);
  queue.waitAndPop(p);
  EXPECT_EQ(p.getAge(), 56);
  EXPECT_EQ(p.getName(), "Joe"s);
}

TEST(ThreadsafePriorityQueue, extractVisitReinsertExpectNoValue)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});

  auto age = queue.extractVisitReinsert([](const test::Person& p) { return p.getAge(); },
                                        [](const test::Person& p) { return p.getName() == "Jo"s; });

  EXPECT_FALSE(age.has_value());
}

TEST(ThreadsafePriorityQueue, extractVisitVoidReinsert)
{
  PriorityQueue<test::Person> queue;
  queue.push(test::Person{78, "Bob"s});
  queue.push(test::Person{56, "Joe"s});
  queue.push(test::Person{81, "Bill"s});
  queue.push(test::Person{86, "Judy"s});

  std::vector<std::string> names;
  names.reserve(3);
  const auto numHosts = queue.extractVisitReinsert([&names](const test::Person& p) { return names.push_back(p.getName()); },
                                                   [](const test::Person& p) { return 70 <= p.getAge() && p.getAge() <= 90; });

  EXPECT_EQ(numHosts, 3);
  EXPECT_EQ(names[0], "Judy"s);
  EXPECT_EQ(names[1], "Bill"s);
  EXPECT_EQ(names[2], "Bob"s);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // THREADSAFEPRIORITYQUEUETEST_H_116503081019092186411993525727974467920746 
