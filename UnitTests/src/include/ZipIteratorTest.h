/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ZipIteratorTest.h
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.08.2020 
 */ 
 
#ifndef ZIPITERATORTEST_H_9292309945582746231067125614027801430482 
#define ZIPITERATORTEST_H_9292309945582746231067125614027801430482 
 
 
// includes
#include <Utils/ZipIterator.h>

#include <list>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std::string_literals;
//using namespace cpptools;


class ZipIteratorTest : public ::testing::Test {

protected:

  // ---------------------------------------------------
  // methods inherited from ::testing::Test
  void SetUp() override;

  // ---------------------------------------------------
  // parameters prepared by this test fixture
 
  std::vector<int> intVec{1,2,3};
  std::list<std::string> strList{"Eins"s, "Zwei"s, "Drei"s};
  const std::vector<int> cintVec{1,2,3};
  const std::list<std::string> cstrList{"Eins"s, "Zwei"s, "Drei"s};


  using Zipper = cpptools::Zipper<std::vector<int>, std::list<std::string>>;
  using zipIterator = Zipper::iterator;
  using constZipIterator = Zipper::const_iterator;
};

inline void ZipIteratorTest::SetUp()
{
}

TEST_F(ZipIteratorTest, TypeCheckNonConstZipIterator)
{
  EXPECT_TRUE((std::is_same_v<zipIterator::referenceT<0>, int&> &&
               std::is_same_v<zipIterator::referenceT<1>, std::string&>));
}

TEST_F(ZipIteratorTest, TypeCheckConstZipIterator)
{
  EXPECT_TRUE((std::is_same_v<constZipIterator::referenceT<0>, int const&> &&
               std::is_same_v<constZipIterator::referenceT<1>, std::string const&>));
}

TEST_F(ZipIteratorTest, DereferenceNonConstIterator)
{
  zipIterator zipIt(intVec.begin(), strList.begin());
  const auto& val = *zipIt;
  const auto expected = std::make_tuple(1, "Eins"s);
  EXPECT_EQ(val, expected);
}

TEST_F(ZipIteratorTest, DereferenceConstIterator)
{
  constZipIterator zipIt(intVec.cbegin(), strList.cbegin());
  const auto& val = *zipIt;
  const auto expected = std::make_tuple(1, "Eins"s);
  EXPECT_EQ(val, expected);
}

TEST_F(ZipIteratorTest, DereferenceAndModify)
{
  zipIterator zipIt(intVec.begin(), strList.begin());

  auto& [val, str] = *zipIt;
  val = 11;
  str = "Elf"s;

  const auto expected = std::make_tuple(11, "Elf"s);
  const auto tup = *zipIt;
  EXPECT_EQ(tup, expected);
}

TEST_F(ZipIteratorTest, DereferenceAfterIncrement)
{
  zipIterator zipIt(intVec.begin(), strList.begin());

  ++zipIt;
  const auto val = *zipIt;
  const auto expected = std::make_tuple(2, "Zwei"s);

  EXPECT_EQ(val, expected);
}

TEST_F(ZipIteratorTest, DereferenceAfterIncrementAndModification)
{
  zipIterator zipIt(intVec.begin(), strList.begin());

  ++zipIt;
  auto& [intVal, str] = *zipIt;
  intVal = 14;
  str = "Vierzehn"s;

  const auto val = *zipIt;
  const auto expected = std::make_tuple(14, "Vierzehn"s);

  EXPECT_EQ(val, expected);
}

TEST_F(ZipIteratorTest, TestForEqualityExpectTrue)
{
  zipIterator zipIt(intVec.begin(), strList.begin());
  zipIterator zipIt2(intVec.begin(), strList.begin());

  ++zipIt;
  ++zipIt2;

  EXPECT_TRUE(zipIt == zipIt2);
}

TEST_F(ZipIteratorTest, TestForEqualityExpectFalse)
{
  zipIterator zipIt(intVec.begin(), strList.begin());
  zipIterator zipIt2(std::next(intVec.begin()), std::next(strList.begin()));

  ++zipIt;
  ++zipIt2;

  EXPECT_TRUE(zipIt != zipIt2);
}

TEST_F(ZipIteratorTest, TestForEqualityAfterCopy)
{
  zipIterator zipIt(intVec.begin(), strList.begin());
  auto zipIt2 = zipIt;

  ++zipIt;
  ++zipIt2;

  EXPECT_TRUE(zipIt == zipIt2);
}

TEST_F(ZipIteratorTest, ZipperLoop)
{
  std::vector<int> nums;
  std::vector<std::string> strs;

  nums.reserve(intVec.size());
  strs.reserve(strList.size());

  for(const auto& [num, str] : Zipper{intVec, strList})
  {
    nums.push_back(num);
    strs.push_back(str);
  }

  std::vector<std::string> expectedStrings {"Eins"s, "Zwei"s, "Drei"s};

  EXPECT_TRUE((nums == intVec && strs == expectedStrings));
}

template <typename Tuple>
auto multValues(Tuple const& tup) {
  return multValues(tup, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}

template <typename Tuple, std::size_t... Is>
auto multValues(Tuple const& tup, std::index_sequence<Is...>) {
  return (... * std::get<Is>(tup));
}

TEST_F(ZipIteratorTest, DotProduct)
{
  const cpptools::Zipper zipper{intVec, intVec, intVec, intVec};

  const auto sum = std::accumulate(zipper.cbegin(), zipper.cend(), 0,
    [](auto val, const auto& tuple) { return val + multValues(tuple);});

  EXPECT_EQ(sum, 98);
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ZIPITERATORTEST_H_9292309945582746231067125614027801430482 
