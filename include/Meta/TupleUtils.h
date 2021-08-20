/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TupleUtils.h 
 * @brief   a collection of functions dealing with std::tuple
 * 
 * @author  Lasse Rosenthal 
 * @date    05.08.2021 
 */ 
 
#ifndef TUPLEUTILS_H_15476128571788717726322518638300331755812233 
#define TUPLEUTILS_H_15476128571788717726322518638300331755812233 
 
 
// includes
#include <cstddef>
#include <tuple>
#include <utility>


namespace meta {


/** 
 * @brief Applies a given function to each element of a given tuple
 * @param t the tuple
 * @param f the callable that is invoked for every element of the tuple
 */
template <typename F, typename... Ts>
inline auto forEach(std::tuple<Ts...> const& t, F&& f)
{
  return forEach(t, std::make_index_sequence<sizeof...(Ts)>(), std::forward<F>(f));
}

template <typename F, typename... Ts, std::size_t... Is>
inline auto forEach(std::tuple<Ts...> const& t, std::index_sequence<Is...>, F&& f)
{
  (..., f(std::get<Is>(t)));
  return std::forward<F>(f);
}

/** 
 * @brief  Computes a generalized sum of the elements within a given range of a tuple.
 * @param  t the tuple
 * @param  op binary operation object that is applied.
 * @tparam Begin an index denoting the first element in the range.
 * @tparam End an index denoting the end of the range.
 */
template <std::size_t Begin, std::size_t End, typename BinaryOp, typename... Ts>
constexpr auto accumulate(std::tuple<Ts...> const& t, BinaryOp&& op)
{
  return detail::TupleAccumulator<Begin, End - 1ULL, BinaryOp, Ts...>::accumulate(t, std::forward<BinaryOp>(op));
}

/** 
 * @brief Computes a generalized sum of all elements of a given tuple.
 * @param t the tuple
 * @param op binary operation object that is applied.
 */
template <typename BinaryOp, typename... Ts>
constexpr auto accumulate(std::tuple<Ts...> const& t, BinaryOp&& op)
{
  return accumulate<0ULL, sizeof...(Ts)>(t, std::forward<BinaryOp>(op));
}

namespace detail {


template <std::size_t Index, std::size_t MaxIndex, typename BinaryOp, typename... Ts>
struct TupleAccumulator
{
  static constexpr auto accumulate(std::tuple<Ts...> const& t, BinaryOp&& op)
  {
    return op(std::get<Index>(t), TupleAccumulator<Index + 1ULL, MaxIndex, BinaryOp, Ts...>::accumulate(t, std::forward<BinaryOp>(op)));
  }
};

template <std::size_t MaxIndex, typename BinaryOp, typename... Ts>
struct TupleAccumulator<MaxIndex, MaxIndex, BinaryOp, Ts...>
{
  static constexpr auto accumulate(std::tuple<Ts...> const& t, BinaryOp&&)
  {
    return std::get<MaxIndex>(t);
  }
};

}   // namespace detail


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TUPLEUTILS_H_15476128571788717726322518638300331755812233 
