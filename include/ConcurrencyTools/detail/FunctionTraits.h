/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FunctionTraits.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    24.03.2021 
 */ 
 
#ifndef FUNCTIONTRAITS_H_25757194481342528313142991576428119997413142 
#define FUNCTIONTRAITS_H_25757194481342528313142991576428119997413142 
 
 
// includes
#include <tuple>
#include <type_traits>
 
 
namespace cctools {
namespace detail {


/** 
 * @struct FunctionTraits 
 * @brief  FunctionTraits is a traits providing information about signatures
 *         of class member functions.
 */
template <typename T, typename = void>
struct FunctionTraits;

template <typename T>
struct FunctionTraits<T, std::enable_if_t<std::is_class_v<T>>> : FunctionTraits<decltype(&T::operator())> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType (ClassType::*)(Args...)> {
  using result_type = ReturnType;
  template <std::size_t index>
  using argument_type = std::tuple_element_t<index, std::tuple<Args...>>;

  static constexpr bool        is_const_method = false;
  static constexpr std::size_t arity           = sizeof...(Args);
};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType (ClassType::*)(Args...) const> {
  using result_type = ReturnType;
  template <std::size_t index>
  using argument_type = std::tuple_element_t<index, std::tuple<Args...>>;

  static constexpr bool        is_const_method = true;
  static constexpr std::size_t arity           = sizeof...(Args);
};


}   // namespace detail
}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FUNCTIONTRAITS_H_25757194481342528313142991576428119997413142 
