/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitField.h 
 * @brief   Definition of the BitField class.
 * 
 * @author  Lasse Rosenthal 
 * @date    06.10.2020 
 */ 
 
#ifndef BITFIELD_H_3135949215333223876395151633011186983461 
#define BITFIELD_H_3135949215333223876395151633011186983461 
 
 
// includes
#include <Utils/miscellaneous.h>

#include <cstddef>


namespace cpptools {

 
/** 
 * @class BitField 
 * @brief BitField represents a fixed size set of bits.
 */
template <std::size_t Size, std::size_t Index = 0Ull>
class BitField {

template <std::size_t Size>
using MinimumType = std::conditional_t<Size == 0ULL, void,
                    std::conditional_t<Size <= 8ULL, std::uint8_t,
                    std::conditional_t<Size <= 16ULL, std::uint16_t,
                    std::conditional_t<Size <= 32ULL, std::uint32_t,
                    std::conditional_t<Size <= 64ULL, std::uint64_t, void>>>>>;

public:

  // ---------------------------------------------------
  // public types
  using value_type = MinimumType<Index + Size>;

  static constexpr value_type mask = (1U << Size) - 1U;
  static constexpr value_type shiftedMask = mask << Index;

  // ---------------------------------------------------
  // constructor
  constexpr BitField (value_type v);
  constexpr BitField (BitField const& src);
  auto operator=     (value_type) noexcept -> BitField&;

  constexpr operator value_type () const noexcept;
  constexpr auto operator[]     (std::size_t position) const noexcept -> bool;
 
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto put (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;

private:

  // ---------------------------------------------------
  // private data
  value_type value;

  // ---------------------------------------------------
  // auxiliary methods
  static constexpr auto getValue(value_type v) noexcept -> value_type;
};


/**
 * @brief Constructor. Initializes the bits with the binary representation of the given value.
 */
template <std::size_t Size, std::size_t Index>
constexpr BitField<Size, Index>::BitField(value_type v)
  : value {getValue(v)}
{}

/**
 * @brief Copy Constructor.
 */
template <std::size_t Size, std::size_t Index>
constexpr BitField<Size, Index>::BitField(BitField<Size, Index> const& src)
  : value {getValue(src.value)}
{}

/**
 * @brief Assignment operator.
 */
template <std::size_t Size, std::size_t Index>
inline auto BitField<Size, Index>::operator=(value_type v) noexcept -> BitField&
{
  value = getValue(v);
  return *this;
}

/**
 * @brief Conversion operator.
 */
template <std::size_t Size, std::size_t Index>
constexpr BitField<Size, Index>::operator value_type() const noexcept
{ 
  return (value >> Index) & mask;
}

/**
 * @brief  Accesses the bit at a given position.
 * @return returns true if the bit is set, false otherwise.
 */
template <std::size_t Size, std::size_t Index>
constexpr auto BitField<Size, Index>::operator[](std::size_t position) const noexcept -> bool
{
  return value & (1U << (Index + position));
}


template <std::size_t Size, std::size_t Index>
constexpr auto BitField<Size, Index>::getValue(value_type v) noexcept -> value_type
{
  return (v & ~shiftedMask) | ((v & mask) << Index);
}

/**
 * @brief Prints the binary representation of the BitField into a given stream.
 */
template <std::size_t Size, std::size_t Index>
template <typename CharT, typename CharTraits>
inline auto BitField<Size, Index>::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
  cpptools::binaryRep(static_cast<value_type>(value & shiftedMask), ostr);
  return ostr;
}

/**
 * @brief Stream operator for bit fields.
 */
template <std::size_t Size, std::size_t Index, typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, BitField<Size, Index> const& b) -> std::basic_ostream<CharT, CharTraits>&
{
  return b.put(ostr);
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELD_H_3135949215333223876395151633011186983461 
