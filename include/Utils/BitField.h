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

#include <cmath>
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
  using size_type       = std::size_t;
  using reference       = BitProxy<false>;
  using const_reference = BitProxy<true>;
  using iterator        = BitFieldIterator<false>;
  using const_iterator  = BitFieldIterator<true>;

  // ---------------------------------------------------
  // public constants
  static constexpr size_type size     = Size;
  static constexpr size_type firstBit = StartBit;

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
  auto           begin          () noexcept -> iterator;
  auto           end            () noexcept -> iterator;
  constexpr auto begin          () const noexcept -> const_iterator;
  constexpr auto end            () const noexcept -> const_iterator;
  constexpr auto cbegin         () const noexcept -> const_iterator;
  constexpr auto cend           () const noexcept -> const_iterator;


 
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

  template <bool>
  friend class BitField<Size, StartBit>::BitFieldIterator;

  // ---------------------------------------------------
  // private constants and types
  static constexpr bool isConst = IsConst;
  static constexpr auto cutter  = BitField<Size, StartBit>::cutter;
  using value_type              = typename BitField<Size, StartBit>::value_type;
  using reference               = std::conditional_t<isConst, value_type const&, value_type&>;
  using size_type               = typename BitField<Size, StartBit>::size_type;
  using difference_type         = std::ptrdiff_t;


public:

  // ---------------------------------------------------
  // constructor
  constexpr BitProxy (reference val, size_type index) noexcept;
  BitProxy           (BitProxy const&) = default;

  // ---------------------------------------------------
  // access operations
  template <typename = std::enable_if_t<!isConst>>
  auto operator=           (bool b) noexcept -> BitProxy& { return setBit(b);}
  constexpr operator bool  () const noexcept;
  template <typename = std::enable_if_t<!isConst>>
  void           flip      () noexcept { *this = *this ? false : true;}
  constexpr auto isSet     () const noexcept -> bool;

private:

  // ---------------------------------------------------
  // private data
  reference  value;
  value_type mask;
  size_type  index;

  // ---------------------------------------------------
  // private methods
  auto                  setBit   (bool b) noexcept -> BitProxy&;
  void                  setMask  (size_type index) noexcept;
  void                  advance  (difference_type n) noexcept;
  void                  setIndex (size_type i) noexcept;
  constexpr static auto getMask  (size_type index) noexcept -> value_type;
};

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitProxy<IsConst>::BitProxy(reference val, size_type ind) noexcept
  : value {val}
  , mask  {getMask(ind)}
  , index {ind}
{}

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
 * @brief Assigns a bool to the referenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitProxy<IsConst>::setBit(bool b) noexcept -> BitProxy&
{
  const value_type tmp = b ? (value & cutter) | mask : (value & cutter) & ~mask;
  value                = (value & ~cutter) | tmp;
  return *this;
}



template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline void BitField<Size, StartBit>::BitProxy<IsConst>::setMask(size_type index) noexcept
{
  mask = getMask(index);
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline void BitField<Size, StartBit>::BitProxy<IsConst>::advance(difference_type n) noexcept
{
  index += n;
  mask = getMask(index);
}


template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline void BitField<Size, StartBit>::BitProxy<IsConst>::setIndex(size_type i) noexcept
{
  index = i;
  mask = getMask(index);
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr auto BitField<Size, StartBit>::BitProxy<IsConst>::getMask(size_type index) noexcept -> value_type
{
  return 1U << (StartBit + index);
}




template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
class BitField<Size, StartBit>::BitFieldIterator {

  static constexpr bool isConst = IsConst;
  static constexpr auto size    = Size;
  using underlyingValueType     = typename BitField<Size, StartBit>::value_type;

public:

  using iterator_category = std::forward_iterator_tag;
  using value_type        = BitField<Size, StartBit>::BitProxy<IsConst>;
  using size_type         = BitField<Size, StartBit>::size_type;
  using difference_type   = std::ptrdiff_t;
  using pointer           = std::conditional_t<IsConst, value_type const*, value_type*>;
  using reference         = std::conditional_t<IsConst, value_type const&, value_type&>;


  template <typename = std::enable_if_t<isConst>>
  constexpr auto operator*() const noexcept -> reference { return proxy;}
  template <typename = std::enable_if_t<!isConst>>
  auto operator*() noexcept -> reference { return proxy;}

  //auto operator-> () const -> pointer;
  auto operator++ () noexcept -> BitFieldIterator&;
  auto operator++ (int) noexcept -> BitFieldIterator&;
  auto operator+= (difference_type n) noexcept -> BitFieldIterator&;
  auto operator-= (difference_type n) noexcept -> BitFieldIterator&;


  //friend auto operator<  (BitFieldIterator const&, BitFieldIterator const&) noexcept -> bool;
  //friend auto operator>  (BitFieldIterator const&, BitFieldIterator const&) noexcept -> bool;

  //template <std::size_t S, std::size_t SB, bool C>
  //friend auto operator== (BitField<S, SB>::BitFieldIterator<C> const& it1, BitField<S, SB>::BitFieldIterator<C> const& it2) noexcept -> bool
  //{
  //  return (&it1.value == &it2.value && it1.index == it2.index);
  //}



  friend auto operator< (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return it1.index < it2.index;
  }
  friend auto operator> (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return it2 < it1;
  }
  friend auto operator<= (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return !(it1 > it2);
  }
  friend auto operator>= (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return !(it1 < it2);
  }
  friend auto operator== (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return (&it1.value == &it2.value && it1.index == it2.index);
  }
  friend auto operator!= (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return !(it1 == it2);
  }


  constexpr BitFieldIterator(underlyingValueType& val, size_type i) noexcept;

private:

  // ---------------------------------------------------
  // private data
  underlyingValueType& value;
  size_type            index;
  value_type           proxy;
  mutable bool         atEnd;

  // ---------------------------------------------------
  // private methods
  constexpr auto isAtEnd() const noexcept -> bool;
  void           advance(difference_type n) noexcept;
};


template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitFieldIterator<IsConst>::BitFieldIterator(underlyingValueType& val, size_type i) noexcept
  : value {val}
  , index {std::min(i, size)}
  , proxy {value, index}
  , atEnd {isAtEnd()}
{}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator++() noexcept -> BitFieldIterator&
{
  advance(1LL);
  return *this;
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator++(int) noexcept -> BitFieldIterator&
{
  BitFieldIterator copy{*this};
  return ++copy;
}


template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator+=(difference_type n) noexcept -> BitFieldIterator&
{
  advance(n);
  return *this;
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator-=(difference_type n) noexcept -> BitFieldIterator&
{
  return *this += -n;
}


template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::isAtEnd() const noexcept -> bool
{
  return index >= size;
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline void BitField<Size, StartBit>::BitFieldIterator<IsConst>::advance(difference_type n) noexcept
{
  if(n > 0LL && !atEnd)
  {
    index += n;
    if(!isAtEnd())
    {
      proxy.advance(n);
    }
    else
    {
      atEnd = true;
    }
  }
  else if (n < 0LL)
  {
    if(std::abs(n) <= static_cast<difference_type>(index))
    {
      index -= n;
      proxy.advance(n);
    }
    else
    {
      index = 0ULL;
      proxy.setIndex(index);
    }
  }
}


//template <std::size_t Size, std::size_t StartBit, bool IsConst>
//inline auto operator<(BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it1,
//                      BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it2) noexcept -> bool
//{
//  return it1.index < it2.index;
//}
//
//template <std::size_t Size, std::size_t StartBit, bool IsConst>
//inline auto operator>(BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it1,
//                      BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it2) noexcept -> bool
//{
//  it2 < it1;
//}

//template <std::size_t Size, std::size_t StartBit, bool IsConst>
//inline auto operator==(BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it1,
//                       BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it2) noexcept -> bool
//{
//  return (&it1.value == &it2.value && it1.index == it2.index);
//}
//
//template <std::size_t Size, std::size_t StartBit, bool IsConst>
//inline auto operator!=(BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it1,
//                       BitField<Size, StartBit>::BitFieldIterator<IsConst> const& it2) noexcept -> bool
//{
//  return !(it1 == it2);
//}






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
}

template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::operator[](std::size_t index) noexcept -> reference
{
  return reference(value, index);
}


/**
 * @brief Begin method. returns a non-const begin iterator.
 */
template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::begin() noexcept -> iterator
{
  return iterator(value, 0ULL);
}

/**
 * @brief Returns a non-const end iterator.
 */
template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::end() noexcept -> iterator
{
  return iterator(value, size);
}

/**
 * @brief Returns a const begin iterator.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::begin() const noexcept -> const_iterator
{
  return const_iterator(value, 0ULL);
}

/**
 * @brief Returns a const end iterator.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::end() const noexcept -> const_iterator
{
  return const_iterator(value, size);
}

/**
 * @brief Returns a const begin iterator.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::cbegin() const noexcept -> const_iterator
{
  return const_iterator(value, 0ULL);
}

/**
 * @brief Returns a const end iterator.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::cend() const noexcept -> const_iterator
{
  return const_iterator(value, size);
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
