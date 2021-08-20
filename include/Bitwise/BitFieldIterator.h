/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitFieldIterator.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.08.2021 
 */ 
 
#ifndef BITFIELDITERATOR_H_12870123271117631723722715281273584928729 
#define BITFIELDITERATOR_H_12870123271117631723722715281273584928729 
 
 
// includes
#include <Bitwise/BitProxy.h>

#include <array>
#include <iterator>


namespace bws {


/**
 * @class BitFieldIterator
 * @brief
 */
template <typename ByteType, bool IsConst = false>
class BitFieldIterator {

public:

  // ---------------------------------------------------
  // public constants and types
  using byteType = ByteType;

  static constexpr bool        isConst    = IsConst;
  static constexpr std::size_t byteSize   = sizeof(byteType);
  static constexpr std::size_t regionSize = 8ULL * byteSize;

  // convenience alias for SFINAE
  template <bool B>
  using RequiresNonConst = std::enable_if_t<!B>;

  using dataPtr = std::conditional_t<isConst, byteType const*, byteType*>;

  // ---------------------------------------------------
  // iterator properties
  using iterator_category = std::random_access_iterator_tag;
  using value_type        = BitProxy<byteType, isConst>;
  using size_type         = typename value_type::size_type;
  using difference_type   = std::ptrdiff_t;
  using pointer           = std::conditional_t<isConst, value_type const*, value_type*>;
  using reference         = std::conditional_t<isConst, value_type const, value_type>;
  using const_reference   = value_type const;

  // ---------------------------------------------------
  // construction
  BitFieldIterator           () = delete;
  constexpr BitFieldIterator (dataPtr data, size_type index, size_type bitFieldSize);
  constexpr BitFieldIterator (BitFieldIterator const&) noexcept = default;
  auto operator=             (BitFieldIterator const&) noexcept -> BitFieldIterator& = default;

  // ---------------------------------------------------
  // access methods
  constexpr auto operator*  () const noexcept -> const_reference { return bitProxy; }
  template <typename = RequiresNonConst<isConst>>
  auto operator*            () -> reference { return bitProxy; }
  auto operator->           () const -> pointer { return &bitProxy; }
  constexpr auto operator[] (difference_type n) const noexcept -> const_reference { return *(*this + n); }
  template <typename = RequiresNonConst<isConst>>
  auto           operator[] (difference_type n) noexcept -> reference { return *(*this + n);}

  // ---------------------------------------------------
  // incrementing and decrementing methods
  auto operator+= (difference_type n) noexcept -> BitFieldIterator&;
  auto operator-= (difference_type n) noexcept -> BitFieldIterator&;
  auto operator++ () noexcept -> BitFieldIterator&;
  auto operator++ (int) noexcept -> BitFieldIterator;
  auto operator-- () noexcept -> BitFieldIterator&;
  auto operator-- (int) noexcept -> BitFieldIterator;

  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator+ (BitFieldIterator<T, B> const& it,
                                                 typename BitFieldIterator<T, B>::difference_type n) noexcept -> BitFieldIterator<T, B>;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator+ (typename BitFieldIterator<T, B>::difference_type n,
                                                 BitFieldIterator<T, B> const& it) noexcept -> BitFieldIterator<T, B>;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator- (BitFieldIterator<T, B> const& it,
                                                 typename BitFieldIterator<T, B>::difference_type n) noexcept -> BitFieldIterator<T, B>;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator- (typename BitFieldIterator<T, B>::difference_type n,
                                                 BitFieldIterator<T, B> const& it) noexcept -> BitFieldIterator<T, B>;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator- (BitFieldIterator<T, B> const& it1,
                                                 BitFieldIterator<T, B> const& it2) noexcept -> typename BitFieldIterator<T, B>::difference_type;

  // ---------------------------------------------------
  // comparison
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator<  (BitFieldIterator<T, B> const& it1,
                                                  BitFieldIterator<T, B> const& it2) noexcept -> bool;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator>  (BitFieldIterator<T, B> const& it1,
                                                  BitFieldIterator<T, B> const& it2) noexcept -> bool;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator<= (BitFieldIterator<T, B> const& it1,
                                                  BitFieldIterator<T, B> const& it2) noexcept -> bool;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator>= (BitFieldIterator<T, B> const& it1,
                                                  BitFieldIterator<T, B> const& it2) noexcept -> bool;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator== (BitFieldIterator<T, B> const& it1,
                                                  BitFieldIterator<T, B> const& it2) noexcept -> bool;
  template <typename T, bool B>
  friend [[nodiscard]] constexpr auto operator!= (BitFieldIterator<T, B> const& it1,
                                                  BitFieldIterator<T, B> const& it2) noexcept -> bool;

private:

  // ---------------------------------------------------
  // private data
  dataPtr    data;
  size_type  regionIndex;
  size_type  indexInRegion;
  size_type  bitFieldSize;
  value_type bitProxy;

  // ---------------------------------------------------
  // private methods
  constexpr auto createProxy () const noexcept -> value_type;
  void advance               (difference_type n) noexcept;
  void advanceNoBoundsChecks (difference_type n) noexcept;
  constexpr auto bitIndex    () const noexcept -> size_type;
  constexpr auto isAtEnd     () const noexcept -> bool;
  void moveToEnd             () noexcept;
  void moveToBegin           () noexcept;
};


/**
 * @brief template alias for reverse bitfield iterators.
 */
template <typename ByteType, bool IsConst>
using ReverseBitFieldIterator = std::reverse_iterator<BitFieldIterator<ByteType, IsConst>>;


template <typename ByteType, bool IsConst>
constexpr BitFieldIterator<ByteType, IsConst>::BitFieldIterator(dataPtr data, size_type index, size_type bitFieldSize)
  : data          {data}
  , regionIndex   {index / regionSize}
  , indexInRegion {index % regionSize}
  , bitFieldSize  {bitFieldSize}
  , bitProxy      {createProxy()}
{}

/**
 * @brief creates a /link #BitProxy /endlink object for the current position of the iterator.
 */
template <typename ByteType, bool IsConst>
constexpr auto BitFieldIterator<ByteType, IsConst>::createProxy () const noexcept -> value_type
{
  return value_type{data + regionIndex, indexInRegion};
}

/**
 * @brief increments the iterator by n elements.
 */
template <typename ByteType, bool IsConst>
auto BitFieldIterator<ByteType, IsConst>::operator+=(difference_type n) noexcept -> BitFieldIterator&
{
  advance(n);
  return *this;
}

/**
 * @brief decrements the iterator by n elements.
 */
template <typename ByteType, bool IsConst>
auto BitFieldIterator<ByteType, IsConst>::operator-=(difference_type n) noexcept -> BitFieldIterator&
{
  advance(-n);
  return *this;
}

/**
 * @brief  increments the iterator by one element (postfix version).
 * @return a reference to the incremented iterator.
 */
template <typename ByteType, bool IsConst>
auto BitFieldIterator<ByteType, IsConst>::operator++() noexcept -> BitFieldIterator&
{
  return *this += 1LL;
}

/**
 * @brief  decrements the iterator by one element (prefix version).
 * @return a reference to the decremented iterator.
 */
template <typename ByteType, bool IsConst>
auto BitFieldIterator<ByteType, IsConst>::operator++(int) noexcept -> BitFieldIterator
{
  BitFieldIterator tmp{*this};
  ++(*this);
  return tmp;
}

/**
 * @brief  decrements the iterator by one element (postfix version).
 * @return a reference to the decremented iterator.
 */
template <typename ByteType, bool IsConst>
auto BitFieldIterator<ByteType, IsConst>::operator--() noexcept -> BitFieldIterator&
{
  return *this -= 1LL;
}

/**
 * @brief  decrements the iterator by one element (prefix version).
 * @return a copy of the iterator before it was decremented.
 */
template <typename ByteType, bool IsConst>
auto BitFieldIterator<ByteType, IsConst>::operator--(int) noexcept -> BitFieldIterator
{
  BitFieldIterator tmp{*this};
  --(*this);
  return tmp;
}

/**
 * @brief Advances the iterator by n bits.
 */
template <typename ByteType, bool IsConst>
void BitFieldIterator<ByteType, IsConst>::advance(difference_type n) noexcept
{
  if(n >= 0LL)
  {
    if(bitIndex() + n >= bitFieldSize)
    {
      moveToEnd();
      return;
    }
  }
  else if(static_cast<size_type>(-n) >= bitIndex())
  {
    moveToBegin();
    return;
  }

  advanceNoBoundsChecks(n);
}

/**
 * @brief  Advances the iterator by n bits.
 * @remark This method does not perform any bounds checking.
 */
template <typename ByteType, bool IsConst>
void BitFieldIterator<ByteType, IsConst>::advanceNoBoundsChecks(difference_type n) noexcept
{
  const size_type newBitIndex = bitIndex() + n;
  indexInRegion = newBitIndex % regionSize;
  if(const auto newRegionIndex = newBitIndex / regionSize; newRegionIndex != regionIndex)
  {
    regionIndex = newRegionIndex;
    bitProxy    = createProxy();
  }
  else
  {
    bitProxy.advance(n);
  }
}


template <typename T, bool B>
[[nodiscard]] constexpr auto operator+(BitFieldIterator<T, B> const& it,
                                       typename BitFieldIterator<T, B>::difference_type n) noexcept -> BitFieldIterator<T, B>
{
  return n >= 0LL ?
    BitFieldIterator<T, B>(it.data, it.bitIndex() + n > it.bitFieldSize ? it.bitFieldSize : it.bitIndex() + n, it.bitFieldSize) :
    BitFieldIterator<T, B>(it.data, static_cast<typename BitFieldIterator<T, B>::size_type>(-n) >= it.bitIndex() ? 0ULL : it.bitIndex() + n, it.bitFieldSize);
}

template <typename T, bool B>
[[nodiscard]] constexpr auto operator+(typename BitFieldIterator<T, B>::difference_type n,
                                       BitFieldIterator<T, B> const& it) noexcept -> BitFieldIterator<T, B>
{
  return it + n;
}

template <typename T, bool B>
[[nodiscard]] constexpr auto operator-(BitFieldIterator<T, B> const& it,
                                       typename BitFieldIterator<T, B>::difference_type n) noexcept -> BitFieldIterator<T, B>
{
  return it + (-n);
}

template <typename T, bool B>
[[nodiscard]] constexpr auto operator-(typename BitFieldIterator<T, B>::difference_type n,
                                       BitFieldIterator<T, B> const& it) noexcept -> BitFieldIterator<T, B>
{
  return it - n;
}

/**
 * @brief calculates the difference n between two iterators such that it1 + n == t2;
 */
template <typename T, bool B>
[[nodiscard]] constexpr auto operator-(BitFieldIterator<T, B> const& it1,
                                       BitFieldIterator<T, B> const& it2) noexcept -> typename BitFieldIterator<T, B>::difference_type
{
  return it1.bitIndex() - it2.bitIndex();
}

template <typename ByteType, bool IsConst>
[[nodiscard]] constexpr auto operator<(BitFieldIterator<ByteType, IsConst> const& it1,
                                       BitFieldIterator<ByteType, IsConst> const& it2) noexcept -> bool
{
  return it1.bitIndex() < it2.bitIndex();
}

template <typename ByteType, bool IsConst>
[[nodiscard]] constexpr auto operator>(BitFieldIterator<ByteType, IsConst> const& it1,
                                       BitFieldIterator<ByteType, IsConst> const& it2) noexcept -> bool
{
  return it2 < it1;
}

template <typename ByteType, bool IsConst>
[[nodiscard]] constexpr auto operator<=(BitFieldIterator<ByteType, IsConst> const& it1,
                                        BitFieldIterator<ByteType, IsConst> const& it2) noexcept -> bool
{
  return !(it2 < it1);
}

template <typename ByteType, bool IsConst>
[[nodiscard]] constexpr auto operator>=(BitFieldIterator<ByteType, IsConst> const& it1,
                                        BitFieldIterator<ByteType, IsConst> const& it2) noexcept -> bool
{
  return !(it1 < it2);
}

template <typename ByteType, bool IsConst>
[[nodiscard]] constexpr auto operator==(BitFieldIterator<ByteType, IsConst> const& it1,
                                        BitFieldIterator<ByteType, IsConst> const& it2) noexcept -> bool
{
  return it1.bitIndex() == it2.bitIndex();
}

template <typename ByteType, bool IsConst>
[[nodiscard]] constexpr auto operator!=(BitFieldIterator<ByteType, IsConst> const& it1,
                                        BitFieldIterator<ByteType, IsConst> const& it2) noexcept -> bool
{
  return !(it1 == it2);
}

/**
 * @brief calculates the index of the bit the iterator is pointing to.
 */
template <typename ByteType, bool IsConst>
constexpr auto BitFieldIterator<ByteType, IsConst>::bitIndex() const noexcept -> size_type
{
  return regionSize * regionIndex + indexInRegion;
}

/**
 * @brief Checks if the iterator is pointing behind the last bit.
 */
template <typename ByteType, bool IsConst>
constexpr auto BitFieldIterator<ByteType, IsConst>::isAtEnd() const noexcept -> bool
{
  return bitIndex() >= bitFieldSize;
}

/**
 * @brief Sets the iterator to one past the last bit.
 */
template <typename ByteType, bool IsConst>
inline void BitFieldIterator<ByteType, IsConst>::moveToEnd() noexcept
{
  regionIndex   = bitFieldSize / regionSize;
  indexInRegion = bitFieldSize % regionSize;
}

/**
 * @brief Sets the iterator to the first bit.
 */
template <typename ByteType, bool IsConst>
inline void BitFieldIterator<ByteType, IsConst>::moveToBegin() noexcept
{
  regionIndex   = 0ULL;
  indexInRegion = 0ULL;
  bitProxy      = createProxy();
}



/**
 * @brief returns a /link #BitFieldIterator /endlink pointing to the first bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto begin(std::array<ByteType, N>& arr) noexcept -> BitFieldIterator<ByteType, false>
{
  return BitFieldIterator<ByteType, false>{&(arr.front()), 0ULL, sizeof(ByteType) * N * 8ULL};
}

/**
 * @brief returns a /link #BitFieldIterator /endlink to one past the last bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto end(std::array<ByteType, N>& arr) noexcept -> BitFieldIterator<ByteType, false>
{
  return BitFieldIterator<ByteType, false>{&(arr.front()), sizeof(ByteType) * N * 8ULL, sizeof(ByteType) * N * 8ULL};
}

/**
 * @brief returns a /link #ReverseBitFieldIterator /endlink pointing to the last bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto rbegin(std::array<ByteType, N>& arr) noexcept -> ReverseBitFieldIterator<ByteType, false>
{
  return std::make_reverse_iterator(bws::end(arr));
}

/**
 * @brief returns a /link #ReverseBitFieldIterator /endlink pointing to the first bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto rend(std::array<ByteType, N>& arr) noexcept -> ReverseBitFieldIterator<ByteType, false>
{
  return std::make_reverse_iterator(bws::begin(arr));
}

/**
 * @brief returns a const /link #BitFieldIterator /endlink to the first bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto cbegin(std::array<ByteType, N> const& arr) noexcept -> BitFieldIterator<ByteType, true>
{
  return BitFieldIterator<ByteType, true>{&(arr.front()), 0ULL, sizeof(ByteType) * N * 8ULL};
}

/**
 * @brief returns const a /link #BitFieldIterator /endlink to one past the last bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto cend(std::array<ByteType, N> const& arr) noexcept -> BitFieldIterator<ByteType, true>
{
  return BitFieldIterator<ByteType, true>{&(arr.front()), sizeof(ByteType) * N * 8ULL, sizeof(ByteType) * N * 8ULL};
}

/**
 * @brief returns a const /link #ReverseBitFieldIterator /endlink to the last bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto crbegin(std::array<ByteType, N> const& arr) noexcept -> ReverseBitFieldIterator<ByteType, true>
{
  return std::make_reverse_iterator(bws::cend(arr));
}

/**
 * @brief returns a const /link #ReverseBitFieldIterator /endlink to the first bit of a std::array.
 */
template <typename ByteType, std::size_t N>
constexpr [[nodiscard]] auto crend(std::array<ByteType, N> const& arr) noexcept -> ReverseBitFieldIterator<ByteType, true>
{
  return std::make_reverse_iterator(bws::cbegin(arr));
}


}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITFIELDITERATOR_H_12870123271117631723722715281273584928729 
