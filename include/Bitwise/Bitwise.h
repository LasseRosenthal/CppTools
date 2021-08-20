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


}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITWISE_H_19024254234409316041227258255056394925205 
