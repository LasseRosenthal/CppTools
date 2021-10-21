/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Bitwise.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.08.2021 
 */ 
 
#ifndef BITWISE_H_19024254234409316041227258255056394925205 
#define BITWISE_H_19024254234409316041227258255056394925205 
 
 
// includes
#include <cstdint>
#include <ostream>
#include <type_traits>
 

namespace bws {


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
  for(std::size_t byte{ 1ULL }; byte <= sizeInBytes; ++byte)
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

/**
 * @brief  generates a bitmask with a given size and a given startbit
 * @tparam T the underlying type of the bitmask
 * @param  size the number of bits that are set
 * @param  startBit zero based index of the first bit that is set.
 *         Indexing starts with the least significant bit.
 */
template <typename T>
[[nodiscard]] constexpr auto punchMask(std::size_t const size,
                                       std::size_t const startBit = 0ULL) noexcept -> T
{
  if(size + startBit >= sizeof(T) * 8ULL)
  {
    return static_cast<T>(~T{} << startBit);
  }
  else
  {
    return ((static_cast<T>(1) << size) - 1) << startBit;
  }
}

/**
 * @brief  Counts the number of bits set in a give value v.
 * @tparam T the type of the value.
 */
template <typename T>
[[nodiscard]] constexpr auto countBits(T v) noexcept -> std::enable_if_t<!std::is_signed_v<T>, std::size_t>
{
  v = v - ((v >> 1) & (T) ~(T)0 / 3);
  v = (v & (T) ~(T)0 / 15 * 3) + ((v >> 2) & (T) ~(T)0 / 15 * 3);
  //v = (v + (v >> 4)) & (T) ~(T)0 / 255 * 15;
  //return (T)(v * ((T) ~(T)0 / 255)) >> ((sizeof(T) - 1) * CHAR_BIT);

  return (T)(((v + (v >> 4)) & (T) ~(T)0 / 255 * 15) * ((T) ~(T)0 / 255)) >> ((sizeof(T) - 1ULL) * CHAR_BIT);
}

template <typename T>
[[nodiscard]] constexpr auto countBits(T v) noexcept -> std::enable_if_t<std::is_signed_v<T>, std::size_t>
{
  std::size_t c{};
  for(; v; v >>= 1)
  {
    c += v & 1;
  }

  return c;
}

template <typename T>
[[nodiscard]] auto firstBitSet(T v) noexcept -> std::size_t
{
  std::size_t c{};
  if(v & 0x1)
  {
    // special case for odd v (assumed to happen half of the time)
    c = 0;
  }
  else
  {
    c = 1;
    if((v & 0xffff) == 0)
    {
      v >>= 16;
      c += 16;
    }
    if((v & 0xff) == 0)
    {
      v >>= 8;
      c += 8;
    }
    if((v & 0xf) == 0)
    {
      v >>= 4;
      c += 4;
    }
    if((v & 0x3) == 0)
    {
      v >>= 2;
      c += 2;
    }
    c -= v & 0x1;
  }

  return c;

  //std::size_t c{};
  //while(!(v & T{1}))
  //{
  //  ++c;
  //  v >>= 1;
  //}

  //return c;
}


}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITWISE_H_19024254234409316041227258255056394925205 
