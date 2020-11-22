/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Valuelist.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.06.2020 
 */ 
 
#ifndef VALUELIST_H_284454785552729487230342037114028595310485 
#define VALUELIST_H_284454785552729487230342037114028595310485 
 
 
// includes
#include <Meta/Typelist.h>
 

namespace meta {


/** 
 * @struct Valuelist
 * @brief  Valuelist models a homogeneous list of compile-time values.
 * @tparam T The type of the stored values.
 * @tparam Values The values stored in the list.
 */
template <typename T, T... Values>
struct Valuelist {};


namespace valuelist {


/**
 * @brief ConstValueType is an alias template.
 */
template <typename T, T Value>
using ConstValueType = std::integral_constant<T, Value>;

/**
 * @struct TypelistEquivalentT
 * @brief  TypelistEquivalentT provides a Typelist equivalent for a Valuelist
 */
template <typename List>
struct TypelistEquivalentT {};

template <typename T, T... Values>
struct TypelistEquivalentT<Valuelist<T, Values...>> 
  : IdentityT<meta::Typelist<ConstValueType<T, Values>...>> {};

template <typename List>
using TypelistEquivalent = typename TypelistEquivalentT<List>::type;

/**
 * @struct ValuelistEquivalentT
 * @brief  ValuelistEquivalent provides a ValueList equivalent for
 *         a Typelist of type Typelist<ConstValueType<T, v1>, ConstValueType<T, v2>, ...>
 */
template <typename List>
struct ValuelistEquivalentT {};

template <typename T, T... Values>
struct ValuelistEquivalentT<Typelist<ConstValueType<T, Values>...>> : IdentityT<Valuelist<T, Values...>> {};

template <typename T, T Value>
struct ValuelistEquivalentT<Typelist<ConstValueType<T, Value>>> : IdentityT<Valuelist<T, Value>> {};

template <typename List>
using ValuelistEquivalent = typename ValuelistEquivalentT<List>::type;


/**
 * @struct TypeT
 * @brief  TypeT provides the type of a given Valuelist.
 * @tparam List The Valuelist.
 */
template <typename List>
struct TypeT {};

template <typename T, T... Values>
struct TypeT<Valuelist<T, Values...>> : IdentityT<T> {};

template <typename List>
using Type = typename TypeT<List>::type;

/** 
 * @brief  Size is a compile-time constant storing the size of a given Valuelist.
 */
template <typename List>
constexpr std::size_t Size = meta::typelist::Size<TypelistEquivalent<List>>;

/**
 * @brief  IsEmpty is a compile-time boolean indicating whether a given Valuelist is empty.
 */
template <typename List>
constexpr bool IsEmpty = meta::typelist::IsEmpty<TypelistEquivalent<List>>;

/**
 * @struct IotaT
 * @brief  IotaT generates a valuelist of consecutive integers
 *         ranging from 0 to Size - 1
 */
template <typename, typename>
struct IotaHelper;

template <typename T, T... Is>
struct IotaHelper<T, std::integer_sequence<T, Is...>>
  : IdentityT<Valuelist<T, Is...>> {};

template <int Size, typename T = int>
struct IotaT : IotaHelper<T, std::make_integer_sequence<T, Size>> {};

template <int Size, typename T = int>
using Iota = typename IotaT<Size, T>::type;

/** 
 * @brief Front is the first value of a given Valuelist.
 */
template <typename List>
constexpr Type<List> Front = typelist::Front<TypelistEquivalent<List>>::value;

/**
 * @struct PushFrontT 
 * @brief  PushFrontT adds a given value to the front of a given Valuelist.
 * @tparam List the Valuelist.
 * @tparam NewElement the type that is added to the Typelist.
 */
template <typename List, Type<List> NewElement>
struct PushFrontT;

template <typename T, T NewElement, T... Values>
struct PushFrontT<Valuelist<T, Values...>, NewElement> : IdentityT<Valuelist<T, NewElement, Values...>> {};

template <typename List, Type<List> NewElement>
using PushFront = typename PushFrontT<List, NewElement>::type;

/**
 * @struct PopFrontT
 * @brief  PopFront maps a given Valuelist on a Valuelist that emerges by
 *         popping the first element of the given Valuelist.
 * @tparam List the Valuelist.
 */
template <typename List>
struct PopFrontT {};

template <typename T, T Head, T... Tail>
struct PopFrontT<Valuelist<T, Head, Tail...>> : IdentityT<Valuelist<T, Tail...>> {};

template <typename List>
using PopFront = typename PopFrontT<List>::type;

/** 
 * @brief PushBack appends a given value to a given typelist.
 */
template <typename List, Type<List> NewElement>
using PushBack = ValuelistEquivalent<typelist::PushBack<TypelistEquivalent<List>, ConstValueType<Type<List>, NewElement>>>;

/** 
 * @brief  NthElement
 * @brief  Provides access to the N-th element of a given Valuelist.
 * @tparam List the Valuelist.
 * @tparam N A zero based index indicating the position of the type.
 */
template <typename List, std::size_t N>
struct NthElementT : NthElementT<PopFront<List>, N - 1UL> {};

template <typename List>
struct NthElementT<List, 0UL> 
  : std::integral_constant<Type<List>, Front<List>> {};

template <typename List, std::size_t N>
constexpr Type<List> NthElement = NthElementT<List, N>::value;

/** 
 * @brief HasValue is compile-time boolean which is true if a Valuelist has a certain value.
 */
template <typename List, Type<List> Value>
constexpr bool HasValue = typelist::HasType<TypelistEquivalent<List>, ConstValueType<Type<List>, Value>>;

template <typename List, template <Type<List>...> class NAryPredicate>
struct ValueToTypePredicateT {
  template <typename... Cs>
  struct type : std::conditional_t<NAryPredicate<Cs::value...>::value,
                                   std::true_type,
                                   std::false_type> {};
};

template <typename List, template <Type<List>...> class NAryPredicate>
using ValueToTypePredicate = ValueToTypePredicateT<List, NAryPredicate>::template type;

/** 
 * @brief MapBinaryMetaFun maps a Value based meta function
 *        to a meta function expecting types of std::integral_constant or any
 *        other template class providing a value member.
 */
template <typename List, template <Type<List>...> class ValueFun>
struct ValueToTypeFunT {
  template <typename... Cs>
  struct type : ConstValueType<Type<List>, ValueFun<Cs::value...>::value> {};
};

template <typename List, template <Type<List>...> class ValueFun>
using ValueToTypeFun = ValueToTypeFunT<List, ValueFun>::template type;

/** 
 * @brief HasValueWithProperty
 */
template <typename List, template <Type<List>> class UnaryPredicate>
constexpr bool HasValueWithProperty =
  typelist::HasTypeWithProperty<TypelistEquivalent<List>, ValueToTypePredicate<List, UnaryPredicate>>;

/** 
 * @brief FindIf
 */
template <typename List, template <Type<List>> class UnaryPredicate>
constexpr auto FindIf =
  meta::typelist::FindIf<TypelistEquivalent<List>, ValueToTypePredicate<List, UnaryPredicate>>;

/** 
 * @brief CountIf
 */
template <typename List, template <Type<List>> class UnaryPredicate>
constexpr auto CountIf =
  meta::typelist::CountIf<TypelistEquivalent<List>, ValueToTypePredicate<List, UnaryPredicate>>;

/** 
 * @brief IndexOf
 */
template <typename List, Type<List> Element>
constexpr auto IndexOf =
  meta::typelist::IndexOf<TypelistEquivalent<List>, ConstValueType<Type<List>, Element>>;

/** 
 * @brief Remove
 */
template <typename List, std::size_t Index>
using Remove = ValuelistEquivalent<meta::typelist::Remove<TypelistEquivalent<List>, Index>>;

/** 
 * @brief Insert
 */
template <typename List, Type<List> NewElement, std::size_t Index>
using Insert = ValuelistEquivalent<
  meta::typelist::Insert<TypelistEquivalent<List>, ConstValueType<Type<List>, NewElement>, Index>>;

/** 
 * @brief Replace
 */
template <typename List, Type<List> NewElement, std::size_t Index>
using Replace = ValuelistEquivalent<
  meta::typelist::Replace<TypelistEquivalent<List>, ConstValueType<Type<List>, NewElement>, Index>>;

/** 
 * @brief MaxElement
 */
template <typename List, template <Type<List>, Type<List>> class BinaryPredicate>
constexpr auto MaxElement =
  meta::typelist::MaxElement<TypelistEquivalent<List>, ValueToTypePredicate<List, BinaryPredicate>>;

/** 
 * @brief Reverse
 */
template <typename List, bool = IsEmpty<List>>
struct ReverseT;

template <typename List>
using Reverse = typename ReverseT<List>::type;

template <typename List>
struct ReverseT<List, false> {
  using type = PushBack<Reverse<PopFront<List>>, Front<List>>;
};

template <typename List>
struct ReverseT<List, true> : meta::IdentityT<List> {};

/** 
 * @brief  Transform
 */
template <typename List, template <Type<List>> class MetaFun, bool = IsEmpty<List>>
struct TransformT;

template <typename List, template <Type<List>> class MetaFun>
using Transform = typename TransformT<List, MetaFun>::type;

template <typename List, template <Type<List>> class MetaFun>
struct TransformT<List, MetaFun, false> : PushFrontT<Transform<PopFront<List>, MetaFun>, MetaFun<Front<List>>::value> {};

template <typename List, template <Type<List>> class MetaFun>
struct TransformT<List, MetaFun, true> : meta::IdentityT<List> {};

/** 
 * @brief TransformBinary
 */
template <typename List1, typename List2, template <Type<List1>, Type<List2>> class BinaryMetaFun,
          bool S1 = (Size<List1> >= 2), bool Empty = IsEmpty<List1>>
struct TransformBinaryT;

template <typename List1, typename List2, template <Type<List1>, Type<List2>> class BinaryMetaFun>
using TransformBinary = typename TransformBinaryT<List1, List2, BinaryMetaFun>::type;

template <typename List1, typename List2, template <Type<List1>, Type<List2>> class BinaryMetaFun>
struct TransformBinaryT<List1, List2, BinaryMetaFun, true, false /*, Size<List1>*/>
  : PushFrontT<TransformBinary<PopFront<List1>, PopFront<List2>, BinaryMetaFun>, BinaryMetaFun<Front<List1>, Front<List2>>::value> {};

template <typename List1, typename List2, template <Type<List1>, Type<List2>> class BinaryMetaFun>
struct TransformBinaryT<List1, List2, BinaryMetaFun, false, false>
  : meta::IdentityT<Valuelist<Type<List1>, BinaryMetaFun<Front<List1>, Front<List2>>::value>> {};

template <typename List1, typename List2, template <Type<List1>, Type<List2>> class BinaryMetaFun>
struct TransformBinaryT<List1, List2, BinaryMetaFun, false, true> : meta::IdentityT<List1> {};

/** 
 * @brief Accumulate
 */
template <typename List, template <Type<List>, Type<List>> class BinaryMetaFun,
          Type<List> I = Type<List>{}>
constexpr auto Accumulate =
  meta::typelist::Accumulate<TypelistEquivalent<List>, ValueToTypeFun<List, BinaryMetaFun>,
                             ConstValueType<Type<List>, I>>::value;

/** 
 * @brief InsertSorted
 */
template <typename List, Type<List> NewElement,
          template <Type<List>, Type<List>> class BinaryPredicate>
struct InsertSortedT
  : meta::IdentityT<ValuelistEquivalent<
      meta::typelist::InsertSorted<TypelistEquivalent<List>, ConstValueType<Type<List>, NewElement>,
                                   ValueToTypePredicate<List, BinaryPredicate>>>> {};

template <typename List, Type<List> NewElement, template <Type<List>, Type<List>> class BinaryPredicate>
using InsertSorted = typename InsertSortedT<List, NewElement, BinaryPredicate>::type;

/** 
 * @brief InsertionSort
 */
template <typename List, template <Type<List>, Type<List>> class BinaryPredicate, bool = IsEmpty<List>>
struct InsertionSortT;

template <typename List, template <Type<List>, Type<List>> class BinaryPredicate>
using InsertionSort = typename InsertionSortT<List, BinaryPredicate>::type;

template <typename List, template <Type<List>, Type<List>> class BinaryPredicate>
struct InsertionSortT<List, BinaryPredicate, false>
  : InsertSortedT<InsertionSort<PopFront<List>, BinaryPredicate>, Front<List>, BinaryPredicate> {};

template <typename List, template <Type<List>, Type<List>> class BinaryPredicate>
struct InsertionSortT<List, BinaryPredicate, true> : meta::IdentityT<List> {};

/** 
 * @brief ElementsWithProperty creates a Valuelist of all values for which
 *        the given unary predicate returns true.
 */
template <typename List, template <Type<List>> class UnaryPredicate, bool = (Size<List> >= 2), bool = IsEmpty<List>>
struct ElementsWithPropertyT;

template <typename List, template <Type<List>> class UnaryPredicate>
using ElementsWithProperty = typename ElementsWithPropertyT<List, UnaryPredicate>::type;

template <typename List, template <Type<List>> class UnaryPredicate>
struct ElementsWithPropertyT<List, UnaryPredicate, true, false>
  : std::conditional<UnaryPredicate<Front<List>>::value,
                     PushFront<ElementsWithProperty<PopFront<List>, UnaryPredicate>, Front<List>>,
                     ElementsWithProperty<PopFront<List>, UnaryPredicate>> {};

template <typename List, template <Type<List>> class UnaryPredicate>
struct ElementsWithPropertyT<List, UnaryPredicate, false, false>
  : std::conditional<UnaryPredicate<Front<List>>::value, Valuelist<Type<List>, Front<List>>,
                     Valuelist<Type<List>>> {};

template <typename List, template <Type<List>> class UnaryPredicate>
struct ElementsWithPropertyT<List, UnaryPredicate, false, true> : meta::IdentityT<List> {};

/** 
 * @brief AllIndicesOfTypes creates a Valuelist of all indices of types for which
 *        the given unary predicate returns true.
 */
template <typename OrigList, typename PartList, template <typename> class UnaryPredicate,
          bool = (meta::typelist::Size<PartList> >= 2), bool = meta::typelist::IsEmpty<PartList>>
struct AllIndicesOfTypesImplT;

template <typename OrigList, typename PartList, template <typename> class UnaryPredicate>
using AllIndicesOfTypesImpl =
  typename AllIndicesOfTypesImplT<OrigList, PartList, UnaryPredicate>::type;

template <typename OrigList, typename PartList, template <typename> class UnaryPredicate>
struct AllIndicesOfTypesImplT<OrigList, PartList, UnaryPredicate, true, false>
  : std::conditional<
      UnaryPredicate<meta::typelist::Front<PartList>>::value,
      PushFront<AllIndicesOfTypesImpl<OrigList, meta::typelist::PopFront<PartList>, UnaryPredicate>,
                meta::typelist::IndexOf<OrigList, meta::typelist::Front<PartList>>>,
      AllIndicesOfTypesImpl<OrigList, meta::typelist::PopFront<PartList>, UnaryPredicate>
    > {};

template <typename OrigList, typename PartList, template <typename> class UnaryPredicate>
struct AllIndicesOfTypesImplT<OrigList, PartList, UnaryPredicate, false, false>
  : std::conditional<
      UnaryPredicate<meta::typelist::Front<PartList>>::value,
      Valuelist<int, meta::typelist::IndexOf<OrigList, meta::typelist::Front<PartList>>>,
      Valuelist<int>
    > {};

template <typename OrigList, typename PartList, template <typename> class UnaryPredicate>
struct AllIndicesOfTypesImplT<OrigList, PartList, UnaryPredicate, false, true>
  : meta::IdentityT<Valuelist<int>> {};

template <typename List, template <typename> class UnaryPredicate>
using AllIndicesOfTypes = AllIndicesOfTypesImpl<List, List, UnaryPredicate>;

/** 
 * @brief AllIndicesOf creates a Valuelist of all indices of values for which
 *        the given unary predicate returns true.
 */
template <typename List, template <Type<List>> class UnaryPredicate>
using AllIndicesOf = AllIndicesOfTypes<TypelistEquivalent<List>, ValueToTypePredicate<List, UnaryPredicate>>;

/** 
 * @brief TypeToValuelist transforms a typelist into a valuelist by applying a given
 *        type based meta function to every element of the given typelist.
 */
template <typename List, template <typename> class MetaFun, bool = (meta::typelist::Size<List> > 1)>
struct TypeToValuelistT;

template <typename List, template <typename> class MetaFun>
using TypeToValuelist = typename TypeToValuelistT<List, MetaFun>::type;

template <typename List, template <typename> class MetaFun>
struct TypeToValuelistT<List, MetaFun, true>
  : PushFrontT<TypeToValuelist<meta::typelist::PopFront<List>, MetaFun>,
               MetaFun<meta::typelist::Front<List>>::value> {};

template <typename List, template <typename> class MetaFun>
struct TypeToValuelistT<List, MetaFun, false>
  : meta::IdentityT<Valuelist<std::decay_t<decltype(MetaFun<meta::typelist::Front<List>>::value)>,
                    MetaFun<meta::typelist::Front<List>>::value>> {};


}   // namespace valuelist
}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // VALUELIST_H_284454785552729487230342037114028595310485 
