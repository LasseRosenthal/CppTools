/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    MultiTypeMapTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    07.07.2020 
 */ 
 
#ifndef MULTITYPEMAPTEST_H_193778450724512453146281666903204891795 
#define MULTITYPEMAPTEST_H_193778450724512453146281666903204891795 
 
 
// includes
#include <Utils/MultiTypeMap.h>

using namespace std::string_literals;
 

/**
 *  @brief  Test fixture class for MultiTypeMap tests.
 */
template <template <typename...> class Map>
class MultiTypeMaptest : public ::testing::Test {

protected:

  // ---------------------------------------------------
  // methods inherited from ::testing::Test
  void SetUp() override;

  // ---------------------------------------------------
  // parameters prepared by this test fixture
  using mapType    = cpptools::MultiTypeMapT<Map, std::string, double, std::wstring, int, char>;
  using valueType  = typename mapType::value_type;
  using mappedType = typename mapType::mapped_type;

  mapType multiMap;
};

template <template <typename...> class Map>
inline void MultiTypeMaptest<Map>::SetUp()
{
  multiMap["monday"s]    = 12.3;
  multiMap["tuesday"s]   = L"string"s;
  multiMap["wednesday"s] = 42;
  multiMap["thursday"s]  = 'c';
}

using MultiTypeMapTest = MultiTypeMaptest<std::map>;
using MultiTypeHashMapTest = MultiTypeMaptest<std::unordered_map>;

/**
 * @brief  tests the empty method for a non empty map.
 */
TEST_F(MultiTypeHashMapTest, EmptyExpectFalse)
{
  EXPECT_FALSE(multiMap.empty());
}

TEST_F(MultiTypeMapTest, EmptyExpectFalse)
{
  EXPECT_FALSE(multiMap.empty());
}

/**
 * @brief  tests the size method for a non empty map.
 */
TEST_F(MultiTypeHashMapTest, SizeNotEmpty)
{
  EXPECT_EQ(multiMap.size(), 4);
}

TEST_F(MultiTypeMapTest, SizeNotEmpty)
{
  EXPECT_EQ(multiMap.size(), 4);
}

/**
 * @brief  tests the clear method for a non empty map.
 */
TEST_F(MultiTypeHashMapTest, Clear)
{
  multiMap.clear();
  EXPECT_TRUE(multiMap.empty());
}

TEST_F(MultiTypeMapTest, Clear)
{
  multiMap.clear();
  EXPECT_TRUE(multiMap.empty());
}

/**
 * @brief  tests the insert method for a non empty map.
 */
TEST_F(MultiTypeHashMapTest, Insert)
{
  multiMap.insert({"friday"s, mappedType{13}});
  EXPECT_EQ(multiMap.get<int>("friday"s), 13);
}

TEST_F(MultiTypeMapTest, Insert)
{
  multiMap.insert({"friday"s, mappedType{13}});
  EXPECT_EQ(multiMap.get<int>("friday"s), 13);
}

/**
 * @brief  tests the find method for a non empty map.
 */
TEST_F(MultiTypeHashMapTest, FindSuccess)
{
  const auto m = multiMap.find("monday"s);
  EXPECT_NE(multiMap.end(), m);
}

TEST_F(MultiTypeMapTest, FindSuccess)
{
  const auto m = multiMap.find("monday"s);
  EXPECT_NE(multiMap.end(), m);
}

TEST_F(MultiTypeHashMapTest, FindNoSuccess)
{
  const auto m = multiMap.find("sunday"s);
  EXPECT_EQ(multiMap.end(), m);
}

TEST_F(MultiTypeMapTest, FindNoSuccess)
{
  const auto m = multiMap.find("sunday"s);
  EXPECT_EQ(multiMap.end(), m);
}

/**
 * @brief  tests if the get method returns the correct value.
 */
TEST_F(MultiTypeHashMapTest, AccessOperatorValue)
{
  EXPECT_EQ(multiMap.get<std::wstring>("tuesday"s), L"string"s);
}

TEST_F(MultiTypeMapTest, AccessOperatorValue)
{
  EXPECT_EQ(multiMap.get<std::wstring>("tuesday"s), L"string"s);
}

TEST_F(MultiTypeHashMapTest, AccessVariantAt)
{
  auto v = multiMap.at("thursday"s);
  EXPECT_EQ(std::get<char>(v), 'c');
}

TEST_F(MultiTypeMapTest, AccessVariantAt)
{
  auto v = multiMap.at("thursday"s);
  EXPECT_EQ(std::get<char>(v), 'c');
}

TEST_F(MultiTypeHashMapTest, AccessVariantAtModify)
{
  decltype(auto) v = multiMap.at("thursday"s);
  v = L"Hallo"s;
  EXPECT_EQ(multiMap.get<std::wstring>("thursday"s), L"Hallo"s);
}

TEST_F(MultiTypeMapTest, AccessVariantAtModify)
{
  decltype(auto) v = multiMap.at("thursday"s);
  v = L"Hallo"s;
  EXPECT_EQ(multiMap.get<std::wstring>("thursday"s), L"Hallo"s);
}

TEST_F(MultiTypeHashMapTest, AccessVariantAtNoModification)
{
  auto v = multiMap.at("thursday"s);
  v = L"Hallo"s;
  EXPECT_EQ(multiMap.get<char>("thursday"s), 'c');
}

TEST_F(MultiTypeMapTest, AccessVariantAtNoModification)
{
  auto v = multiMap.at("thursday"s);
  v = L"Hallo"s;
  EXPECT_EQ(multiMap.get<char>("thursday"s), 'c');
}

/**
 * @brief  tests if the get method returns the correct value after modification.
 */
TEST_F(MultiTypeHashMapTest, ModifyValue)
{
  multiMap["tuesday"s] = 56;
  EXPECT_EQ(multiMap.get<int>("tuesday"s), 56);
}

TEST_F(MultiTypeMapTest, ModifyValue)
{
  multiMap["tuesday"s] = 56;
  EXPECT_EQ(multiMap.get<int>("tuesday"s), 56);
}

/**
 * @brief  tests if the get method returns a const reference.
 */
TEST_F(MultiTypeHashMapTest, ReturnTypeExpectNonConstRef)
{
  decltype(auto) strRef = multiMap.get<std::wstring>("tuesday"s);
  EXPECT_TRUE((std::is_same_v<decltype(strRef),  std::wstring&>));
}

TEST_F(MultiTypeMapTest, ReturnTypeExpectNonConstRef)
{
  decltype(auto) strRef = multiMap.get<std::wstring>("tuesday"s);
  EXPECT_TRUE((std::is_same_v<decltype(strRef),  std::wstring&>));
}

/**
 * @brief  tests if the get method returns a double by value.
 */
TEST_F(MultiTypeHashMapTest, ReturnTypeExpectDouble)
{
  decltype(auto) val = multiMap.get<double>("monday"s);
  EXPECT_TRUE((std::is_same_v<decltype(val), double>));
}

TEST_F(MultiTypeMapTest, ReturnTypeExpectDouble)
{
  decltype(auto) val = multiMap.get<double>("monday"s);
  EXPECT_TRUE((std::is_same_v<decltype(val), double>));
}

/**
 * @brief  tests if the get method returns a const reference.
 */
TEST_F(MultiTypeHashMapTest, ModifyValueViaReference)
{
  decltype(auto) strRef = multiMap.get<std::wstring>("tuesday"s);
  strRef = L"myString"s;
  EXPECT_EQ(multiMap.get<std::wstring>("tuesday"s), L"myString"s);
}

TEST_F(MultiTypeMapTest, ModifyValueViaReference)
{
  decltype(auto) strRef = multiMap.get<std::wstring>("tuesday"s);
  strRef = L"myString"s;
  EXPECT_EQ(multiMap.get<std::wstring>("tuesday"s), L"myString"s);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTITYPEMAPTEST_H_193778450724512453146281666903204891795 
