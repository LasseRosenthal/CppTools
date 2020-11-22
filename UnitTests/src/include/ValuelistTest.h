/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ValuelistTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.06.2020 
 */ 
 
#ifndef VALUELISTTEST_H_28385142461954299802351053518262318072436 
#define VALUELISTTEST_H_28385142461954299802351053518262318072436 
 
 
// includes
#include <Meta/Valuelist.h>

#include <string>
#include <type_traits>
 
 
using namespace meta;


/**
 * @brief  tests the ValueType method for an empty Valuelist of type int.
 */
TEST(Valuelist, TypeNonEmptyOneElement)
{
  using myType = int;
  using myList = Valuelist<myType>;
  EXPECT_TRUE((std::is_same_v<valuelist::Type<myList>, myType>));
}

/**
 * @brief  tests the ValueType method for a non empty Valuelist of type std::size_t.
 */
TEST(Valuelist, TypeNonEmptyTwoElements)
{
  using myType = std::size_t;
  using myList = Valuelist<myType, 1UL, 2UL>;
  EXPECT_TRUE((std::is_same_v<valuelist::Type<myList>, myType>));
}

/**
 * @brief  tests the ValueType method for an empty Valuelist of type std::size_t.
 */
TEST(Valuelist, TypeEmpty)
{
  using myType = std::size_t;
  using myList = Valuelist<myType>;
  EXPECT_TRUE((std::is_same_v<valuelist::Type<myList>, myType>));
}

/**
 * @brief  tests the Size method for an empty Valuelist.
 */
TEST(Valuelist, SizeEmpty)
{
  using myList = Valuelist<int>;
  EXPECT_EQ(valuelist::Size<myList>, 0);
}

/**
 * @brief  tests the Size method for a non-empty Valuelist.
 */
TEST(Valuelist, SizeNonEmpty)
{
  using myList = Valuelist<int, 1, 2, 3, 6, 9>;
  EXPECT_EQ(valuelist::Size<myList>, 5);
}

/**
 * @brief  tests the IsEmpty method for an empty Valuelist.
 */
TEST(Valuelist, IsEmptyEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_TRUE(valuelist::IsEmpty<myList>);
}

/**
 * @brief  tests the IsEmpty method for a non-empty Valuelist.
 */
TEST(Valuelist, IsEmptyNonEmptyList)
{
  using myList = Valuelist<int, 16>;
  EXPECT_FALSE(valuelist::IsEmpty<myList>);
}

/**
 * @brief  tests the Iota method for a size equal to zero.
 */
TEST(Valuelist, IotaSizeZero)
{
  constexpr int size = 0;
  using myType = int;
  using expected = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::Iota<size>, expected>));
}

/**
 * @brief  tests the Iota method for a size equal to zero.
 */
TEST(Valuelist, IotaSizeEqualsOne)
{
  constexpr int size = 1;
  using myType = int;
  using expected = Valuelist<int, 0>;
  EXPECT_TRUE((std::is_same_v<valuelist::Iota<size>, expected>));
}

/**
 * @brief  tests the Iota method for a size larger than zero.
 */
TEST(Valuelist, IotaSizeEqualsFive)
{
  constexpr int size = 5;
  using myType = int;
  using expected = Valuelist<int,0,1,2,3,4>;
  EXPECT_TRUE((std::is_same_v<valuelist::Iota<size>, expected>));
}

/**
 * @brief  tests the Front method for Valuelist.
 */
TEST(Valuelist, FrontNonEmptyList)
{
  using myList = Valuelist<int, 31, 2, 12>;
  EXPECT_EQ(valuelist::Front<myList>, 31);
}

/**
 *  @brief  tests the PushFront method for an empty Valuelist.
 */
TEST(Valuelist, PushFrontEmptyList)
{
  constexpr int newElement = 692;
  using myList             = Valuelist<int>;
  using myExtendedList     = Valuelist<int, newElement>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<valuelist::PushFront<myList, newElement>, myExtendedList>));
}

/**
 *  @brief  tests the PushFront method for a non empty empty Valuelist with one element.
 */
TEST(Valuelist, PushFrontNonEmptyListOneElement)
{
  constexpr std::size_t newElement = 975UL;
  using myList         = Valuelist<std::size_t, 1UL>;
  using myExtendedList = Valuelist<std::size_t, newElement, 1UL>;
  EXPECT_TRUE((std::is_same_v<valuelist::PushFront<myList, newElement>, myExtendedList>));
}

/**
 * @brief  tests the PushFront method for a non empty empty Valuelist.
 */
TEST(Valuelist, PushFrontNonEmptyList)
{
  constexpr int newElement = 69;
  using myList             = Valuelist<int, 1, 2, 3>;
  using myExtendedList     = Valuelist<int, newElement, 1, 2, 3>;
  EXPECT_TRUE((std::is_same_v<valuelist::PushFront<myList, newElement>, myExtendedList>));
}

/**
 * @brief  tests the PopFront helper for Valuelist.
 */
TEST(Valuelist, PopFront)
{
  using myList       = Valuelist<int, 13, 21, 78>;
  using myPoppedList = Valuelist<int, 21, 78>;
  EXPECT_TRUE((std::is_same_v<valuelist::PopFront<myList>, myPoppedList>));
}

/**
 * @brief  tests the PopFront helper for Valuelist which is empty after the pop operation.
 */
TEST(Valuelist, PopFrontEmptyAfterPop)
{
  using myList       = Valuelist<int, 13>;
  using myPoppedList = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::PopFront<myList>, myPoppedList>));
}

/**
 * @brief  tests the PushBack method for an empty Valuelist.
 */
TEST(Valuelist, PushBackEmptyList)
{
  constexpr int newElement = 469;
  using myList             = Valuelist<int>;
  using myExtendedList     = Valuelist<int, newElement>;
  EXPECT_TRUE((std::is_same_v<valuelist::PushBack<myList, newElement>, myExtendedList>));
}

/**
 * @brief  tests the PushFront method for a non empty empty Valuelist.
 */
TEST(Valuelist, PushBackNonEmptyList)
{
  constexpr int newElement = -10469;
  using myList             = Valuelist<int, 1, 2, 3>;
  using myExtendedList     = Valuelist<int, 1, 2, 3, newElement>;
  EXPECT_TRUE((std::is_same_v<valuelist::PushBack<myList, newElement>, myExtendedList>));
}
 
/**
 * @brief  tests the NthElement helper for Valuelist.
 */
TEST(Valuelist, NthElementFirstElement)
{
  using myList            = Valuelist<int, 11, -2, 311>;
  constexpr int myNthType = valuelist::NthElement<myList, 0>;
  EXPECT_EQ(myNthType, 11);
}

/**
 * @brief  tests the NthElement helper for Valuelist.
 */
TEST(Valuelist, NthElement)
{
  using myList            = Valuelist<int, 11, -2, 311>;
  constexpr int myNthType = valuelist::NthElement<myList, 1>;
  EXPECT_EQ(myNthType, -2);
}

/**
 * @brief  tests the NthElement helper for Valuelist.
 */
TEST(Valuelist, NthElementLastElement)
{
  using myList            = Valuelist<int, 11, -2, 311>;
  constexpr int myNthType = valuelist::NthElement<myList, 2>;
  EXPECT_EQ(myNthType, 311);
}

/**
 * @brief  tests the HasValue helper for a non empty Valuelist.
 */
TEST(Valuelist, HasValueNonEmptyListTrue)
{
  using myList     = Valuelist<int, 11, -2, 311>;
  constexpr bool b = valuelist::HasValue<myList, 311>;
  EXPECT_TRUE(b);
}

/**
 * @brief  tests the HasValue helper for a non empty Valuelist.
 */
TEST(Valuelist, HasValueNonEmptyListFalse)
{
  using myList     = Valuelist<int, 11, -2, 311>;
  constexpr bool b = valuelist::HasValue<myList, 21>;
  EXPECT_FALSE(b);
}

/**
 * @brief  tests the HasValue helper for an empty Valuelist.
 */
TEST(Valuelist, HasValueEmptyList)
{
  using myList     = Valuelist<int>;
  constexpr bool b = valuelist::HasValue<myList, 21>;
  EXPECT_FALSE(b);
}

template <int Value>
struct HasValueLarger3 : std::conditional_t<(Value > 3), std::true_type, std::false_type> {};

/**
 *  @brief  tests the HasValueWithProperty helper for an empty Typelist.
 */
TEST(Valuelist, HasValueWithPropertyEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_FALSE((valuelist::HasValueWithProperty<myList, HasValueLarger3>));
}

/**
 * @brief  tests the HasValueWithProperty helper for a non empty Valuelist.
 */
TEST(Valuelist, HasValueWithPropertyNonEmptyListTrue)
{
  using myList = Valuelist<int, -11, -2, 4>;
  EXPECT_TRUE((valuelist::HasValueWithProperty<myList, HasValueLarger3>));
}

/**
 * @brief  tests the HasValueWithProperty helper for a non empty Typelist.
 */
TEST(Valuelist, HasValueWithPropertyNonEmptyListFalse)
{
  using myList = Valuelist<int, 1, -2, 1>;
  EXPECT_FALSE((valuelist::HasValueWithProperty<myList, HasValueLarger3>));
}

/**
 *  @brief  tests the FindIf helper for an empty Valuelist.
 */
TEST(Valuelist, FindIfEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_EQ((valuelist::FindIf<myList, HasValueLarger3>), -1);
}

/**
 *  @brief  tests the FindIf helper for an non-empty Valuelist.
 */
TEST(Valuelist, FindIfNonEmptyListTrue)
{
  using myList = Valuelist<int, 1, 4, -12>;
  EXPECT_EQ((valuelist::FindIf<myList, HasValueLarger3>), 1);
}

/**
 *  @brief  tests the FindIf helper for an non-empty Valuelist.
 */
TEST(Valuelist, FindIfNonEmptyListFalse)
{
  using myList = Valuelist<int, 1, 0, -12>;
  EXPECT_EQ((valuelist::FindIf<myList, HasValueLarger3>), -1);
}

/**
 *  @brief  tests the CountIf helper for an empty Valuelist.
 */
TEST(Valuelist, CountIfEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_EQ((valuelist::CountIf<myList, HasValueLarger3>), 0UL);
}

/**
 *  @brief  tests the CountIf helper for Valuelist.
 */
TEST(Valuelist, CountIfNonEmptyListTrue)
{
  using myList = Valuelist<int, 1, 24, 12, -13, 54, 0>;
  EXPECT_EQ((valuelist::CountIf<myList, HasValueLarger3>), 3UL);
}

/**
 *  @brief  tests the CountIf helper for Valuelist.
 */
TEST(Valuelist, CountIfNonEmptyListFalse)
{
  using myList = Valuelist<int, 1, 2, -12, -13, -54, 0>;
  EXPECT_EQ((valuelist::CountIf<myList, HasValueLarger3>), 0UL);
}

/**
 *  @brief  tests the Index helper for an empty Valuelist.
 */
TEST(Valuelist, IndexEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_EQ((valuelist::IndexOf<myList, 1>), -1);
}

/**
 *  @brief  tests the Index helper for an empty Valuelist.
 */
TEST(Valuelist, IndexNonEmptyListTrue)
{
  using myList = Valuelist<int, 0, -14, 24, 14, 1>;
  EXPECT_EQ((valuelist::IndexOf<myList, 1>), 4);
}

/**
 *  @brief  tests the Remove helper for a non empty Valuelist.
 */
TEST(Valuelist, RemoveNonEmptyListFirstElement)
{
  constexpr std::size_t index = 0UL;
  using myList                = Valuelist<int, 0, -14, 24, 14, 1>;
  using myRemovedList         = Valuelist<int, -14, 24, 14, 1>;
  EXPECT_TRUE((std::is_same_v<valuelist::Remove<myList, index>, myRemovedList>));
}

/**
 *  @brief  tests the Remove helper for a non empty Valuelist.
 */
TEST(Valuelist, RemoveNonEmptyListLastElement)
{
  constexpr std::size_t index = 4UL;
  using myList                = Valuelist<int, 0, -14, 24, 14, 1>;
  using myRemovedList         = Valuelist<int, 0, -14, 24, 14>;
  EXPECT_TRUE((std::is_same_v<valuelist::Remove<myList, index>, myRemovedList>));
}

/**
 *  @brief  tests the Remove helper for a non empty Valuelist.
 */
TEST(Valuelist, RemoveNonEmptyList)
{
  constexpr std::size_t index = 2;
  using myList                = Valuelist<int, 0, -14, 24, 14, 1>;
  using myRemovedList         = Valuelist<int, 0, -14, 14, 1>;
  EXPECT_TRUE((std::is_same_v<valuelist::Remove<myList, index>, myRemovedList>));
}

/**
 *  @brief  tests the Insert helper for a non empty Valuelist.
 */
TEST(Valuelist, InsertNonEmptyAtFront)
{
  constexpr std::size_t index  = 0UL;
  constexpr int         newVal = 134;
  using myList                 = Valuelist<int, 0, -14, 24, 14, 1>;
  using myNewList              = Valuelist<int, newVal, 0, -14, 24, 14, 1>;
  EXPECT_TRUE((std::is_same_v<valuelist::Insert<myList, newVal, index>, myNewList>));
}

/**
 *  @brief  tests the Insert helper for a non empty Valuelist.
 */
TEST(Valuelist, InsertNonEmptyInMiddle)
{
  constexpr std::size_t index  = 2UL;
  constexpr int         newVal = 134;
  using myList                 = Valuelist<int, 0, -14, 24, 14, 1>;
  using myNewList              = Valuelist<int, 0, -14, newVal, 24, 14, 1>;
  EXPECT_TRUE((std::is_same_v<valuelist::Insert<myList, newVal, index>, myNewList>));
}

/**
 *  @brief  tests the Insert helper for a non empty Valuelist.
 */
TEST(Valuelist, InsertNonEmptyAtEnd)
{
  constexpr std::size_t index  = 5UL;
  constexpr int         newVal = 134;
  using myList                 = Valuelist<int, 0, -14, 24, 14, 1>;
  using myNewList              = Valuelist<int, 0, -14, 24, 14, 1, newVal>;
  EXPECT_TRUE((std::is_same_v<valuelist::Insert<myList, newVal, index>, myNewList>));
}

/**
 *  @brief  tests the Replace helper for a non empty Valuelist.
 */
TEST(Valuelist, ReplaceNonEmptyAtFront)
{
  constexpr std::size_t index  = 0UL;
  constexpr int         newVal = 134;
  using myList                 = Valuelist<int, 0, -14, 24, 14, 1>;
  using myNewList              = Valuelist<int, newVal, -14, 24, 14, 1>;
  EXPECT_TRUE((std::is_same_v<valuelist::Replace<myList, newVal, index>, myNewList>));
}

/**
 *  @brief  tests the Insert helper for a non empty Valuelist.
 */
TEST(Valuelist, ReplaceNonEmptyInMiddle)
{
  constexpr std::size_t index  = 2UL;
  constexpr int         newVal = 134;
  using myList                 = Valuelist<int, 0, -14, 24, 14, 1>;
  using myNewList              = Valuelist<int, 0, -14, newVal, 14, 1>;
  EXPECT_TRUE((std::is_same_v<valuelist::Replace<myList, newVal, index>, myNewList>));
}

/**
 *  @brief  tests the Insert helper for a non empty Valuelist.
 */
TEST(Valuelist, ReplaceNonEmptyAtEnd)
{
  constexpr std::size_t index  = 4UL;
  constexpr int         newVal = 134;
  using myList                 = Valuelist<int, 0, -14, 24, 14, 1>;
  using myNewList              = Valuelist<int, 0, -14, 24, 14, newVal>;
  EXPECT_TRUE((std::is_same_v<valuelist::Replace<myList, newVal, index>, myNewList>));
}

template <int I1, int I2>
struct IntGreaterThan : std::conditional_t<(I1 >= I2), std::true_type, std::false_type> {};

template <int I1, int I2>
struct IntSmallerThan : std::conditional_t<(I1 < I2), std::true_type, std::false_type> {};

/**
 *  @brief  tests the MaxElement helper for an empty Valuelist.
 */
TEST(Valuelist, MaxElementEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_EQ((valuelist::MaxElement<myList, IntGreaterThan>), -1);
}

/**
 *  @brief  tests the MaxElement helper for a Valuelist with one element.
 */
TEST(Valuelist, MaxElementNonEmptyListFirstElement)
{
  using myList = Valuelist<int, 1>;
  EXPECT_EQ((valuelist::MaxElement<myList, IntGreaterThan>), 0);
}

/**
 *  @brief  tests the MaxElement helper for a non empty Valuelist
 *          with its largets element in the middle.
 */
TEST(Valuelist, MaxElementNonEmptyListElementInTheMiddle)
{
  using myList = Valuelist<int, 1, 34, 945, -12, 0>;
  EXPECT_EQ((valuelist::MaxElement<myList, IntGreaterThan>), 2);
}

/**
 *  @brief  tests the MaxElement helper for a non empty Valuelist
 *          with its largets element at the end.
 */
TEST(Valuelist, MaxElementNonEmptyListElementAtTheEnd)
{
  using myList = Valuelist<int, 1, 34, 945, -12, 0, 1234>;
  EXPECT_EQ((valuelist::MaxElement<myList, IntGreaterThan>), 5);
}

/**
 *  @brief  tests the Revers method for an empty Valuelist.
 */
TEST(ValuelistTest, ReverseEmptyList)
{
  using myList = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::Reverse<myList>, myList>));
}

/**
 *  @brief  tests the Revers method for a Valuelist with one element.
 */
TEST(ValuelistTest, ReverseListOneElement)
{
  using myList = Valuelist<int, 123>;
  EXPECT_TRUE((std::is_same_v<valuelist::Reverse<myList>, myList>));
}

/**
 *  @brief  tests the Revers method for a Valuelist with more than one element.
 */
TEST(ValuelistTest, ReverseListMoreElements)
{
  using myList        = Valuelist<int, 123, -3, 78, 1023, 1, 1, 0>;
  using myReverseList = Valuelist<int, 0, 1, 1, 1023, 78, -3, 123>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<valuelist::Reverse<myList>, myReverseList>));
}


template <int N>
struct Square : std::integral_constant<int, N * N> {};

/**
 *  @brief  tests the Transform method for an empty Valuelist.
 */
TEST(Valuelist, TransformEmptyList)
{
  using myList            = Valuelist<int>;
  using myTransformedList = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::Transform<myList, Square>, myTransformedList>));
}

/**
 *  @brief  tests the Transform method for a Valuelist with one element.
 */
TEST(Valuelist, TransformListOneElement)
{
  using myList            = Valuelist<int, 12>;
  using myTransformedList = Valuelist<int, 144>;
  EXPECT_TRUE((std::is_same_v<valuelist::Transform<myList, Square>, myTransformedList>));
}

/**
 *  @brief  tests the Transform method for a Valuelist with more than one element.
 */
TEST(Valuelist, TransformListMoreElements)
{
  using myList            = Valuelist<int, 1, 2, 3, 4, 5>;
  using myTransformedList = Valuelist<int, 1, 4, 9, 16, 25>;
  EXPECT_TRUE((std::is_same_v<valuelist::Transform<myList, Square>, myTransformedList>));
}

template <int I1, int I2>
struct AddInts {
  using type = int;
  static constexpr int value = I1 + I2;
};

/**
 *  @brief  tests the Transform method for two empty Valuelist.
 */
TEST(Valuelist, TransformBinaryEmptyLists)
{
  using myList1           = Valuelist<int>;
  using myList2           = Valuelist<int>;
  using myTransformedList = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::TransformBinary<myList1, myList2, AddInts>, myTransformedList>));
}

/**
 *  @brief  tests the Transform method for two Valuelist with one element.
 */
TEST(Valuelist, TransformBinaryListOneElement)
{
  using myList1           = Valuelist<int, 12>;
  using myList2           = Valuelist<int, 13>;
  using myTransformedList = Valuelist<int, 25>;
  EXPECT_TRUE((std::is_same_v<valuelist::TransformBinary<myList1, myList2, AddInts>, myTransformedList>));
}

/**
 *  @brief  tests the Transform method for Valuelists with more elements.
 */
TEST(Valuelist, TransformBinaryDifferentLengths)
{
  using myList1           = Valuelist<int, 12, 2, 23>;
  using myList2           = Valuelist<int, 13, 19, -12, 123>;
  using myTransformedList = Valuelist<int, 25, 21, 11>;
  EXPECT_TRUE((std::is_same_v<valuelist::TransformBinary<myList1, myList2, AddInts>, myTransformedList>));
}

/**
 *  @brief  tests the Accumulate method for an empty Valuelist with no bias.
 */
TEST(Valuelist, AccumulateEmptyListNoBias)
{
  using myList = Valuelist<int>;
  EXPECT_EQ((valuelist::Accumulate<myList, AddInts>), 0);
}

/**
 *  @brief  tests the Accumulate method for an empty Valuelist with no bias.
 */
TEST(Valuelist, AccumulateEmptyListBias)
{
  using myList       = Valuelist<int>;
  constexpr int bias = 23;
  EXPECT_EQ((valuelist::Accumulate<myList, AddInts, bias>), bias);
}

/**
 *  @brief  tests the Accumulate method for a non empty Valuelist.
 */
TEST(Valuelist, AccumulateNonEmptyListNoBias)
{
  using myList = Valuelist<int, 12, 13, 165, 1>;
  EXPECT_EQ((valuelist::Accumulate<myList, AddInts>), 191);
}

/**
 *  @brief  tests the Accumulate method for a non empty Valuelist.
 */
TEST(Valuelist, AccumulateNonEmptyListBias)
{
  using myList       = Valuelist<int, 12, 13, 165, 1>;
  constexpr int bias = 23;
  EXPECT_EQ((valuelist::Accumulate<myList, AddInts, bias>), 191 + bias);
}

/**
 *  @brief  tests the InsertSorted method for a non empty Valuelist.
 */
TEST(ValuelistTest, InsertSortedNonEmpty)
{
  using myList           = Valuelist<int, 1, 4, 6, 9>;
  constexpr int newValue = 5;
  using myExpandedList   = Valuelist<int, 1, 4, newValue, 6, 9>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<valuelist::InsertSorted<myList, newValue, IntSmallerThan>, myExpandedList>));
}

/**
 *  @brief  tests the InsertionSort method for an empty Valuelist.
 */
TEST(Valuelist, InsertionSortEmpty)
{
  using myList = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::InsertionSort<myList, IntSmallerThan>, myList>));
}

/**
 *  @brief  tests the InsertionSort method for a non empty Valuelist.
 */
TEST(Valuelist, InsertionSortNonEmptyOneElement)
{
  using myList = Valuelist<int, 11>;
  EXPECT_TRUE((std::is_same_v<valuelist::InsertionSort<myList, IntSmallerThan>, myList>));
}

/**
 *  @brief  tests the InsertionSort method for a non empty Valuelist.
 */
TEST(Valuelist, InsertionSortNonEmpty)
{
  using myList       = Valuelist<int, 11, 4, 2, 9>;
  using mySortedList = Valuelist<int, 2, 4, 9, 11>;
  EXPECT_TRUE((std::is_same_v<valuelist::InsertionSort<myList, IntSmallerThan>, mySortedList>));
}

template <int N>
struct IsOddInt : std::conditional_t<(N % 2 == 1), std::true_type, std::false_type> {};

/**
 *  @brief  tests the ElementsWithPropertyNonEmpty method for a non empty Valuelist with one element.
 */
TEST(Valuelist, ElementsWithPropertyOneElementTrue)
{
  using myList     = Valuelist<int, 11>;
  using myElements = Valuelist<int, 11>;
  EXPECT_TRUE((std::is_same_v<valuelist::ElementsWithProperty<myList, IsOddInt>, myElements>));
}

/**
 *  @brief  tests the ElementsWithPropertyNonEmpty method for a non empty Valuelist with one element.
 */
TEST(Valuelist, ElementsWithPropertyOneElementFalse)
{
  using myList     = Valuelist<int, 12>;
  using myElements = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::ElementsWithProperty<myList, IsOddInt>, myElements>));
}

/**
 *  @brief  tests the ElementsWithPropertyNonEmpty method for a non empty Valuelist.
 */
TEST(Valuelist, ElementsWithPropertyNonEmpty)
{
  using myList     = Valuelist<int, 11, 4, 2, 9>;
  using myElements = Valuelist<int, 11, 9>;
  EXPECT_TRUE((std::is_same_v<valuelist::ElementsWithProperty<myList, IsOddInt>, myElements>));
}

template <int N>
struct IntGreaterThanThree : std::conditional_t<(N > 3), std::true_type, std::false_type> {};

/**
 *  @brief  tests the ElementsWithPropertyNonEmpty method for a non empty Valuelist.
 */
TEST(Valuelist, ElementsWithPropertyNonEmptyGreaterThanThree)
{
  using myList     = Valuelist<int, 11, 4, 2, 9>;
  using myElements = Valuelist<int, 11, 4, 9>;
  EXPECT_TRUE((std::is_same_v<valuelist::ElementsWithProperty<myList, IntGreaterThanThree>, myElements>));
}

/**
 *  @brief  tests the ElementsWithPropertyNonEmpty method for a non empty Valuelist.
 */
TEST(Valuelist, ElementsWithPropertyNonEmptyGreaterThanThreeNoElements)
{
  using myList     = Valuelist<int, -11, -4, 2, -9>;
  using myElements = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::ElementsWithProperty<myList, IntGreaterThanThree>, myElements>));
}

/**
 *  @brief  tests the AllIndicesOf method for an empty Valuelist.
 */
TEST(Valuelist, AllIndicesOfEmptyGreaterThanThreeNoElements)
{
  using myList    = Valuelist<int>;
  using myIndices = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::AllIndicesOf<myList, IntGreaterThanThree>, myIndices>));
}

/**
 *  @brief  tests the AllIndicesOf method for a non empty Valuelist.
 */
TEST(Valuelist, AllIndicesOfNonEmptyGreaterThanThreeOneElement)
{
  using myList    = Valuelist<int, 11>;
  using myIndices = Valuelist<int, 0>;
  EXPECT_TRUE((std::is_same_v<valuelist::AllIndicesOf<myList, IntGreaterThanThree>, myIndices>));
}

/**
 *  @brief  tests the AllIndicesOf method for a non empty Valuelist.
 */
TEST(Valuelist, AllIndicesOfNonEmptyGreaterThanThree)
{
  using myList    = Valuelist<int, 11, -4, 12, -9>;
  using myIndices = Valuelist<int, 0, 2>;
  EXPECT_TRUE((std::is_same_v<valuelist::AllIndicesOf<myList, IntGreaterThanThree>, myIndices>));
}

/**
 *  @brief  tests the AllIndicesOf method for a non empty Valuelist.
 */
TEST(Valuelist, AllIndicesOfNonEmptyGreaterThanThreeNoElements)
{
  using myList    = Valuelist<int, 2, -4, -12, -9>;
  using myIndices = Valuelist<int>;
  EXPECT_TRUE((std::is_same_v<valuelist::AllIndicesOf<myList, IntGreaterThanThree>, myIndices>));
}

template <typename T>
struct SizeOfType {
  using type = std::size_t;
  static constexpr type value = sizeof(T);
};

TEST(Valuelist, TypeToValuelistNonEmptyOneElement)
{
  using myTypes     = meta::Typelist<char>;
  using myTypeSizes = Valuelist<typename SizeOfType<char>::type, SizeOfType<char>::value>;
  EXPECT_TRUE((std::is_same_v<valuelist::TypeToValuelist<myTypes, SizeOfType>, myTypeSizes>));
}

TEST(Valuelist, TypeToValuelistNonEmpty)
{
  using myTypes     = meta::Typelist<char, int>;
  using myTypeSizes = Valuelist</*typename SizeOfType<char>::type*/std::decay_t<decltype(SizeOfType<char>::value)>,
                                           SizeOfType<char>::value,
                                           SizeOfType<int>::value>;
  EXPECT_TRUE((std::is_same_v<valuelist::TypeToValuelist<myTypes, SizeOfType>, myTypeSizes>));
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // VALUELISTTEST_H_28385142461954299802351053518262318072436 
