/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    miscellaneous.h 
 * @brief   a loose collection of small useful functions
 * 
 * @author  Lasse Rosenthal 
 * @date    29.09.2020 
 */ 
 
#ifndef MISCELLANEOUS_H_241921507466663734281853239619697318654075 
#define MISCELLANEOUS_H_241921507466663734281853239619697318654075 
 
 
// includes
#include <Meta/Utility.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <ostream>
#include <type_traits>
#include <vector>


namespace cpptools {


/** 
 * @brief returns the minimum of an arbitrary number of elements of different types that
 *        are comparable via the comparison operator <
 */
template <typename T>
[[nodiscard]] constexpr T min(T&& t)
{
  return std::forward<T>(t);
}

template <typename T1, typename T2, typename... Ts>
[[nodiscard]] constexpr auto min(T1&& head1, T2&& head2, Ts&&... tail) -> std::common_type_t<T1, T2, Ts...>
{
  return head1 < head2 ? min(std::forward<T1>(head1), std::forward<Ts>(tail)...)
                       : min(std::forward<T2>(head2), std::forward<Ts>(tail)...);
}

/** 
 * @brief returns the maximum of an arbitrary number of elements of different types that
 *        are comparable via the comparison operator >
 */
template <typename T>
[[nodiscard]] constexpr T max(T&& t)
{
  return std::forward<T>(t);
}

template <typename T1, typename T2, typename... Ts>
[[nodiscard]] constexpr auto max(T1&& head1, T2&& head2, Ts&&... tail) -> std::common_type_t<T1, T2, Ts...>
{
  return head1 > head2 ? max(std::forward<T1>(head1), std::forward<Ts>(tail)...)
                       : max(std::forward<T2>(head2), std::forward<Ts>(tail)...);
}

/**
 * @brief   aligns a value to a given alignment (templated version)
 * @tparam  Alignment the alignment
 * @param   n the value that is aligned to alignment
 * @returns the aligned version of n
 */
template <std::size_t Alignment>
constexpr auto alignUp(std::size_t n) noexcept -> std::enable_if_t<meta::IsPowerOfTwo<Alignment>, std::size_t>
{
  return (n + (Alignment - 1ULL)) & ~(Alignment - 1ULL);
}

template <std::size_t Alignment>
constexpr auto alignUp(std::size_t n) noexcept -> std::enable_if_t<!meta::IsPowerOfTwo<Alignment>, std::size_t>
{
  return ((n + Alignment - 1ULL) / Alignment) * Alignment;
}

/**
 * @brief   aligns a value to a given alignment
 * @param   n the value that is that has to be aligned
 * @param   alignment
 * @returns the aligned version of n
 */
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline auto alignUp(const T n, const T alignment) -> T
{
  return ((n + alignment - 1) / alignment) * alignment;
}

/**
 * @brief   returns a vector containing all keys of the given map.
 * @param   the map whose keys are stored in a vector.
 * @returns a vector containing the keys of the given map.
 * @remark  the size and capacity of the returned vector equals the size of the map.
 */
template <typename Map>
auto getMapKeys(Map const& m) -> std::vector<typename Map::key_type>
{
  std::vector<typename Map::key_type> v;
  v.reserve(m.size());
  std::transform(m.begin(), m.end(), std::back_inserter(v), [](typename Map::value_type const& val) { return val.first; });
  return v;
}

/**
 * @brief  Computes the number of decimal places of a given floating point.
 * @remark due to the nature of binary floating point representation, the
 *         return value might incorrect due to rounding errors.
 */
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
auto numberOfDecimalPlaces(T value) -> std::size_t
{
  if constexpr(std::is_floating_point_v<T>)
  {
    T tmp           = value - std::floor(value);
    std::size_t ctr = 0ULL;
    T factor        = 10.0;
    while(tmp > 0.0 && ctr < std::numeric_limits<T>::max_digits10)
    {
      tmp = value * factor;
      tmp -= std::floor(tmp);
      factor *= 10.0;
      ++ctr;
    }

    return ctr;
  }
  else
  {
    return 0ULL;
  }
}

/**
 * @brief  prints the binary representation of an arbitrary object into a given stream.
 * @tparam T the type of the object
 * @param  value the object whose binary representation is printed.
 * @param  ostr a reference to the ostream object into which the representation is streamed.
 * @remark works only on little-endian architectures.
 */
template <typename T, typename CharT, typename CharTraits = std::char_traits<CharT>>
auto binaryRep(const T& value, std::basic_ostream<CharT, CharTraits>& ostr)
{
  constexpr std::size_t byteSize = 8ULL;
  constexpr std::size_t sizeInBytes = sizeof(T);
  auto rawPtr = reinterpret_cast<const char*>(&value);
  CharT bitArray[sizeInBytes * byteSize];
  auto bitPtr = std::prev(std::end(bitArray));

  for(std::size_t byte{}; byte < sizeInBytes; ++byte, ++rawPtr)
  {
    for(std::size_t bit{}; bit < byteSize; ++bit, --bitPtr)
    {
      *bitPtr = (*rawPtr >> bit) & 0b0001;
    }
  }

  ++bitPtr;
  const auto space = ostr.widen(' ');
  for(std::size_t byte{1ULL}; byte <= sizeInBytes; ++byte)
  {
    for(std::size_t bit{}; bit < byteSize; ++bit, ++bitPtr)
    {
      ostr << static_cast<int>(*bitPtr);
    }

    if(byte != sizeInBytes)
    {
      ostr << space;
    }
  }
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MISCELLANEOUS_H_241921507466663734281853239619697318654075 
