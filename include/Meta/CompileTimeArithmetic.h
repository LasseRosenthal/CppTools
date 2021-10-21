/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    CompileTimeArithmetic.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.11.2020 
 */ 
 
#ifndef COMPILETIMEARITHMETIC_H_319681283711677571310392245422551057217578 
#define COMPILETIMEARITHMETIC_H_319681283711677571310392245422551057217578 
 
 
// includes
#include <type_traits>
#include <utility>


namespace meta {


/** 
 * @brief  Power is a compile method to compute the power of a given number
 *         to a given exponent.
 */
template <typename T, int... Ns>
auto constexpr Power(T const x, std::integer_sequence<int, Ns...>) noexcept -> T
{
  return (... * (x * [](auto) { return 1.0; }(Ns)));
}

template <int N, typename T>
auto constexpr Power(T const x) noexcept -> std::enable_if_t<(N > 0), T>
{
  return Power<T>(x, std::make_integer_sequence<int, N>());
}

template <int N, typename T>
auto constexpr Power(T const x) noexcept -> std::enable_if_t<(N < 0), T>
{
  return 1.0 / Power<T>(x, std::make_integer_sequence<int, -N>());
}

template <int N, typename T>
auto constexpr Power(T const x) noexcept -> std::enable_if_t<(N == 0), T>
{
  return 1.0;
}

/** 
 * @struct  NthRoot
 * @brief   NthRoot is a compile implementation to compute the nth root
 *          of a given number.
 * @remarks the computation is based on the Newton method.
 * @see     https://en.wikipedia.org/wiki/Newton%27s_method
 */
template <std::size_t N, typename T>
auto constexpr NthRoot(T const a, T xn, int ctr = 1) -> T
{
//  static constexpr int recursionDepth = 400;
  if(ctr == 400)
  {
    return xn;
  }

  // x_n+1 = x_n - f(x_n) / f'(x_n)
  // for f(x) = x^m - a, we get
  // x_n+1 = x_n - ((x_n)^m - a) / (m*(x_n)^(m-1))
  //       = x_n - (x_n)/m + a / (m*(x_n)^(m-1))
  //       = ((m-1)*x_n + a / (x_n)^(m-1)) / m

  xn = ((N - 1) * xn + a / Power<N-1>(xn)) / N;
  return NthRoot<N>(a, xn, ++ctr);
}

template <std::size_t N, typename T>
auto constexpr NthRoot(T const x) -> std::enable_if_t<(N > 1), T>
{
  return NthRoot<N>(x, x);
}

template <std::size_t N, typename T, typename = std::enable_if_t<(N == 1)>>
auto constexpr NthRoot(T const x) -> std::enable_if_t<(N == 1), T>
{
  return x;
}


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // COMPILETIMEARITHMETIC_H_319681283711677571310392245422551057217578 
