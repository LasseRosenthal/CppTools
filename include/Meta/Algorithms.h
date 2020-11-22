/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Algorithms.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    22.11.2020 
 */ 
 
#ifndef ALGORITHMS_H_62391763420405790782822441175171792931153 
#define ALGORITHMS_H_62391763420405790782822441175171792931153 
 
 
// includes
#include <array>


namespace meta {


/** 
 * @brief  accumulate computes the generalized sum of init and the values
 *         of a given std::array at compile time.
 */
template <typename T, std::size_t N, typename F, std::size_t... Is>
auto constexpr accumulate(std::array<T, N> const& arr, F&& f, std::index_sequence<Is...>, T init) noexcept -> T
{
  ([&init, &f](auto const x) { std::forward<F>(f)(init, x); }(arr[Is]), ...);
  return init;
}

template <typename T, std::size_t N, typename F>
auto constexpr accumulate(std::array<T, N> const& arr, F&& f, T init = T{}) noexcept -> T
{
  return accumulate<T, N>(arr, std::forward<F>(f), std::make_index_sequence<N>(), init);
}


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ALGORITHMS_H_62391763420405790782822441175171792931153 
