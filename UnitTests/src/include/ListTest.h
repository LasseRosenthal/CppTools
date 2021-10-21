/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ListTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    30.06.2021 
 */ 
 
#ifndef LISTTEST_H_20370149751152870315304755015221939914892 
#define LISTTEST_H_20370149751152870315304755015221939914892 
 
 
// includes
#include <ConcurrencyTools/List.h>
#include "Person.h"

#include <vector>

 
TEST(List, DefaultConstructor)
{
  cctools::List<int> myList;
  myList.push_front(3);
}


TEST(List, removeIf)
{
  cctools::List<int> myList;
  myList.push_front(3);
  myList.remove_if([](int v) { return v == 3; });
}

TEST(ThreadsafeList, DefaultConstructor)
{
  cctools::ThreadsafeList<int> myList;
  myList.push_front(3);
}

TEST(ThreadsafeList, pushBack)
{
  cctools::ThreadsafeList<int> myList;
  myList.push_back(3);
  myList.push_back(5);
  myList.push_back(7);
  myList.push_back(8);
  myList.push_back(9);

  std::vector<int> vals;
  vals.reserve(5);
  myList.for_each([&vals](int v) { vals.push_back(v); });

  EXPECT_EQ(vals, (std::vector<int>{ 3,5,7,8,9 }));
}

TEST(List, pushBack)
{
  cctools::List<int> myList;
  myList.push_back(3);
  myList.push_back(5);
  myList.push_back(7);
  myList.push_back(8);
  myList.push_back(9);

  std::vector<int> vals;
  vals.reserve(5);
  myList.for_each([&vals](int v) { vals.push_back(v); });

  EXPECT_EQ(vals, (std::vector<int>{ 3,5,7,8,9 }));
}

TEST(ThreadsafeList, pushFront)
{
  cctools::ThreadsafeList<int> myList;
  myList.push_front(3);
  myList.push_front(5);
  myList.push_front(7);
  myList.push_front(8);
  myList.push_front(9);

  std::vector<int> vals;
  vals.reserve(5);
  myList.for_each([&vals](int v) { vals.push_back(v); });

  EXPECT_EQ(vals, (std::vector<int>{ 9,8,7,5,3 }));
}

TEST(List, pushFront)
{
  cctools::List<int> myList;
  myList.push_front(3);
  myList.push_front(5);
  myList.push_front(7);
  myList.push_front(8);
  myList.push_front(9);

  std::vector<int> vals;
  vals.reserve(5);
  myList.for_each([&vals](int v) { vals.push_back(v); });

  EXPECT_EQ(vals, (std::vector<int>{ 9,8,7,5,3 }));
}

TEST(ThreadsafeList, removeIf)
{
  cctools::ThreadsafeList<int> myList;
  myList.push_front(3);
  myList.push_front(5);
  myList.push_front(7);
  myList.push_front(8);
  myList.push_front(9);

  myList.remove_if([](int v) { return v == 7 || v == 5; });

  std::vector<int> vals;
  vals.reserve(3);
  myList.for_each([&vals](int v) { vals.push_back(v); });

  EXPECT_EQ(vals, (std::vector{ 9,8,3 }));
}
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // LISTTEST_H_20370149751152870315304755015221939914892 
