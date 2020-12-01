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
#include <iterator>


namespace cpptools {

 
/** 
 * @class BitField 
 * @brief BitField represents a fixed size set of bits.
 */
template <std::size_t Size, std::size_t StartBit = 0Ull>
class BitField {

  template <std::size_t Size>
  using MinimumType = std::conditional_t<Size == 0ULL, void,
                      std::conditional_t<Size <= 8ULL, std::uint8_t,
                      std::conditional_t<Size <= 16ULL, std::uint16_t,
                      std::conditional_t<Size <= 32ULL, std::uint32_t,
                      std::conditional_t<Size <= 64ULL, std::uint64_t, void>>>>>;

  template <bool IsConst>
  class BitProxy;

  template <bool IsConst>
  class BitFieldIterator;

public:

  // ---------------------------------------------------
  // public types
  using value_type      = MinimumType<StartBit + Size>;
  using reference       = BitProxy<false>;
  using const_reference = BitProxy<true>;

  // ---------------------------------------------------
  // constructor
  constexpr BitField (value_type v);
  constexpr BitField (BitField const& src);
  auto operator=     (value_type) noexcept -> BitField&;

  // ---------------------------------------------------
  // access operations
  constexpr operator value_type () const noexcept;
  constexpr auto operator[]     (std::size_t position) const noexcept -> const_reference;
  auto           operator[]     (std::size_t position) noexcept -> reference;

 
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto put (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;

  constexpr auto getBitProxy(std::size_t index) const noexcept -> BitProxy<true>;
  auto getBitProxy(std::size_t index) noexcept -> BitProxy<false>;

private:

  static constexpr value_type mask = (1U << Size) - 1U;   // BitField<3, 2> --> 0000 0111
  static constexpr value_type cutter = mask << StartBit;  // BitField<3, 2> --> 0001 1100

  // ---------------------------------------------------
  // private data
  value_type value;

  // ---------------------------------------------------
  // auxiliary methods
  static constexpr auto getValue(value_type v) noexcept -> value_type;
};


/** 
 * @class BitProxy
 * @brief BitProxy is a proxy class defined in class \link #BitField \endlink
 *        that is used to interact with individual bits.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
class BitField<Size, StartBit>::BitProxy {

  // ---------------------------------------------------
  // private constants and types
  static constexpr bool isConst = IsConst;
  static constexpr auto cutter  = BitField<Size, StartBit>::cutter;
  using value_type              = typename BitField<Size, StartBit>::value_type;
  using reference               = std::conditional_t<isConst, value_type const&, value_type&>;

public:

  // ---------------------------------------------------
  // constructor
  constexpr BitProxy (reference val, std::size_t index) noexcept;
  BitProxy           (BitProxy const&) = default;

  // ---------------------------------------------------
  // access operations
  auto operator=           (bool b) noexcept -> BitProxy&;
  constexpr operator bool  () const noexcept;
  void flip                () noexcept;
  constexpr auto isSet     () const noexcept -> bool;

private:

  // ---------------------------------------------------
  // private data
  reference         value;
  const value_type  mask;
};

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitProxy<IsConst>::BitProxy(reference val, std::size_t index) noexcept
  : value {val}
  , mask  {1U << (StartBit + index)}
{}

/**
 * @brief Assigns a bool to the referenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitProxy<IsConst>::operator=(bool b) noexcept -> BitProxy&
{
  const value_type tmp = b ? (value & cutter) | mask : (value & cutter) & ~mask;
  value = (value & ~cutter) | tmp;
  return *this;
}

/**
 * @brief Returns the value of the refenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr auto BitField<Size, StartBit>::BitProxy<IsConst>::isSet() const noexcept -> bool
{
  return value & mask;
}

/**
 * @brief Returns the value of the refenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitProxy<IsConst>::operator bool() const noexcept
{
  return value & mask;
}

/**
 * @brief Flips the refenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline void BitField<Size, StartBit>::BitProxy<IsConst>::flip() noexcept
{
  *this = *this ? false : true;
}


template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
class BitField<Size, StartBit>::BitFieldIterator {

  using iterator_category = std::forward_iterator_tag;
  using value_type        = BitField<Size, StartBit>::BitProxy<IsConst>;
  using difference_type   = std::ptrdiff_t;
  using pointer           = std::conditional_t<IsConst, value_type const*, value_type*>;
  using reference         = std::conditional_t<IsConst, value_type const&, value_type&>;

public:

  //auto operator*  () const -> reference;
  //auto operator-> () const -> pointer;
  //auto operator++ () -> UPTLayoutIterator&;


private:



};



/**
 * @brief Constructor. Initializes the bits with the binary representation of the given value.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr BitField<Size, StartBit>::BitField(value_type v)
  : value {getValue(v)}
{}

/**
 * @brief Copy Constructor.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr BitField<Size, StartBit>::BitField(BitField<Size, StartBit> const& src)
  : value {getValue(src.value)}
{}

/**
 * @brief Assignment operator.
 */
template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::operator=(value_type v) noexcept -> BitField&
{
  value = getValue(v);
  return *this;
}

/**
 * @brief Conversion operator.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr BitField<Size, StartBit>::operator value_type() const noexcept
{ 
  return (value >> StartBit) & mask;
}

/**
 * @brief  Accesses the bit at a given position.
 * @return returns true if the bit is set, false otherwise.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::operator[](std::size_t index) const noexcept -> const_reference
{
  return const_reference(value, index);
  //  return value & (1U << (StartBit + position));
}

template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::operator[](std::size_t index) noexcept -> reference
{
  return reference(value, index);
  //  return value & (1U << (StartBit + position));
}


template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::getValue(value_type v) noexcept -> value_type
{
  return (v & ~cutter) | ((v & mask) << StartBit);
}

template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::getBitProxy(std::size_t index) const noexcept -> BitProxy<true>
{
  BitProxy<true> p(value, index);
  return p;
}

template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::getBitProxy(std::size_t index) noexcept -> BitProxy<false>
{
  BitProxy<false> p(value, index);
  return p;
}


/**
 * @brief Prints the binary representation of the BitField into a given stream.
 */
template <std::size_t Size, std::size_t StartBit>
template <typename CharT, typename CharTraits>
inline auto BitField<Size, StartBit>::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
  cpptools::binaryRep(static_cast<value_type>(value & cutter), ostr);
  return ostr;
}

/**
 * @brief Stream operator for bit fields.
 */
template <std::size_t Size, std::size_t StartBit, typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, BitField<Size, StartBit> const& b) -> std::basic_ostream<CharT, CharTraits>&
{
  return b.put(ostr);
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELD_H_3135949215333223876395151633011186983461 
