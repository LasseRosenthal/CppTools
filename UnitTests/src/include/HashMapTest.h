/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    HashMapTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    30.03.2021 
 */ 
 
#ifndef HASHMAPTEST_H_26288105664719356260633142596395082109 
#define HASHMAPTEST_H_26288105664719356260633142596395082109


// includes
#include "Person.h"

#include <ConcurrencyTools/HashMap.h>

#include <string>
#include <vector>

using namespace std::string_literals;
using namespace cctools;
 

template <typename Key, typename Value>
using ThreadSafeHashMap = cctools::ThreadsafeHashMap<Key, Value, std::hash<Key>>;

template <typename Key, typename Value>
using hashMap = cctools::HashMap<Key, Value>;


TEST(ThreadSafeHashMap, defaultConstructor)
{
  ThreadSafeHashMap<std::string, int> map;
  EXPECT_TRUE(map.empty());
}

TEST(HashMap, defaultConstructor)
{
  hashMap<std::string, int> map;
  EXPECT_TRUE(map.empty());
}

TEST(ThreadSafeHashMap, sequentialConstructor)
{
  ThreadSafeHashMap<std::string, int> map{
    {"Bill"s, 36}, {"Judy"s, 23}, {"Lasse"s, 39}
  };

  EXPECT_EQ(map.size(), 3ULL);
  EXPECT_TRUE(map.contains("Judy"s));
}

TEST(HashMap, sequentialConstructor)
{
  hashMap<std::string, int> map{
    {"Bill"s, 36}, {"Judy"s, 23}, {"Lasse"s, 39}
  };

  EXPECT_EQ(map.size(), 3ULL);
  EXPECT_TRUE(map.contains("Judy"s));
}

TEST(ThreadSafeHashMap, ContainsExpectFalse)
{
  ThreadSafeHashMap<std::string, int> map;
  EXPECT_FALSE(map.contains("string"s));
}

TEST(HashMap, ContainsExpectFalse)
{
  hashMap<std::string, int> map;
  EXPECT_FALSE(map.contains("string"s));
}

TEST(HashMap, indexOperatorKeyNotExistent)
{
  hashMap<std::string, int> map{{"Bill"s, 36}, {"Judy"s, 23}, {"Lasse"s, 39}};

  map["Martin"s] = 46;

  EXPECT_EQ(map.size(), 4ULL);
  EXPECT_TRUE(map.contains("Martin"s));
  EXPECT_EQ(map["Martin"s], 46);
}

TEST(HashMap, acccessViaAtKeyNotExistentExpectThrow)
{
  hashMap<std::string, int> map{{"Bill"s, 36}, {"Judy"s, 23}, {"Lasse"s, 39}};

  EXPECT_THROW(
    map.at("Martin"s) = 46,
    std::out_of_range
  );
}

TEST(HashMap, indexOperatorNoDefaultConstructorKeyNotExistent)
{hashMap<std::string, test::NoDefault> map{{"Bill"s, {36}}, {"Judy"s, {23}}, {"Lasse"s, {39}}};

  auto& v = map["Judy"s];

  EXPECT_EQ(v.value(), 23);
}

TEST(ThreadSafeHashMap, insertOrReplaceCheckSize)
{
  ThreadSafeHashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);
  EXPECT_EQ(map.size(), 1ULL);
}

TEST(HashMap, insertOrReplaceCheckSize)
{
  hashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);
  EXPECT_EQ(map.size(), 1ULL);
}

TEST(ThreadSafeHashMap, moveConstructor)
{
  ThreadSafeHashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);

  ThreadSafeHashMap<std::string, int> mapCopy{std::move(map)};

  EXPECT_TRUE(map.empty());
  EXPECT_EQ(mapCopy.size(), 1ULL);
}

TEST(HashMap, moveConstructor)
{
  hashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);

  hashMap<std::string, int> mapCopy{std::move(map)};

  EXPECT_TRUE(map.empty());
  EXPECT_EQ(mapCopy.size(), 1ULL);
}

TEST(ThreadSafeHashMap, moveAssignment)
{
  ThreadSafeHashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);

  ThreadSafeHashMap<std::string, int> mapCopy;
  mapCopy = std::move(map);

  EXPECT_TRUE(map.empty());
  EXPECT_EQ(mapCopy.size(), 1ULL);
}

TEST(HashMap, moveAssignment)
{
  hashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);

  hashMap<std::string, int> mapCopy;
  mapCopy = std::move(map);

  EXPECT_TRUE(map.empty());
  EXPECT_EQ(mapCopy.size(), 1ULL);
}

TEST(ThreadSafeHashMap, insertOrReplaceCheckIfKeyIsContained)
{
  ThreadSafeHashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);
  EXPECT_TRUE(map.contains("Bill"s));
}

TEST(HashMap, insertOrReplaceCheckIfKeyIsContained)
{
  hashMap<std::string, int> map;
  map.insertOrReplace("Bill"s, 38);
  EXPECT_TRUE(map.contains("Bill"s));
}

TEST(ThreadSafeHashMap, erase)
{
  ThreadSafeHashMap<std::string, int> map{{"Bill"s, 36}, {"Judy"s, 23}, {"Lasse"s, 39}};

  EXPECT_EQ(map.size(), 3ULL);
  EXPECT_TRUE(map.contains("Judy"s));

  const auto n = map.erase("Judy"s);
  EXPECT_EQ(n, 1ULL);
  EXPECT_EQ(map.size(), 2ULL);
  EXPECT_FALSE(map.contains("Judy"s));
}

TEST(HashMap, erase)
{
  hashMap<std::string, int> map{{"Bill"s, 36}, {"Judy"s, 23}, {"Lasse"s, 39}};

  EXPECT_EQ(map.size(), 3ULL);
  EXPECT_TRUE(map.contains("Judy"s));

  const auto n = map.erase("Judy"s);
  EXPECT_EQ(n, 1ULL);
  EXPECT_EQ(map.size(), 2ULL);
  EXPECT_FALSE(map.contains("Judy"s));
}

TEST(ThreadSafeHashMap, emplaceNonExistent)
{
  ThreadSafeHashMap<int, test::Person> map;

  const auto n = map.emplace(1, 8, "Tabea"s);
  EXPECT_EQ(n, 1ULL);
  EXPECT_TRUE(map.contains(1));
  EXPECT_EQ(map.size(), 1ULL);
}

TEST(HashMap, emplaceNonExistent)
{
  hashMap<int, test::Person> map;

  const auto n = map.emplace(1, 8, "Tabea"s);
  EXPECT_EQ(n, 1ULL);
  EXPECT_TRUE(map.contains(1));
  EXPECT_EQ(map.size(), 1ULL);
}

TEST(ThreadSafeHashMap, emplaceExistent)
{
  ThreadSafeHashMap<int, test::Person> map{
    {1, {8, "Elena"s}}
  };

  const auto n = map.emplace(1, 8, "Tabea"s);
  EXPECT_EQ(n, 0ULL);
  EXPECT_TRUE(map.contains(1));
  EXPECT_EQ(map.size(), 1ULL);
}

TEST(HashMap, emplaceExistent)
{
  hashMap<int, test::Person> map{{1, {8, "Elena"s}}};

  const auto n = map.emplace(1, 8, "Tabea"s);
  EXPECT_EQ(n, 0ULL);
  EXPECT_TRUE(map.contains(1));
  EXPECT_EQ(map.size(), 1ULL);
}

TEST(ThreadSafeHashMap, invoke)
{
  ThreadSafeHashMap<int, test::Person> map{{1, {8, "Elena"s}}};

  const auto n = map.emplace(1, 8, "Tabea"s);

  auto age = map.invoke(1, [](test::Person const& p) { return p.getAge(); });
  const auto& name = map.invoke(1, [](test::Person const& p) { return p.getName(); });

  EXPECT_TRUE((std::is_same_v<decltype(age), int>));

  EXPECT_EQ(name, "Tabea"s);
  EXPECT_EQ(age, 8);

  map.invoke(1, [](test::Person& p) { p.setName("Theodor"s); });
  map.invoke(1, [](test::Person& p) { p.setAge(11); });

  const auto& name2 = map.invoke(1, [](test::Person const& p) { return p.getName(); });
  age               = map.invoke(1, [](test::Person const& p) { return p.getAge(); });
  EXPECT_EQ(name2, "Theodor"s);
  EXPECT_EQ(age, 11);
}
 
TEST(HashMap, invoke)
{
  HashMap<int, test::Person> map{{1, {8, "Elena"s}}};

  const auto n = map.emplace(1, 8, "Tabea"s);

  auto        age  = map.invoke(1, [](test::Person const& p) { return p.getAge(); });
  const auto& name = map.invoke(1, [](test::Person const& p) { return p.getName(); });

  EXPECT_TRUE((std::is_same_v<decltype(age), int>));

  EXPECT_EQ(name, "Tabea"s);
  EXPECT_EQ(age, 8);

  map.invoke(1, [](test::Person& p) { p.setName("Theodor"s); });
  map.invoke(1, [](test::Person& p) { p.setAge(11); });

  const auto& name2 = map.invoke(1, [](test::Person const& p) { return p.getName(); });
  age               = map.invoke(1, [](test::Person const& p) { return p.getAge(); });
  EXPECT_EQ(name2, "Theodor"s);
  EXPECT_EQ(age, 11);
}

TEST(HashMap, forEachValue)
{
  HashMap<int, test::Person> map{{1, {9, "Elena"s}}, {2, {8, "Tabea"s}}};

  std::vector<int> ages(map.size());

  map.forEachValue([&ages](const auto& entry) {
    const auto& [key, person] = entry;
    if(person.getName() == "Elena"s)
      ages[0] = person.getAge();
    else
      ages[1] = person.getAge();
  });

  EXPECT_EQ(ages[0], 9);
  EXPECT_EQ(ages[1], 8);
}

TEST(ThreadSafeHashMap, forEachValue)
{
  const ThreadSafeHashMap<int, test::Person> map{{1, {9, "Elena"s}}, {2, {8, "Tabea"s}}};

  std::vector<int> ages(map.size());

  map.forEachValue([&ages](const auto& entry) {
    const auto& [key, person] = entry;
    if(person.getName() == "Elena"s)
      ages[0] = person.getAge();
    else
      ages[1] = person.getAge();
  });

  EXPECT_EQ(ages[0], 9);
  EXPECT_EQ(ages[1], 8);
}

TEST(ThreadSafeHashMap, invokeExpectException)
{
  ThreadSafeHashMap<int, test::Person> map{{1, {8, "Elena"s}}};

  EXPECT_THROW(
    map.invoke(2, [](test::Person& p) { p.setAge(9); }),
    std::out_of_range
  );
}

TEST(HashMap, invokeExpectException)
{
  HashMap<int, test::Person> map{{1, {8, "Elena"s}}};

  EXPECT_THROW(
    map.invoke(2, [](test::Person& p) { p.setAge(9); }),
    std::out_of_range
  );
}


 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // HASHMAPTEST_H_26288105664719356260633142596395082109 
