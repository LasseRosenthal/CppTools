/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Typelist.h 
 * @brief   Definition of the Typelist structure and a bunch of compile-time meta functions
 *          for the investigation of typelists.
 * 
 * @author  Lasse Rosenthal 
 * @date    24.06.2020 
 */ 
 
#ifndef TYPELIST_H_319413207746512531227501161632313285010491 
#define TYPELIST_H_319413207746512531227501161632313285010491 
 
 
// includes
#include <Meta/Utility.h>

#include <array>
#include <type_traits>
 

namespace meta {
 

/** 
 * @struct Typelist
 * @brief  Typelist models a heterogeneous compile-time list of types.
 * @tparam Ts The types the Typelist is build on.
 */
template <typename... Ts>
struct Typelist {};


namespace typelist {


/**
 * @struct SizeT
 * @brief  SizeT is a meta function returning the size of a given typelist.
 * @tparam List the Typelist.
 */
template <typename List>
struct SizeT {};

template <typename... Tail>
struct SizeT<Typelist<Tail...>> {
  static constexpr std::size_t value = sizeof...(Tail);
};

template <typename List>
constexpr auto Size = SizeT<List>::value;

/**
 * @struct IsEmptyT
 * @brief  IsEmptyT is a meta function returning a compile-time boolean
 *         indicating whether a given Typelist is empty.
 * @tparam List the Typelist.
 */
template <typename List>
struct IsEmptyT : std::false_type {};

template <>
struct IsEmptyT<Typelist<>> : std::true_type {};

template <typename List>
constexpr bool IsEmpty = IsEmptyT<List>::value;

/**
 * @struct FrontT
 * @brief  FrontT provides access the the type of the first element
 *         of a given Typelist.
 * @tparam List the Typelist.
 */
template <typename List>
struct FrontT {};

template <typename Head, typename... Tail>
struct FrontT<Typelist<Head, Tail...>> : IdentityT<Head> {};

template <typename List>
using Front = typename FrontT<List>::type;

/**
 * @struct PushFrontT 
 * @brief  PushFrontT adds a given type to the front of a given Typelist.
 * @tparam List the Typelist.
 * @tparam NewElement the type that is added to the Typelist.
 */
template <typename List, typename NewElement>
struct PushFrontT;

template <typename... Ts, typename NewElement>
struct PushFrontT<Typelist<Ts...>, NewElement> : IdentityT<Typelist<NewElement, Ts...>> {};

template <typename List, typename NewElement>
using PushFront = typename PushFrontT<List, NewElement>::type;

/**
 * @struct PopFrontT
 * @brief  PopFront maps a given Typelist on a Typelist that emerges by
 *         popping the first element of the given Typelist.
 * @tparam List the Typelist.
 */
template <typename List>
struct PopFrontT {};

template <typename Head, typename... Tail>
struct PopFrontT<Typelist<Head, Tail...>> : IdentityT<Typelist<Tail...>> {};

template <typename List>
using PopFront = typename PopFrontT<List>::type;

/** 
 * @brief  NthElement
 * @brief  Provides access to the N-th element of a given typelist.
 *         If the index is out of bounds, the return type is InvalidType.
 * @tparam List the Typelist.
 * @tparam N A zero based index indicating the position of the type.
 */
template <typename List, std::size_t N, bool = (N < Size<List>)>
struct NthElementT;

template <typename List, std::size_t N>
struct NthElementT<List, N, true> : NthElementT<PopFront<List>, N - 1UL> {};

template <typename List>
struct NthElementT<List, 0UL, true> : FrontT<List> {};

template <typename List, std::size_t N>
struct NthElementT<List, N, false> : IdentityT<InvalidType> {};

template <typename List, std::size_t N>
using NthElement = typename NthElementT<List, N>::type;

/**
 * @struct BackT
 * @brief  BackT provides access the the type of the last element
 *         of a given Typelist.
 * @tparam List the Typelist.
 */
template <typename List, bool = IsEmpty<List>>
struct BackT;

template <typename List>
struct BackT<List, false> : NthElementT<List, Size<List> - 1> {};

template <typename List>
struct BackT<List, true> : IdentityT<InvalidType> {};

template <typename List>
using Back = typename BackT<List>::type;

/** 
 * @struct PushBackT
 * @brief  PushBackT appends a given type to the front of a given Typelist.
 * @tparam List the Typelist.
 * @tparam NewElement the type that is appended to the Typelist.
 */
template <typename List, typename NewElement, bool = IsEmpty<List>>
struct PushBackT;

template <typename List, typename NewElement>
using PushBack = typename PushBackT<List, NewElement>::type;

template <typename List, typename NewElement>
struct PushBackT<List, NewElement, false> : PushFrontT<PushBack<PopFront<List>, NewElement>, Front<List>> {};

template <typename List, typename NewElement>
struct PushBackT<List, NewElement, true> : IdentityT<Typelist<NewElement>> {};

/**
 * @struct ReverseT
 * @brief  ReverseT reverses a given Typelist.
 * @tparam List the Typelist.
 */
template <typename List, bool = IsEmpty<List>>
struct ReverseT;

template <typename List>
using Reverse = typename ReverseT<List>::type;

template <typename List>
struct ReverseT<List, false> : PushBackT<Reverse<PopFront<List>>, Front<List>> {};

template <typename List>
struct ReverseT<List, true> : IdentityT<List> {};

/**
 * @struct PopBackT
 * @brief  PopBackT maps a given Typelist on a Typelist that emerges by
 *         popping the last element of the given Typelist.
 * @tparam List the Typelist.
 */
template <typename List, bool = IsEmpty<List>>
struct PopBackT;

template <typename List>
struct PopBackT<List, false> : ReverseT<PopFront<Reverse<List>>>{};

template <typename List>
struct PopBackT<List, true> : IdentityT<InvalidType> {};

template <typename List>
using PopBack = typename PopBackT<List>::type;

/**
 * @struct ConcatT
 * @brief  ConcatT provides a Typelist resulting from the concatenation of two Typelists.
 * @tparam List1 the Typelist at whose end the other Typelist is appended.
 * @tparam List2 the Typelist that is appended to the first Typelist.
 */
template <typename List1, typename List2, bool Empty = IsEmpty<List2>>
struct ConcatT;

template <typename List1, typename List2>
using Concat = typename ConcatT<List1, List2>::type;

template <typename List1, typename List2>
struct ConcatT<List1, List2, false> : 
  PushBackT<Concat<List1, PopBack<List2>>, Back<List2>> 
{};

template <typename List1, typename List2>
struct ConcatT<List1, List2, true> : IdentityT<List1> {};

/**
 * @struct HasTypeT
 * @brief  HasTypeT returns true if a given Typelist has a certain type.
 *         Otherwise, false.
 * @tparam List the Typelist.
 * @tparam T the type that is asked for.
 */
template <typename List, typename T, bool = IsEmpty<List>>
struct HasTypeT;

template <typename List, typename T>
struct HasTypeT<List, T, false>
  : std::conditional_t<std::is_same_v<Front<List>, T>, std::true_type, HasTypeT<PopFront<List>, T>> {};

template <typename List, typename T>
struct HasTypeT<List, T, true> : std::false_type {};

template <typename List, typename T>
constexpr bool HasType = HasTypeT<List, T>::value;

/**
 * @struct HasTypeWithPropertyT
 * @brief  HasTypeWithPropertyT returns true if a given Typelist type fullfilling a given predicate.
 *         Otherwise, false.
 * @tparam List the Typelist.
 * @tparam Predicate the predicate that is asked for.
 */
template <typename List, template <typename> class Predicate, bool = IsEmpty<List>>
struct HasTypeWithPropertyT;

template <typename List, template <typename> class Predicate>
constexpr bool HasTypeWithProperty = HasTypeWithPropertyT<List, Predicate>::value;

template <typename List, template <typename> class Predicate>
struct HasTypeWithPropertyT<List, Predicate, false>
  : std::conditional_t<Predicate<Front<List>>::value,
                       std::true_type,
                       HasTypeWithPropertyT<PopFront<List>, Predicate>
                      > {};

template <typename List, template <typename> class Predicate>
struct HasTypeWithPropertyT<List, Predicate, true> : std::false_type {};

/**
 * @struct FindIfT
 * @brief  FindIfT returns the zero based index of the first type fullfilling a given
 *         predicate. If no such type is found, -1 is returned.
 * @tparam List the Typelist.
 * @tparam Predicate the predicate that is asked for.
 */
template <typename List, template <typename> class Predicate, bool = HasTypeWithProperty<List, Predicate>>
struct FindIfT;

template <typename List, template <typename> class Predicate>
constexpr auto FindIf = FindIfT<List, Predicate>::value;

template <typename List, template <typename> class Predicate>
struct FindIfT<List, Predicate, true>
  : std::conditional_t<Predicate<Front<List>>::value,
                       std::integral_constant<int, 0>,
                       std::integral_constant<int, 1 + FindIf<PopFront<List>, Predicate>>
                      > {};

template <typename List, template <typename> class Predicate>
struct FindIfT<List, Predicate, false> : std::integral_constant<int, -1> {};

/**
 * @struct CountIfT
 * @brief  CountIfT counts the number of elements fullfilling a given predicate.
 * @tparam List the Typelist.
 * @tparam Predicate the predicate that is asked for.
 */
template <typename List, template <typename> class Predicate, bool = IsEmpty<List>>
struct CountIfT;

template <typename List, template <typename> class Predicate>
constexpr auto CountIf = CountIfT<List, Predicate>::value;

template <typename List, template <typename> class Predicate>
struct CountIfT<List, Predicate, false>
  : std::conditional_t<Predicate<Front<List>>::value,
                       std::integral_constant<std::size_t, 1UL + CountIf<PopFront<List>, Predicate>>,
                       std::integral_constant<std::size_t, CountIf<PopFront<List>, Predicate>>
                      > {};

template <typename List, template <typename> class Predicate>
struct CountIfT<List, Predicate, true> : std::integral_constant<std::size_t, 0UL> {};

/**
 * @brief  AllOf is a compile-time boolean which is true if all elements of a given Typelist
 *         fullfill a given predicate.
 * @tparam List the Typelist.
 * @tparam Predicate the predicate that is asked for.
 */
template <typename List, template <typename> class Predicate>
constexpr bool AllOf = CountIf<List, Predicate> == Size<List>;

/**
 * @struct IndexOfT
 * @brief  IndexOfT provides the zero based index of the first occurence of a given type.
 * @tparam List the Typelist.
 * @tparam T the type whose index is asked for.
 */
template <typename List, typename T, bool = HasType<List, T>>
struct IndexOfT;

template <typename List, typename T>
constexpr auto IndexOf = IndexOfT<List, T>::value;

template <typename List, typename T>
struct IndexOfT<List, T, true>
  : std::conditional_t<std::is_same_v<Front<List>, T>,
                       std::integral_constant<int, 0>,
                       std::integral_constant<int, 1 + IndexOf<PopFront<List>, T>>
                      > {};

template <typename List, typename T>
struct IndexOfT<List, T, false> : std::integral_constant<int, -1> {};

/**
 * @struct MaxElementT
 * @brief  MaxElementT provides the zero based index of the maximum element with respect to a given Comparison.
 * @tparam List the Typelist.
 * @tparam Compare the comparison.
 */
template <typename List, template <typename, typename> class Compare, bool = IsEmpty<List>>
struct MaxElementT;

template <typename List, template <typename, typename> class Compare>
constexpr auto MaxElement = MaxElementT<List, Compare>::value;

template <typename List, template <typename, typename> class Compare>
struct MaxElementT<List, Compare, false>
  : std::conditional_t<Compare<Front<List>, NthElement<PopFront<List>, MaxElement<PopFront<List>, Compare>>>::value,
                       std::integral_constant<int, 0>,
                       std::integral_constant<int, 1 + MaxElement<PopFront<List>, Compare>>
                      > {};

template <typename T, template <typename, typename> class Compare>
struct MaxElementT<Typelist<T>, Compare, false> : std::integral_constant<int, 0> {};

template <typename List, template <typename, typename> class Compare>
struct MaxElementT<List, Compare, true> : std::integral_constant<int, -1> {};

/**
 * @struct TransformT
 * @brief  TransformT applies a given type-to-type Transformation to all types of a given list
 *         and provides a Typelist constructed by the resulting transformations.
 * @tparam List the Typelist.
 * @tparam TypeTransform the type-to-type transformation.
 */
template <typename List, template <typename> class TypeTransform, bool = IsEmpty<List>>
struct TransformT;

template <typename List, template <typename> class TypeTransform>
using Transform = typename TransformT<List, TypeTransform>::type;

template <typename List, template <typename> class TypeTransform>
struct TransformT<List, TypeTransform, false> 
  : PushFrontT<Transform<PopFront<List>, TypeTransform>, typename TypeTransform<Front<List>>::type> {};

template <typename List, template <typename> class TypeTransform>
struct TransformT<List, TypeTransform, true> : IdentityT<List> {};

/**
 * @struct TransformBinaryT
 * @brief  TransformBinaryT applies a given type-to-type Transformation to all types of two given list
 *         and provides a Typelist constructed by the resulting transformations.
 * @tparam List1 the first Typelist.
 * @tparam List2 the second Typelist.
 * @tparam BinaryTypeTransform the two type to one type transformation.
 */
template <typename List1, typename List2, template <typename, typename> class BinaryTypeTransform, bool = IsEmpty<List1>>
struct TransformBinaryT;

template <typename List1, typename List2, template <typename, typename> class BinaryTypeTransform>
using TransformBinary = typename TransformBinaryT<List1, List2, BinaryTypeTransform>::type;

template <typename List1, typename List2, template <typename, typename> class BinaryTypeTransform>
struct TransformBinaryT<List1, List2, BinaryTypeTransform, false>
  : PushFrontT<TransformBinary<PopFront<List1>, PopFront<List2>, BinaryTypeTransform>,
               typename BinaryTypeTransform<Front<List1>, Front<List2>>::type
              > {};

template <typename List1, typename List2, template <typename, typename> class BinaryTypeTransform>
struct TransformBinaryT<List1, List2, BinaryTypeTransform, true> : IdentityT<List1> {};

/**
 * @struct RemoveT
 * @brief  RemoveT removes the type at a given position
 * @tparam List the Typelist from which the type is removed.
 * @tparam Index The index of the type that has to be removed.
 */
template <typename List, std::size_t Index, bool = IsEmpty<List>>
struct RemoveT;

template <typename List, std::size_t Index>
using Remove = typename RemoveT<List, Index>::type;

template <typename List, std::size_t Index>
struct RemoveT<List, Index, false> : PushFrontT<Remove<PopFront<List>, Index - 1UL>, Front<List>> {};

template <typename List>
struct RemoveT<List, 0UL, false> : PopFrontT<List> {};

/**
 * @struct InsertT
 * @brief  InsertT inserts a given type at a certain position.
 * @tparam List the Typelist which will be extended.
 * @tparam Index The index where the type that has to be inserted.
 */
template <typename List, typename T, std::size_t Index, bool = IsEmpty<List>>
struct InsertT;

template <typename List, typename T, std::size_t Index>
using Insert = typename InsertT<List, T, Index>::type;

template <typename List, typename T, std::size_t Index>
struct InsertT<List, T, Index, false> : PushFrontT<Insert<PopFront<List>, T, Index - 1UL>, Front<List>> {};

template <typename List, typename T>
struct InsertT<List, T, 0UL, false> : PushFrontT<List, T> {};

template <typename List, typename T>
struct InsertT<List, T, 0UL, true> : PushFrontT<List, T> {};

/**
 * @struct ReplaceT
 * @brief  ReplaceT replaces a given type at a certain position with another type.
 * @tparam List the Typelist which will be extended.
 * @tparam T the new type.
 * @tparam Index The index where the type that has to be inserted.
 */
template <typename List, typename T, std::size_t Index, bool = IsEmpty<List>>
struct ReplaceT;

template <typename List, typename T, std::size_t Index>
using Replace = typename ReplaceT<List, T, Index>::type;

template <typename List, typename T, std::size_t Index>
struct ReplaceT<List, T, Index, false> : PushFrontT<Replace<PopFront<List>, T, Index - 1UL>, Front<List>> {};

template <typename List, typename T>
struct ReplaceT<List, T, 0UL, false> : PushFrontT<PopFront<List>, T> {};

/**
 * @struct InsertSortedT
 * @brief  InsertSortedT finds the position of a given type in a given Typelist with respect
 *         to a Comparison and inserts the type at this position.
 * @tparam T the type that has to be added to the list.
 * @tparam Compare the comparison that is used to insert the new type.
 */
template <typename List, typename T, template <typename, typename> class Compare, bool = IsEmpty<List>>
struct InsertSortedT;

template <typename List, typename T, template <typename, typename> class Compare>
using InsertSorted = typename InsertSortedT<List, T, Compare>::type;

template <typename List, typename T, template <typename, typename> class Compare>
struct InsertSortedT<List, T, Compare, false> {
private:
  using newTail = std::conditional_t<Compare<T, Front<List>>::value, Identity<List>, InsertSorted<PopFront<List>, T, Compare>>;
  using newHead = std::conditional_t<Compare<T, Front<List>>::value, T, Front<List>>;

public:
  using type = PushFront<newTail, newHead>;
};

template <typename List, typename T, template <typename, typename> class Compare>
struct InsertSortedT<List, T, Compare, true> : PushFrontT<List, T> {};

/**
 * @struct InsertionSortT
 * @brief  InsertionSortT creates a sorted version a given Typelist via the insertion sort algorithm
 *         and a given comparison.
 * @tparam List the new type.
 * @tparam T the new type.
 * @tparam Compare the comparison that is used to create the sorted list.
 */
template <typename List, template <typename, typename> class Compare, bool = IsEmpty<List>>
struct InsertionSortT;

template <typename List, template <typename, typename> class Compare>
using InsertionSort = typename InsertionSortT<List, Compare>::type;

template <typename List, template <typename, typename> class Compare>
struct InsertionSortT<List, Compare, false> : InsertSortedT<InsertionSort<PopFront<List>, Compare>, Front<List>, Compare> {};

template <typename List, template <typename, typename> class Compare>
struct InsertionSortT<List, Compare, true> : IdentityT<List> {};

/** 
 * @brief Accumulate
 */
template <typename List, template <typename, typename> class F, typename I, bool = IsEmpty<List>>
struct AccumulateT;

template <typename List, template <typename, typename> class F, typename I>
using Accumulate = typename AccumulateT<List, F, I>::type;

template <typename List, template <typename, typename> class F, typename I>
struct AccumulateT<List, F, I, false> : AccumulateT<PopFront<List>, F, typename F<I, Front<List>>::type> {};

template <typename List, template <typename, typename> class F, typename I>
struct AccumulateT<List, F, I, true> : Identity<I> {};


template <template <typename...> class VarTypeFun, typename... Lists>
struct CallWrapper {
  template <std::size_t I>
  static constexpr auto value = VarTypeFun<NthElement<Lists, I>...>::value;
};

/**
 * @brief  TransformInvoke applies a given variadic metafunction to all types of an arbitrary
 *         number of Typelists and stores the results in a std::array.
 * @tparam VarTypeFun a variadic metafunction that maps its template parameters to a value.
 */
template <template <typename...> class VarTypeFun, typename... Lists>
constexpr auto TransformInvoke() noexcept
{
  using RetType = std::array<typename VarTypeFun<Front<Lists>...>::value_type, Size<meta::FirstTypeOf<Lists...>>>;
  return TransformInvoke<CallWrapper<VarTypeFun, Lists...>, RetType>(std::make_index_sequence<Size<meta::FirstTypeOf<Lists...>>>());
}

template <typename CallWrapperT, typename RetType, std::size_t... Is>
constexpr auto TransformInvoke(std::index_sequence<Is...>) noexcept -> RetType
{
  return {CallWrapperT::template value<Is>...};
}


}   // namespace typelist
}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TYPELIST_H_319413207746512531227501161632313285010491 
