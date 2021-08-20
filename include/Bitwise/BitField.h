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
  static constexpr value_type mask    = (static_cast<value_type>(1) << Size) - 1;   // BitField<3, 2> --> 0000 0111
  static constexpr value_type cutter  = mask << StartBit;                           // BitField<3, 2> --> 0001 1100
  static constexpr size_type firstBit = StartBit;

  // ---------------------------------------------------
  // constructor
  constexpr BitField () = default;
  constexpr BitField (value_type v) noexcept;
  constexpr BitField (BitField const& src) noexcept;
  auto operator=     (value_type) noexcept -> BitField&;

  // ---------------------------------------------------
  // logical bitwise operations
  auto operator&= (BitField const& src) noexcept -> BitField&;
  auto operator|= (BitField const& src) noexcept -> BitField&;
  auto operator^= (BitField const& src) noexcept -> BitField&;

  // ---------------------------------------------------
  // access operations
  constexpr operator value_type     () const noexcept;
  [[nodiscard]] constexpr auto size () const noexcept -> size_type;
  constexpr auto operator[]         (std::size_t position) const noexcept -> const_reference;
  auto           operator[]         (std::size_t position) noexcept -> reference;
  auto           begin              () noexcept -> iterator;
  auto           end                () noexcept -> iterator;
  constexpr auto begin              () const noexcept -> const_iterator;
  constexpr auto end                () const noexcept -> const_iterator;
  constexpr auto cbegin             () const noexcept -> const_iterator;
  constexpr auto cend               () const noexcept -> const_iterator;
 
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto put (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;

  constexpr auto getBitProxy(std::size_t index) const noexcept -> BitProxy<true>;
  auto getBitProxy(std::size_t index) noexcept -> BitProxy<false>;

private:

  // ---------------------------------------------------
  // private data
  value_type value{};

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
  using pointer                 = std::conditional_t<isConst, value_type const*, value_type*>;
  using reference               = std::conditional_t<isConst, value_type const&, value_type&>;
  using size_type               = typename BitField<Size, StartBit>::size_type;
  using difference_type         = std::ptrdiff_t;

public:

  // ---------------------------------------------------
  // constructor
  constexpr BitProxy (pointer val, size_type index) noexcept;
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
  pointer    value;
  value_type mask;
  size_type  index;

  // ---------------------------------------------------
  // private methods
  auto                  setBit   (bool b) noexcept -> BitProxy&;
  void                  advance  (difference_type n) noexcept;
  void                  setIndex (size_type i) noexcept;
  constexpr static auto getMask  (size_type index) noexcept -> value_type;
};

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitProxy<IsConst>::BitProxy(pointer val, size_type ind) noexcept
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
  return *value & mask;
}

/**
 * @brief Returns the value of the refenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitProxy<IsConst>::operator bool() const noexcept
{
  return *value & mask;
}

/**
 * @brief Assigns a bool to the referenced bit.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitProxy<IsConst>::setBit(bool b) noexcept -> BitProxy&
{
  const auto currentValue = *value;
  const value_type tmp = b ? (currentValue & cutter) | mask : (currentValue & cutter) & ~mask;
  *value = (currentValue & ~cutter) | tmp;
  return *this;
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



/** 
 * @class BitFieldIterator
 * @brief BitFieldIterator is a random access iterator that provides
 *        access to individual bits via the \link #BitProxy \endlink class.
 * @see   https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
class BitField<Size, StartBit>::BitFieldIterator {

  static constexpr bool isConst = IsConst;
  static constexpr auto size    = Size;
  using underlyingValueType     = typename BitField<Size, StartBit>::value_type;
  using underlyingPointer       = std::conditional_t<isConst, underlyingValueType const*, underlyingValueType*>;

public:

  // ---------------------------------------------------
  // iterator properties
  using iterator_category = std::random_access_iterator_tag;
  using value_type        = BitField<Size, StartBit>::BitProxy<IsConst>;
  using size_type         = BitField<Size, StartBit>::size_type;
  using difference_type   = std::ptrdiff_t;
  using pointer           = std::conditional_t<IsConst, value_type const*, value_type*>;
  using reference         = std::conditional_t<IsConst, value_type const&, value_type&>;

  // ---------------------------------------------------
  // constructor
  constexpr BitFieldIterator(underlyingPointer val, size_type i) noexcept;

  // ---------------------------------------------------
  // dereferencing
  template <typename = std::enable_if_t<isConst>>
  constexpr auto operator*() const noexcept -> reference { return proxy;}
  template <typename = std::enable_if_t<!isConst>>
  auto operator*() noexcept -> reference { return proxy;}

  template <typename = std::enable_if_t<isConst>>
  constexpr auto operator->() const noexcept -> pointer { return &proxy;}
  template <typename = std::enable_if_t<!isConst>>
  auto operator->() noexcept -> pointer { return &proxy;}

  // ---------------------------------------------------
  // incrementing and decrementing properties
  auto operator++ () noexcept -> BitFieldIterator&;
  auto operator++ (int) noexcept -> BitFieldIterator;
  auto operator-- () noexcept -> BitFieldIterator&;
  auto operator-- (int) noexcept -> BitFieldIterator;
  auto operator+= (difference_type n) noexcept -> BitFieldIterator&;
  auto operator-= (difference_type n) noexcept -> BitFieldIterator&;
  auto operator[] (size_type) noexcept -> value_type;

  //auto operator-> () const -> pointer;

  // ---------------------------------------------------
  // comparison
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
    return it1.index == it2.index;
  }
  friend auto operator!= (BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> bool
  {
    return !(it1 == it2);
  }

  // 
  friend auto operator+(BitFieldIterator const& it, difference_type n) noexcept -> BitFieldIterator
  {
    BitFieldIterator temp = it;
    return temp += n;
  }

  friend auto operator+(difference_type n, BitFieldIterator const& it) noexcept -> BitFieldIterator
  {
    BitFieldIterator temp = it;
    return temp += n;
  }

  friend constexpr auto operator-(BitFieldIterator const& it1, BitFieldIterator const& it2) noexcept -> difference_type
  {
    return it1.index >= it2.index ? static_cast<difference_type>(it1.index - it2.index) : -1LL * static_cast<difference_type>(it2.index - it1.index);
  }

private:

  // ---------------------------------------------------
  // private data
  size_type  index;
  value_type proxy;

  // ---------------------------------------------------
  // private methods
  [[nodiscard]] constexpr auto isAtEnd() const noexcept -> bool;
  void advance(difference_type n) noexcept;
};


/**
 * @brief Constructor. Constructs the proxy element and initializes members.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
constexpr BitField<Size, StartBit>::BitFieldIterator<IsConst>::BitFieldIterator(underlyingPointer val, size_type i) noexcept
  : index {std::min(i, size)}
  , proxy {val, index}
{}

/**
 * @brief increments the iterator by n elements.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator+=(difference_type n) noexcept -> BitFieldIterator&
{
  advance(n);
  return *this;
}

/**
 * @brief decrements the iterator by n elements.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator-=(difference_type n) noexcept -> BitFieldIterator&
{
  return *this += -n;
}

/**
 * @brief  increments the iterator by one element (postfix version).
 * @return a reference to the incremented iterator.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator++() noexcept -> BitFieldIterator&
{
  return *this += 1LL;
}

/**
 * @brief  increments the iterator by one element (präfix version).
 * @return a copy of the iterator before it was incremented.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator++(int) noexcept -> BitFieldIterator
{
  BitFieldIterator tmp{*this};
  ++(*this);
  return tmp;
}

/**
 * @brief  decrements the iterator by one element (postfix version).
 * @return a reference to the decremented iterator.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator--() noexcept -> BitFieldIterator&
{
  return *this -= 1LL;
}

/**
 * @brief  decrements the iterator by one element (präfix version).
 * @return a copy of the iterator before it was decremented.
 */
template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator--(int) noexcept -> BitFieldIterator
{
  BitFieldIterator tmp{*this};
  --(*this);
  return tmp;
}


template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::operator[](size_type i) noexcept -> value_type
{
  return *(*this + i);
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
[[nodiscard]] constexpr auto BitField<Size, StartBit>::BitFieldIterator<IsConst>::isAtEnd() const noexcept -> bool
{
  return index >= size;
}

template <std::size_t Size, std::size_t StartBit>
template <bool IsConst>
inline void BitField<Size, StartBit>::BitFieldIterator<IsConst>::advance(difference_type n) noexcept
{
  if(n > 0LL && !isAtEnd())
  {
    index += n;
    if(!isAtEnd())
    {
      proxy.setIndex(index);
    }
    else
    {
      index = size;
    }
  }
  else if (n < 0LL)
  {
    if(std::abs(n) <= static_cast<difference_type>(index))
    {
      index += n;
    }
    else
    {
      index = 0ULL;
    }
    proxy.setIndex(index);
  }
}


/**
 * @brief Constructor. Initializes the bits with the binary representation of the given value.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr BitField<Size, StartBit>::BitField(value_type v) noexcept
  : value {getValue(v)}
{}

/**
 * @brief Copy Constructor.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr BitField<Size, StartBit>::BitField(BitField<Size, StartBit> const& src) noexcept
  : value {getValue(src.value)}
{}

/**
 * @brief Assignment operator.
 */
template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::operator=(value_type v) noexcept -> BitField&
{
  value = (value & ~cutter) | ((v & mask) << StartBit);
  return *this;
}

/**
 * @brief Conversion operator.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr BitField<Size, StartBit>::operator value_type() const noexcept
{ 
  return (value >> StartBit) & mask;
}

/**
 * @brief Returns the size of the bit field.
 */
template <std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto BitField<Size, StartBit>::size() const noexcept -> size_type
{
  return Size;
}

/**
 * @brief Bitwise AND.
 */
template <std::size_t Size, std::size_t StartBit>
auto BitField<Size, StartBit>::operator&=(BitField const& src) noexcept -> BitField&
{
  value = value & ~cutter | ((value & cutter) & (src.value & cutter));
  return *this;
}

/**
 * @brief Bitwise OR.
 */
template <std::size_t Size, std::size_t StartBit>
auto BitField<Size, StartBit>::operator|=(BitField const& src) noexcept -> BitField&
{
  value = value & ~cutter | ((value & cutter) | (src.value & cutter));
  return *this;
}

/**
 * @brief Bitwise XOR.
 */
template <std::size_t Size, std::size_t StartBit>
auto BitField<Size, StartBit>::operator^=(BitField const& src) noexcept -> BitField&
{
  value = value & ~cutter | ((value & cutter) ^ (src.value & cutter));
  return *this;
}


/**
 * @brief  Accesses the bit at a given position.
 * @return returns true if the bit is set, false otherwise.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::operator[](std::size_t index) const noexcept -> const_reference
{
  return const_reference(&value, index);
}

template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::operator[](std::size_t index) noexcept -> reference
{
  return reference(&value, index);
}

/**
 * @brief Begin method. returns a non-const begin iterator.
 */
template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::begin() noexcept -> iterator
{
  return iterator(&value, 0ULL);
}

/**
 * @brief Returns a non-const end iterator.
 */
template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::end() noexcept -> iterator
{
  return iterator(&value, size());
}

/**
 * @brief Returns a const begin iterator.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::begin() const noexcept -> const_iterator
{
  return const_iterator(&value, 0ULL);
}

/**
 * @brief Returns a const end iterator.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::end() const noexcept -> const_iterator
{
  return const_iterator(&value, size());
}

/**
 * @brief Returns a const begin iterator.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::cbegin() const noexcept -> const_iterator
{
  return const_iterator(&value, 0ULL);
}

/**
 * @brief Returns a const end iterator.
 */
template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::cend() const noexcept -> const_iterator
{
  return const_iterator(&value, size());
}


template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::getValue(value_type v) noexcept -> value_type
{
  return (v & ~cutter) | ((v & mask) << StartBit);
}

template <std::size_t Size, std::size_t StartBit>
constexpr auto BitField<Size, StartBit>::getBitProxy(std::size_t index) const noexcept -> BitProxy<true>
{
  BitProxy<true> p(&value, index);
  return p;
}

template <std::size_t Size, std::size_t StartBit>
inline auto BitField<Size, StartBit>::getBitProxy(std::size_t index) noexcept -> BitProxy<false>
{
  BitProxy<false> p(&value, index);
  return p;
}

/**
 * @brief Bitwise comparison.
 */
template <std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto operator==(BitField<Size, StartBit> const& b1, BitField<Size, StartBit> const& b2) noexcept -> bool
{
  return static_cast<typename BitField<Size, StartBit>::value_type>(b1) ==
         static_cast<typename BitField<Size, StartBit>::value_type>(b2);
}

/**
 * @brief Bitwise NOT.
 */
template <std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto operator~(BitField<Size, StartBit> const& b1) noexcept -> BitField<Size, StartBit>
{
  return BitField<Size, StartBit>(~static_cast<typename BitField<Size, StartBit>::value_type>(b1));
}

/**
 * @brief Bitwise AND.
 */
template <std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto operator&(BitField<Size, StartBit> const& b1, BitField<Size, StartBit> const& b2) noexcept
{
  return BitField<Size, StartBit>(static_cast<typename BitField<Size, StartBit>::value_type>(b1) & static_cast<typename BitField<Size, StartBit>::value_type>(b2));
}

/**
 * @brief Bitwise OR.
 */
template <std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto operator|(BitField<Size, StartBit> const& b1, BitField<Size, StartBit> const& b2) noexcept
{
  return BitField<Size, StartBit>(static_cast<typename BitField<Size, StartBit>::value_type>(b1) | static_cast<typename BitField<Size, StartBit>::value_type>(b2));
}

/**
 * @brief Bitwise XOR.
 */
template <std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto operator^(BitField<Size, StartBit> const& b1, BitField<Size, StartBit> const& b2) noexcept
{
  return BitField<Size, StartBit>(static_cast<typename BitField<Size, StartBit>::value_type>(b1) ^ static_cast<typename BitField<Size, StartBit>::value_type>(b2));
}

/**
 * @brief Prints the binary representation of the BitField into a given stream.
 */
template <std::size_t Size, std::size_t StartBit>
template <typename CharT, typename CharTraits>
inline auto BitField<Size, StartBit>::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
//  cpptools::binaryRep(static_cast<value_type>(value & cutter), ostr);
  std::vector<CharT> v;
  v.reserve(Size);
  for(auto b : *this)
  {
    v.push_back(b == true ? ostr.widen('1') : ostr.widen('0'));
  }

  for(auto val = v.crbegin(); val != v.crend(); ++val)
  {
    ostr << *val;
  }
  return ostr;
}

/**
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
