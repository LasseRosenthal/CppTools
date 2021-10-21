/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitProxy.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    14.06.2021 
 */ 
 
#ifndef BITPROXY_H_20321419910188205481840213735141891382916113 
#define BITPROXY_H_20321419910188205481840213735141891382916113 
 
 
// includes
#include <Bitwise/Bitwise.h>

#include <ostream>
#include <type_traits>


namespace bws {


/**
 * @class BitProxy
 * @brief
 */
template <typename T, bool IsConst, std::size_t Size = sizeof(T) * 8ULL,
          std::size_t StartBit = 0ULL>
class BitProxy {

public:

  // ---------------------------------------------------
  // public types
  using value_type      = T;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;

  // ---------------------------------------------------
  // private constants and types
  static constexpr bool isConst         = IsConst;
  static constexpr size_type size       = Size;
  static constexpr size_type startBit   = StartBit;
  static constexpr value_type punchMask = bws::punchMask<value_type>(size, startBit);

private:

  // ---------------------------------------------------
  // private constants and types
  using pointer   = std::conditional_t<isConst, value_type const*, value_type*>;
  using reference = std::conditional_t<isConst, value_type const&, value_type&>;

  template <bool B>
  using RequiresNonConst = std::enable_if_t<!B>;

public:

  // ---------------------------------------------------
  // special member functions
  BitProxy           () = delete;
  constexpr BitProxy (pointer data, size_type index) noexcept;
  constexpr BitProxy (BitProxy const&) noexcept = default;
  auto operator=     (BitProxy const&) noexcept -> BitProxy& = default;

  // ---------------------------------------------------
  // public api
  constexpr auto isSet    () const noexcept -> bool;
  constexpr operator bool () const noexcept;
  template <typename = RequiresNonConst<isConst>>
  void set                (bool const b) noexcept;
  template <typename = RequiresNonConst<isConst>>
  auto operator=          (bool const b) noexcept -> bool { set(b); return b; }
  template <typename = RequiresNonConst<isConst>>
  void flip               () noexcept;
  void advance            (difference_type n) noexcept;
  [[nodiscard]] constexpr auto value() const noexcept -> value_type;

  // ---------------------------------------------------
  // private data
  pointer    data;
  size_type  index;
  value_type bitMask;
};


/**
 * @brief Constructor.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
constexpr BitProxy<T, IsConst, Size, StartBit>::BitProxy(pointer d, size_type i) noexcept
  : data    {d}
  , index   {i}
  , bitMask {bws::punchMask<value_type>(1ULL, index)}
{}

/**
 * @brief Returns the value of the refenced bit.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
constexpr auto BitProxy<T, IsConst, Size, StartBit>::isSet() const noexcept -> bool
{
  return static_cast<bool>(*data & bitMask);
}

/**
 * @brief Returns the value of the refenced bit.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
constexpr BitProxy<T, IsConst, Size, StartBit>::operator bool() const noexcept
{
  return static_cast<bool>(*data & bitMask);
}

/**
 * @brief Assigns a bool to the referenced bit.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
template <typename>
inline void BitProxy<T, IsConst, Size, StartBit>::set(bool const b) noexcept
{
  if(b)
  {
    *data |= bitMask;
  }
  else
  {
    *data &= ~bitMask;
  }
}

/**
 * @brief Toggles the referenced bit.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
template <typename>
inline void BitProxy<T, IsConst, Size, StartBit>::flip() noexcept
{
  *data ^= bitMask;
}

/**
 * @brief advances the referenced bits by n bits.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
inline void BitProxy<T, IsConst, Size, StartBit>::advance(difference_type n) noexcept
{
  index   = (index + n) % size;
  bitMask = bws::punchMask<value_type>(1ULL, index);
}

/**
 * @brief returns a copy of the data value the proxy is watching.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit>
[[nodiscard]] constexpr auto BitProxy<T, IsConst, Size, StartBit>::value() const noexcept -> value_type
{
  return *data;
}

/**
 * @brief Stream operator for BitProxy objects. The underlying bits are represented as 1 or 0.
 */
template <typename T, bool IsConst, std::size_t Size, std::size_t StartBit,
          typename CharT, typename ChartTraits = std::char_traits<CharT>>
auto operator<<(std::basic_ostream<CharT, ChartTraits>& ostr, BitProxy<T, IsConst, Size, StartBit> const& bitProxy) -> std::basic_ostream<CharT, ChartTraits>&
{
  return ostr << (static_cast<bool>(bitProxy) ? ostr.widen('1') : ostr.widen('0'));
}


}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITPROXY_H_20321419910188205481840213735141891382916113 
