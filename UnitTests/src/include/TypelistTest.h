/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TypelistTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    24.06.2020 
 */ 
 
#ifndef TYPELISTTEST_H_32369310941833031878104711730140601259310821 
#define TYPELISTTEST_H_32369310941833031878104711730140601259310821 
 
 
// includes
#include <Meta/Typelist.h>

#include <string>
 
 
using namespace meta;


/**
 * @brief  tests the IsEmpty method for an empty Typelist.
 */
TEST(Typelist, IsEmptyEmptyList)
{
  using myList = Typelist<>;
  EXPECT_TRUE(typelist::IsEmpty<myList>);
}

/**
 * @brief  tests the IsEmpty method for an empty Typelist.
 */
TEST(Typelist, IsEmptyNonEmptyList)
{
  using myList = Typelist<double, int>;
  EXPECT_FALSE(typelist::IsEmpty<myList>);
}

/**
 * @brief  tests the size method for an empty Typelist.
 */
TEST(Typelist, SizeEmptyList)
{
  using myList = Typelist<>;
  EXPECT_EQ(typelist::Size<myList>, 0UL);
}

/**
 * @brief  tests the size method for a non-empty Typelist.
 */
TEST(Typelist, SizeNonEmptyList)
{
  using myList = Typelist<double, int, short>;
  EXPECT_EQ(typelist::Size<myList>, 3UL);
}

/**
 * @brief  tests the Front helper for Typelist.
 */
TEST(Typelist, TypelistFront)
{
  using myList    = Typelist<double, int, short>;
  using frontType = typelist::Front<myList>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<frontType, double>));
}

/**
 * @brief  tests the PopFront helper for Typelist.
 */
TEST(Typelist, PopFront)
{
  using myList       = Typelist<double, int, short>;
  using myPoppedList = Typelist<int, short>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<typelist::PopFront<myList>, myPoppedList>));
}

/**
 * @brief  tests the PopFront helper for Typelist which is empty after the pop operation.
 */
TEST(Typelist, PopFrontEmptyAfterPop)
{
  using myList       = Typelist<double>;
  using myPoppedList = Typelist<>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<typelist::PopFront<myList>, myPoppedList>));
}

/**
 * @brief  tests the PushFront helper for an empty Typelist.
 */
TEST(Typelist, PushFrontEmptyList)
{
  using myList         = Typelist<>;
  using myExpandedList = Typelist<unsigned long long>;

  EXPECT_TRUE((std::is_same_v<typelist::PushFront<myList, unsigned long long>, myExpandedList>));
}

/**
 * @brief  tests the PushFront helper for a non empty Typelist.
 */
TEST(Typelist, PushFrontNonEmptyList)
{
  using myList         = Typelist<double, int, short>;
  using myExpandedList = Typelist<unsigned long long, double, int, short>;

  EXPECT_TRUE((std::is_same_v<typelist::PushFront<myList, unsigned long long>, myExpandedList>));
}

/**
 * @brief  tests the Back helper for Typelist.
 */
TEST(Typelist, BackNonEmptyList)
{
  using myList   = Typelist<double, int, short>;
  using backType = typelist::Back<myList>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<backType, short>));
}

/**
 * @brief  tests the Back helper for Typelist.
 */
TEST(Typelist, BackEmptyList)
{
  using myList   = Typelist<>;
  using backType = typelist::Back<myList>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<backType, InvalidType>));
}

/**
 *  @brief  tests the PushBack helper for Typelist.
 */
TEST(Typelist, PushBackNonEmptyList)
{
  using myList         = Typelist<double, int, short>;
  using myExpandedList = Typelist<double, int, short, unsigned long long>;

  EXPECT_TRUE((std::is_same_v<typelist::PushBack<myList, unsigned long long>, myExpandedList>));
}

/**
 * @brief  tests the PushBack helper for Typelist.
 */
TEST(Typelist, PushBackEmptyList)
{
  using myList         = Typelist<>;
  using myExpandedList = Typelist<char>;

  EXPECT_TRUE((std::is_same_v<typelist::PushBack<myList, char>, myExpandedList>));
}

/**
 * @brief  tests the PopBack helper for Typelist.
 */
TEST(Typelist, PopBack)
{
  using myList       = Typelist<double, int, short>;
  using myPoppedList = Typelist<double, int>;

  EXPECT_TRUE((std::is_same_v<typelist::PopBack<myList>, myPoppedList>));
}

/**
 * @brief  tests the PopFront helper for Typelist which is empty after the pop operation.
 */
TEST(Typelist, PopBackEmptyAfterPop)
{
  using myList       = Typelist<double>;
  using myPoppedList = Typelist<>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<typelist::PopBack<myList>, myPoppedList>));
}

/**
 * @brief  tests the PopFront helper for Typelist which is empty.
 */
TEST(Typelist, PopBackEmptyList)
{
  using myList       = Typelist<>;
  using myPoppedList = InvalidType;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<typelist::PopBack<myList>, myPoppedList>));
}

/**
 *  @brief  tests the Concat method for two empty Typelists.
 */
TEST(Typelist, ConcatBothListsEmpty)
{
  using List1    = Typelist<>;
  using List2    = Typelist<>;
  using Expected = Typelist<>;

  // double brackets are necessary here
  EXPECT_TRUE((std::is_same_v<typelist::Concat<List1, List2>, Expected>));
}

/**
 * @brief  tests the Concat method for two Typelists with one list empty.
 */
TEST(Typelist, ConcatList1Empty)
{
  using List1    = Typelist<>;
  using List2    = Typelist<long, char>;
  using Expected = Typelist<long, char>;

  EXPECT_TRUE((std::is_same_v<typelist::Concat<List1, List2>, Expected>));
}

/**
 * @brief  tests the Reverse Concat for two Typelists.
 */
TEST(Typelist, ConcatList2Empty)
{
  using List1    = Typelist<int, double>;
  using List2    = Typelist<>;
  using Expected = Typelist<int, double>;

  EXPECT_TRUE((std::is_same_v<typelist::Concat<List1, List2>, Expected>));
}

/**
 * @brief  tests the Concat method for two non empty Typelists.
 */
TEST(Typelist, ConcatBothListsNonEmpty)
{
  using List1    = Typelist<int, double>;
  using List2    = Typelist<long, char>;
  using Expected = Typelist<int, double, long, char>;
  EXPECT_TRUE((std::is_same_v<typelist::Concat<List1, List2>, Expected>));
}

/**
 * @brief  tests the NthElement helper for Typelist.
 */
TEST(Typelist, NthElementNonEmptyListValidIndex)
{
  using myList    = Typelist<double, int, short, unsigned int>;
  using myNthType = typelist::NthElement<myList, 1>;

  EXPECT_TRUE((std::is_same_v<myNthType, int>));
}

/**
 * @brief  tests the NthElement helper for Typelist.
 */
TEST(Typelist, NthElementNonEmptyListInvalidIndex)
{
  using myList    = Typelist<double, int, short, unsigned int>;
  using myNthType = typelist::NthElement<myList, 4>;

  EXPECT_TRUE((std::is_same_v<myNthType, InvalidType>));
}

/**
 * @brief  tests the Reverse method for non empty Typelist.
 */
TEST(Typelist, ReverseNonEmptyList)
{
  using myList        = Typelist<int, std::string, double, double>;
  using myReverseList = Typelist<double, double, std::string, int>;

  EXPECT_TRUE((std::is_same_v<typelist::Reverse<myList>, myReverseList>));
}

/**
 * @brief  tests the Reverse method for an empty Typelist.
 */
TEST(Typelist, ReverseEmptyList)
{
  using myList = Typelist<>;
  EXPECT_TRUE((std::is_same_v<typelist::Reverse<myList>, myList>));
}

/**
 * @brief  tests the Reverse method for a Typelist with one element.
 */
TEST(Typelist, ReverseOneElement)
{
  using myList = Typelist<int>;
  EXPECT_TRUE((std::is_same_v<typelist::Reverse<myList>, myList>));
}

/**
 * @brief  tests the Reverse method for a Typelist with one element.
 */
TEST(Typelist, ReverseTwoElement)
{
  using myList        = Typelist<int, double>;
  using myReverseList = Typelist<double, int>;

  EXPECT_TRUE((std::is_same_v<typelist::Reverse<myList>, myReverseList>));
}

/**
 * @brief  tests the HasType helper for Typelist.
 */
TEST(Typelist, HasTypeAvailableType)
{
  using myList = Typelist<double, int, short, unsigned int>;
  EXPECT_TRUE((typelist::HasType<myList, short>));
}

/**
 * @brief  tests the HasType helper for Typelist.
 */
TEST(Typelist, HasTypeNonAvailableType)
{
  using myList = Typelist<double, int, short, unsigned int>;
  EXPECT_FALSE((typelist::HasType<myList, char>));
}

/**
 * @brief  tests the HasType helper for Typelist.
 */
TEST(Typelist, HasTypeEmptyList)
{
  using myList = Typelist<>;
  EXPECT_FALSE((typelist::HasType<myList, char>));
}

template <typename T>
struct HasTypeSizeOne : std::conditional_t<sizeof(T) == 1, std::true_type, std::false_type> {};

/**
 * @brief  tests the HasTypeWithPropertyT helper for Typelist.
 */
TEST(Typelist, HasTypeWithPropertyNonEmptyListTrue)
{
  using myList = Typelist<double, int, char, unsigned int>;
  EXPECT_TRUE((typelist::HasTypeWithProperty<myList, HasTypeSizeOne>));
}

/**
 * @brief  tests the HasTypeWithPropertyT helper for Typelist.
 */
TEST(Typelist, HasTypeWithPropertyNonEmptyListFalse)
{
  using myList = Typelist<double, int, long long, unsigned int>;
  EXPECT_FALSE((typelist::HasTypeWithProperty<myList, HasTypeSizeOne>));
}

/**
 * @brief  tests the HasTypeWithPropertyT helper for Typelist.
 */
TEST(Typelist, HasTypeWithPropertyEmptyList)
{
  using myList = Typelist<>;
  EXPECT_FALSE((typelist::HasTypeWithProperty<myList, HasTypeSizeOne>));
}

/**
 * @brief  tests the FindIf helper for Typelist.
 */
TEST(Typelist, FindIfNonEmptyListTrue)
{
  using myList = Typelist<double, int, char, unsigned int>;
  EXPECT_EQ((typelist::FindIf<myList, HasTypeSizeOne>), 2);
}

/**
 * @brief  tests the FindIf helper for Typelist.
 */
TEST(Typelist, FindIfNonEmptyListFalse)
{
  using myList = Typelist<double, int, long long, unsigned int>;
  EXPECT_EQ((typelist::FindIf<myList, HasTypeSizeOne>), -1);
}

/**
 * @brief  tests the FindIf helper for Typelist.
 */
TEST(Typelist, FindIfEmptyList)
{
  using myList = Typelist<>;
  EXPECT_EQ((typelist::FindIf<myList, HasTypeSizeOne>), -1);
}

/**
 * @brief  tests the CountIf helper for Typelist.
 */
TEST(Typelist, CountIfNonEmptyListTrue)
{
  using myList = Typelist<double, int, char, unsigned int, char, bool>;
  EXPECT_EQ((typelist::CountIf<myList, HasTypeSizeOne>), 3UL);
}

/**
 * @brief  tests the CountIf helper for Typelist.
 */
TEST(Typelist, CountIfNonEmptyListFalse)
{
  using myList = Typelist<double, int, double, unsigned int, long long>;
  EXPECT_EQ((typelist::CountIf<myList, HasTypeSizeOne>), 0UL);
}

/**
 * @brief  tests the CountIf helper for an empty Typelist.
 */
TEST(Typelist, CountIfEmptyList)
{
  using myList = Typelist<>;
  EXPECT_EQ((typelist::CountIf<myList, HasTypeSizeOne>), 0UL);
}

/**
 * @brief  tests the CountIf helper for an empty Typelist.
 */
TEST(Typelist, AllOfExpectTrue)
{
  using myList = Typelist<char, unsigned char>;
  EXPECT_TRUE((typelist::AllOf<myList, HasTypeSizeOne>));
}

/**
 * @brief  tests the CountIf helper for an empty Typelist.
 */
TEST(Typelist, AllOfExpectFalse)
{
  using myList = Typelist<char, unsigned char, double>;
  EXPECT_FALSE((typelist::AllOf<myList, HasTypeSizeOne>));
}

/**
 * @brief  tests the CountIf helper for an empty Typelist.
 */
TEST(Typelist, AllOfExpectTrueEmptyList)
{
  using myList = Typelist<>;
  EXPECT_TRUE((typelist::AllOf<myList, HasTypeSizeOne>));
}

/**
 *  @brief  tests the IndexOf method for an available type.
 */
TEST(Typelist, TypelistIndexOfAvailableType)
{
  using myList = Typelist<double, int, short, unsigned int>;
  constexpr auto index = typelist::IndexOf<myList, short>;
  EXPECT_EQ(index, 2);
}

/**
 * @brief  tests the IndexOf method for a non-available type.
 */
TEST(Typelist, TypelistIndexOfNonAvailableType)
{
  using myList = Typelist<double, int, short, int>;
  constexpr auto index = typelist::IndexOf<myList, long>;
  EXPECT_EQ(index, -1);
}

/**
 * @brief  tests the IndexOf method for an empty Typelist.
 */
TEST(Typelist, IndexOfEmptyList)
{
  using myList = Typelist<>;
  constexpr auto index = typelist::IndexOf<myList, int>;
  EXPECT_EQ(index, -1);
}

/**
 * @brief  tests the IndexOf method for a Typelist with one element.
 */
TEST(Typelist, IndexOfListWithOneElement)
{
  using myList = Typelist<int>;
  constexpr auto index = typelist::IndexOf<myList, int>;
  EXPECT_EQ(index, 0);
}

template <typename T, typename S>
struct GreaterThan : std::conditional_t<(sizeof(T) >= sizeof(S)), std::true_type, std::false_type> {};

/**
 *  @brief  tests the MaxElement method for an empty Typelist.
 */
TEST(Typelist, MaxElementEmptyList)
{
  using myList = Typelist<>;
  EXPECT_EQ((typelist::MaxElement<myList, GreaterThan>), -1);
}

/**
 *  @brief  tests the MaxElement method for a Typelist with one element.
 */
TEST(Typelist, MaxElementNonEmptyListOneElement)
{
  using myList = Typelist<unsigned short>;
  EXPECT_EQ((typelist::MaxElement<myList, GreaterThan>), 0);
}

/**
 *  @brief  tests the MaxElement method for a non empty Typelist.
 */
TEST(Typelist, MaxElementNonEmptyListFirstElement)
{
  using myList = Typelist<double, int, short, unsigned int, long long>;
  EXPECT_EQ((typelist::MaxElement<myList, GreaterThan>), 0);
}

/**
 * @brief  tests the MaxElement method for a non empty Typelist.
 */
TEST(Typelist, MaxElementNonEmptyListElementInTheMiddle)
{
  using myList = Typelist<int, short, double, unsigned int, long long>;
  EXPECT_EQ((typelist::MaxElement<myList, GreaterThan>), 2);
}

/**
 * @brief  tests the MaxElement method for a non empty Typelist.
 */
TEST(Typelist, MaxElementNonEmptyListElementAtEnd)
{
  using myList = Typelist<char, short, int, unsigned int, double>;
  EXPECT_EQ((typelist::MaxElement<myList, GreaterThan>), 4);
}

/**
 *  @brief  tests the Transform method for a Typelist with one element.
 */
TEST(Typelist, TransformNonEmptyListAddConstOneElement)
{
  using myList      = Typelist<int>;
  using myConstList = Typelist<const int>;
  EXPECT_TRUE((std::is_same_v<typelist::Transform<myList, std::add_const>, myConstList>));
}

/**
 * @brief  tests the Transform method for a non empty Typelist.
 */
TEST(Typelist, TransformNonEmptyListAddConst)
{
  using myList      = Typelist<int, double, short, char>;
  using myConstList = Typelist<const int, const double, const short, const char>;
  EXPECT_TRUE((std::is_same_v<typelist::Transform<myList, std::add_const>, myConstList>));
}

/**
 *  @brief  tests the Remove method for a non empty Typelist.
 */
TEST(Typelist, RemoveNonEmptyListValidIndex)
{
  using myList            = Typelist<long long, short, char, int>;
  using myRemovedFromList = Typelist<long long, short, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Remove<myList, 2>, myRemovedFromList>));
}

/**
 * @brief  tests the Remove method for a non empty Typelist.
 */
TEST(Typelist, RemoveNonEmptyListFirstElement)
{
  using myList            = Typelist<long long, short, char, int>;
  using myRemovedFromList = Typelist<short, char, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Remove<myList, 0>, myRemovedFromList>));
}

/**
 * @brief  tests the Remove method for a non empty Typelist.
 */
TEST(Typelist, RemoveNonEmptyListLastElement)
{
  using myList            = Typelist<long long, short, char, int>;
  using myRemovedFromList = Typelist<long long, short, char>;
  EXPECT_TRUE((std::is_same_v<typelist::Remove<myList, typelist::Size<myList> - 1UL>, myRemovedFromList>));
}

/**
 * @brief  tests the Insert method for a non empty Typelist.
 */
TEST(Typelist, InsertAtFront)
{
  using myList = Typelist<long long, short, char, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Insert<myList, double, 0UL>, typelist::PushFront<myList, double>>));
}

/**
 * @brief  tests the Insert method for a non empty Typelist.
 */
TEST(Typelist, InsertAtEnd)
{
  using myList = Typelist<long long, short, char, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Insert<myList, double, typelist::Size<myList>>, typelist::PushBack<myList, double>>));
}

/**
 * @brief  tests the Insert method for a non empty Typelist.
 */
TEST(Typelist, InsertInMiddle)
{
  using myList         = Typelist<long long, short, char, int>;
  using myExpandedList = Typelist<long long, short, double, char, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Insert<myList, double, 2UL>, myExpandedList>));
}

/**
 * @brief  tests the Insert method for an empty Typelist.
 */
TEST(Typelist, InsertEmptyList)
{
  using myList = Typelist<>;
  EXPECT_TRUE((std::is_same_v<typelist::Insert<myList, double, 0UL>, typelist::PushBack<myList, double>>));
}

/**
 * @brief  tests the Replace method for a non empty Typelist.
 */
TEST(Typelist, ReplaceAtFront)
{
  using myList         = Typelist<long long, short, char, int>;
  using myReplacedList = Typelist<double, short, char, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Replace<myList, double, 0UL>, myReplacedList>));
}

/**
 * @brief  tests the Replace method for a non empty Typelist.
 */
TEST(Typelist, ReplaceInMiddle)
{
  using myList         = Typelist<long long, short, char, int>;
  using myReplacedList = Typelist<long long, short, double, int>;
  EXPECT_TRUE((std::is_same_v<typelist::Replace<myList, double, 2UL>, myReplacedList>));
}

/**
 * @brief  tests the Replace method for a non empty Typelist.
 */
TEST(Typelist, ReplaceAtEnd)
{
  using myList         = Typelist<long long, short, char, int>;
  using myReplacedList = Typelist<long long, short, char, double>;
  EXPECT_TRUE((std::is_same_v<typelist::Replace<myList, double, typelist::Size<myList> - 1UL>, myReplacedList>));
}

template <typename T, typename S>
struct SmallerThan : std::conditional_t<(sizeof(T) < sizeof(S)), std::true_type, std::false_type> {};

/**
 * @brief  tests the InsertSorted method for a non empty Typelist.
 */
TEST(Typelist, InsertSorted)
{
  using myList         = Typelist<char, short, long long>;
  using myExpandedList = Typelist<char, short, int, long long>;
  EXPECT_TRUE((std::is_same_v<typelist::InsertSorted<myList, int, SmallerThan>, myExpandedList>));
}

/**
 * @brief  tests the InsertionSort method for a non empty Typelist.
 */
TEST(Typelist, InsertionSort)
{
  using myList       = Typelist<long long, short, char, int>;
  using mySortedList = Typelist<char, short, int, long long>;
  EXPECT_TRUE((std::is_same_v<typelist::InsertionSort<myList, SmallerThan>, mySortedList>));
}

/**
 * @brief  tests the InsertionSort method for a non empty Typelist.
 */
TEST(Typelist, InsertionSortOneElement)
{
  using myList       = Typelist<long long>;
  using mySortedList = Typelist<long long>;
  EXPECT_TRUE((std::is_same_v<typelist::InsertionSort<myList, SmallerThan>, mySortedList>));
}

/**
 * @brief  tests the InsertionSort method for an empty Typelist.
 */
TEST(Typelist, InsertionSortEmptyList)
{
  using myList = Typelist<>;
  EXPECT_TRUE((std::is_same_v<typelist::InsertionSort<myList, SmallerThan>, myList>));
}

template <typename... Ts>
struct AccumulateSize {
  using value_type = std::size_t;
  static constexpr value_type value{(... + sizeof(Ts))};
};

TEST(Typelist, TransformInvokeTwoLists)
{
  constexpr std::size_t numLists = 2ULL;
  using L1 = Typelist<char, short, int, long>;
  constexpr std::array<std::size_t, 4ULL> expected{numLists * sizeof(char), numLists * sizeof(short),
                                                   numLists * sizeof(int), numLists * sizeof(long)};

  constexpr auto generated = meta::typelist::TransformInvoke<AccumulateSize, L1, L1>();

  EXPECT_EQ(generated, expected);
}

TEST(Typelist, TransformInvokeFiveLists)
{
  constexpr std::size_t numLists = 5ULL;
  using L1 = Typelist<char, short, int, long>;
  constexpr std::array<std::size_t, 4ULL> expected{numLists * sizeof(char), numLists * sizeof(short),
                                                   numLists * sizeof(int), numLists * sizeof(long)};

  constexpr auto generated = meta::typelist::TransformInvoke<AccumulateSize, L1, L1, L1, L1, L1>();

  EXPECT_EQ(generated, expected);
}

 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TYPELISTTEST_H_32369310941833031878104711730140601259310821 
