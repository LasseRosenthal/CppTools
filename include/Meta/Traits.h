/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Traits.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    04.06.2021 
 */ 
 
#ifndef TRAITS_H_107621645729625892289552518922566159911908 
#define TRAITS_H_107621645729625892289552518922566159911908 
 
 
// includes
#include <type_traits>


namespace meta {


/**
 * @struct IsCallableT
 * @brief  IsCallableT is a metafunction that detects if a given type T is callable,
 *         i.e. it checks whether T has a function call operator operator().
 */
template <typename T, typename = std::void_t<>>
struct IsCallableT : std::false_type
{};

template <typename T>
struct IsCallableT<T, std::void_t<decltype(&T::operator())>>
  : std::true_type
{};

template <typename T>
constexpr bool IsCallable = IsCallableT<T>::value;

/**
 * @struct IsDurationT
 * @brief  IsDurationT is a metafunction that detects if a given type T is a std::duration,
 *         i.e. it checks whether T has member types rep and period.
 */
template <typename T, typename = std::void_t<>>
struct IsDurationT : std::false_type
{};

template <typename T>
struct IsDurationT<T, std::void_t<typename T::rep, typename T::period>>
  : std::true_type
{};

template <typename T>
constexpr bool IsDuration = IsDurationT<T>::value;


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TRAITS_H_107621645729625892289552518922566159911908 
