/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Utility.h 
 * @brief   Utility defines a collection of utility meta functions.
 * 
 * @author  Lasse Rosenthal 
 * @date    24.06.2020 
 */ 
 
#ifndef UTILITY_H_119847052187393170611138573113858306010428 
#define UTILITY_H_119847052187393170611138573113858306010428 
 
 
// includes
#include <type_traits>
 
 
namespace meta {


/** 
 * @struct IdentityT 
 * @brief  IdentityT can be used to generate metafunctions providing
 *         a type alias.
 */
template <typename T>
struct IdentityT {
  using type = T;
};

template <typename T>
using Identity = typename IdentityT<T>::type;

/** 
 * @struct FirstTypeOfT 
 * @brief  FirstTypeOfT provides an alias for the first type in a parameter pack
 */
template <typename... Ts>
struct FirstTypeOfT;

template <typename T, typename... Ts>
struct FirstTypeOfT<T, Ts...> : meta::IdentityT<T> {};

template <typename... Ts>
using FirstTypeOf = typename FirstTypeOfT<Ts...>::type;

/** 
 * @struct InvalidType 
 * @brief  InvalidType can be used as a return type indicating
 *         out of range errors.
 */
struct InvalidType {};


/**
 * @brief Boolean constant to determine if a number is a power of two.
 */
template <std::size_t N>
constexpr auto IsPowerOfTwo = std::bool_constant<N && !(N & (N - 1ULL))>::value;


/**
 * @brief unaryPredConjunction os an extension of unary predicates to an arbitrary number of types.
 */
template <template <typename> class Pred, typename... Ts>
struct unaryPredConjunction;

template <template <typename> class Pred, typename T>
struct unaryPredConjunction<Pred, T> : std::integral_constant<bool, Pred<T>::value>
{};

template <template <typename> class Pred, typename T1, typename... Ts>
struct unaryPredConjunction<Pred, T1, Ts...> : std::integral_constant<bool, Pred<T1>::value&& unaryPredConjunction<Pred, Ts...>::value>
{};


/**
 * @brief  Extension of binary predicates to an arbitrary number of types.
 * @remark The predicate is assumed to define a transitive relation.
 */
template <template <typename, typename> class BinaryPredicate, typename... Ts>
struct binaryPredConjunction;

template <template <typename, typename> class BinaryPredicate, typename T1, typename T2>
struct binaryPredConjunction<BinaryPredicate, T1, T2> : std::integral_constant<bool, BinaryPredicate<T1, T2>::value>
{};

template <template <typename, typename> class BinaryPredicate, typename T1, typename T2, typename... Ts>
struct binaryPredConjunction<BinaryPredicate, T1, T2, Ts...> :
  std::integral_constant<bool, BinaryPredicate<T1, T2>::value&& binaryPredConjunction<BinaryPredicate, T2, Ts...>::value>
{};


/**
 * @brief Variadic Extension of std::is_integral.
 */
template <typename... Us>
using isIntegralT = unaryPredConjunction<std::is_integral, Us...>;

template <typename... Ts>
constexpr bool isIntegral = isIntegralT<Ts...>::value;

/**
 * @brief Variadic Extension of std::is_arithmetic.
 */
template <typename... Us>
using isArithmeticT = unaryPredConjunction<std::is_arithmetic, Us...>;

template <typename... Ts>
constexpr bool isArithmetic = isArithmeticT<Ts...>::value;

/**
 * @brief Variadic Extension of std::is_floating_point.
 */
template <typename... Us>
using isFloatingPointT = unaryPredConjunction<std::is_floating_point, Us...>;

template <typename... Ts>
constexpr bool isFloatingPoint = isFloatingPointT<Ts...>::value;

/**
 * @brief Variadic Extension of std::is_same.
 */
template <typename... Ts>
using allSameT = binaryPredConjunction<std::is_same, Ts...>;

template <typename... Ts>
constexpr bool allSame = allSameT<Ts...>::value;


/** 
 * @struct IsOneOfT 
 * @brief  IsOneOfT a recursive variadic template that checks if
 *         the first template parameter is identical to one of the
 *         following parameters.
 */
template <typename... Ts>
struct IsOneOfT : std::false_type
{};

template <typename T1, typename T2, typename... Ts>
struct IsOneOfT<T1, T2, Ts...> : std::bool_constant<std::is_same_v<T1, T2> || IsOneOfT<T1, Ts...>::value>
{};

template <typename... Ts>
constexpr bool isOneOf = IsOneOfT<Ts...>::value;


/** 
 * @struct allConvertible 
 * @brief  allConvertible a recursive variadic template that checks if
 *         all parameters of a parameter pack are convertible to the first
 *         type in the pack.
 */
template <typename... Ts>
struct AllConvertibleT : std::false_type
{};

template <typename... Ts>
constexpr bool AllConvertible = AllConvertibleT<Ts...>::value;

template <typename T>
struct AllConvertibleT<T> : std::true_type
{};

template <typename T1, typename T2, typename... Ts>
struct AllConvertibleT<T1, T2, Ts...> : std::bool_constant<std::is_convertible_v<T2, T1> && AllConvertible<T1, Ts...>>
{};


/** 
 * @struct ConstRefOrValueT 
 * @brief  ConstRefOrValueT a metafunction that maps a given type T
 *         on T for arithmetic types and T const& for non-arithmetic types.
 */
template <typename T, typename Enable = void>
struct ConstRefOrValueT;

template <typename T>
using ConstRefOrValue = typename ConstRefOrValueT<T>::type;

template <typename T>
struct ConstRefOrValueT<T, std::enable_if_t<!std::is_arithmetic_v<T>>> : IdentityT<T const&> {};

template <typename T>
struct ConstRefOrValueT<T, std::enable_if_t<std::is_arithmetic_v<T>>> : IdentityT<T> {};


/** 
 * @struct RefOrValueT 
 * @brief  RefOrValueT a metafunction that maps a given type T
 *         on T for arithmetic types and T& for non-arithmetic types.
 */
template <typename T, typename Enable = void>
struct RefOrValueT;

template <typename T>
using RefOrValue = typename RefOrValueT<T>::type;

template <typename T>
struct RefOrValueT<T, std::enable_if_t<!std::is_arithmetic_v<T>>> : IdentityT<T&> {};

template <typename T>
struct RefOrValueT<T, std::enable_if_t<std::is_arithmetic_v<T>>> : IdentityT<T> {};


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UTILITY_H_119847052187393170611138573113858306010428 
