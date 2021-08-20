/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitVector.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    14.06.2021 
 */ 
 
#ifndef BITVECTOR_H_315772235374352685828765467722392194654870 
#define BITVECTOR_H_315772235374352685828765467722392194654870 
 
 
// includes
#include <Bitwise/BitProxy.h>
#include <Utils/miscellaneous.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <new>
#include <sstream>
#include <stdexcept>


namespace bws {


/**
 * @class BitVector
 * @brief
 */
template <typename IntType = std::uint8_t>
class BitVectorT {

  // ---------------------------------------------------
  // private types
  using byte    = IntType;
  using dataPtr = std::unique_ptr<byte[]>;

  // ---------------------------------------------------
  //  private constants
  static constexpr std::size_t byteSize   = sizeof(byte);
  static constexpr std::size_t regionSize = 8ULL * byteSize;

  template <bool IsConst>
  class BitIterator;

public:

  // ---------------------------------------------------
  // public types
  using size_type       = std::size_t;
  using value_type      = bool;
  using reference       = BitProxy<byte, false>;
  using const_reference = BitProxy<byte, true>;
  using iterator        = BitIterator<false>;
  using const_iterator  = BitIterator<true>;
  
  // ---------------------------------------------------
  // ctor & dtor
  BitVectorT () = default;
  BitVectorT (size_type s);
  BitVectorT (std::initializer_list<int> l);

  // ---------------------------------------------------
  // public api
  [[nodiscard]] auto size     () const noexcept -> size_type;
  [[nodiscard]] auto empty    () const noexcept -> bool;
  [[nodiscard]] auto capacity () const noexcept -> size_type;
  void reserve                (size_type c);
  auto operator[]             (size_type index) const -> const_reference;
  auto operator[]             (size_type index) -> reference;
  auto at                     (size_type index) const -> const_reference;
  auto at                     (size_type index) -> reference;
  void push_back              (value_type b);
  void pop_back               () noexcept;
  auto front                  () const -> const_reference;
  auto front                  () -> reference;
  auto back                   () const -> const_reference;
  auto back                   () -> reference;
  auto begin                  () noexcept -> iterator;
  auto end                    () noexcept -> iterator;
  auto begin                  () const noexcept -> const_iterator;
  auto end                    () const noexcept -> const_iterator;
  auto cbegin                 () const noexcept -> const_iterator;
  auto cend                   () const noexcept -> const_iterator;

private:

  // ---------------------------------------------------
  // private data
  size_type currentSize     {};
  size_type currentCapacity {};
  dataPtr   data;

  // ---------------------------------------------------
  // private methods
  auto numberOfRegions    () const noexcept -> size_type;
  auto allocateMemory     () const -> dataPtr;
  void performBoundsCheck (size_type);
};


/**
 * @class BitIterator
 * @brief
 */
template <typename IntType>
template <bool IsConst>
class BitVectorT<IntType>::BitIterator {

  // ---------------------------------------------------
  // private constants and types
  static constexpr bool        isConst    = IsConst;
  static constexpr std::size_t byteSize   = sizeof(IntType);
  static constexpr std::size_t regionSize = 8ULL * byteSize;

  using rawPtr = IntType*;

  // convenience alias for SFINAE
  template <bool B>
  using RequiresNonConst = std::enable_if_t<!B>;

public:

  // ---------------------------------------------------
  // iterator properties
  using value_type        = BitProxy<IntType, isConst>;
  using size_type         = typename value_type::size_type;
  using difference_type   = std::ptrdiff_t;
  using pointer           = std::conditional_t<isConst, value_type const*, value_type*>;
  using reference         = std::conditional_t<isConst, value_type const&, value_type&>;
  using const_reference   = value_type const&;

  // ---------------------------------------------------
  // construction
  BitIterator           () = delete;
  constexpr BitIterator (rawPtr data, size_type index, size_type bitFieldSize);
  BitIterator           (BitIterator const&) noexcept = default;
  auto operator=        (BitIterator const&) noexcept -> BitIterator& = default;

  // ---------------------------------------------------
  // incrementing and decrementing methods
  auto operator+= (difference_type n) noexcept -> BitIterator&;
  auto operator-= (difference_type n) noexcept -> BitIterator&;
  auto operator++ () noexcept -> BitIterator&;
  auto operator++ (int) noexcept -> BitIterator;
  auto operator-- () noexcept -> BitIterator&;
  auto operator-- (int) noexcept -> BitIterator;

  // ---------------------------------------------------
  // api
  template <typename = RequiresNonConst<isConst>>
  constexpr auto operator* () const noexcept -> const_reference { return bitProxy; }
  auto operator*           () -> reference { return bitProxy; }

  // ---------------------------------------------------
  // comparison
  friend auto operator< (BitIterator const& it1, BitIterator const& it2) noexcept -> bool
  {
    return it1.bitIndex() < it2.bitIndex();
  }
  friend auto operator> (BitIterator const& it1, BitIterator const& it2) noexcept -> bool
  {
    return it2 < it1;
  }
  friend auto operator<= (BitIterator const& it1, BitIterator const& it2) noexcept -> bool
  {
    return !(it2 < it1);
  }
  friend auto operator>= (BitIterator const& it1, BitIterator const& it2) noexcept -> bool
  {
    return !(it1 < it2);
  }
  friend auto operator== (BitIterator const& it1, BitIterator const& it2) noexcept -> bool
  {
    return it1.bitIndex() == it2.bitIndex();
  }
  friend auto operator!= (BitIterator const& it1, BitIterator const& it2) noexcept -> bool
  {
    return !(it1 == it2);
  }

//private:

  // ---------------------------------------------------
  // private data
  rawPtr     data;
  size_type  regionIndex;
  size_type  indexInRegion;
  size_type  bitFieldSize;
  value_type bitProxy;

  // ---------------------------------------------------
  // private methods
  void advance                             (difference_type n) noexcept;
  void advanceNoBoundsChecks               (difference_type n) noexcept;
  [[nodiscard]] constexpr auto createProxy () const noexcept -> value_type;
  [[nodiscard]] constexpr auto bitIndex    () const noexcept -> size_type;
  [[nodiscard]] constexpr auto isAtEnd     () const noexcept -> bool;
  void moveToEnd                           () noexcept;
  void moveToBegin                         () noexcept;
};


template <typename IntType>
template <bool IsConst>
constexpr BitVectorT<IntType>::BitIterator<IsConst>::BitIterator(rawPtr data, size_type index, size_type bitFieldSize)
  : data          {data}
  , regionIndex   {index / regionSize}
  , indexInRegion {index % regionSize}
  , bitFieldSize  {bitFieldSize}
  , bitProxy      {createProxy()}
{}

/**
 * @brief increments the iterator by n elements.
 */
template <typename IntType>
template <bool IsConst>
auto BitVectorT<IntType>::BitIterator<IsConst>::operator+=(difference_type n) noexcept -> BitIterator&
{
  advance(n);
  return *this;
}

/**
 * @brief decrements the iterator by n elements.
 */
template <typename IntType>
template <bool IsConst>
auto BitVectorT<IntType>::BitIterator<IsConst>::operator-=(difference_type n) noexcept -> BitIterator&
{
  advance(-n);
  return *this;
}

/**
 * @brief  increments the iterator by one element (postfix version).
 * @return a reference to the incremented iterator.
 */
template <typename IntType>
template <bool IsConst>
auto BitVectorT<IntType>::BitIterator<IsConst>::operator++() noexcept -> BitIterator&
{
  return *this += 1LL;
}

/**
 * @brief  decrements the iterator by one element (präfix version).
 * @return a reference to the decremented iterator.
 */
template <typename IntType>
template <bool IsConst>
auto BitVectorT<IntType>::BitIterator<IsConst>::operator++(int) noexcept -> BitIterator
{
  BitIterator tmp{*this};
  ++(*this);
  return tmp;
}

/**
 * @brief  decrements the iterator by one element (postfix version).
 * @return a reference to the decremented iterator.
 */
template <typename IntType>
template <bool IsConst>
auto BitVectorT<IntType>::BitIterator<IsConst>::operator--() noexcept -> BitIterator&
{
  return *this -= 1LL;
}

/**
 * @brief  decrements the iterator by one element (präfix version).
 * @return a copy of the iterator before it was decremented.
 */
template <typename IntType>
template <bool IsConst>
auto BitVectorT<IntType>::BitIterator<IsConst>::operator--(int) noexcept -> BitIterator
{
  BitIterator tmp{*this};
  --(*this);
  return tmp;
}

/**
 * @brief Advances the iterator by n bits.
 */
template <typename IntType>
template <bool IsConst>
void BitVectorT<IntType>::BitIterator<IsConst>::advance(difference_type n) noexcept
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
template <typename IntType>
template <bool IsConst>
void BitVectorT<IntType>::BitIterator<IsConst>::advanceNoBoundsChecks(difference_type n) noexcept
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

/**
 * @brief creates a /link #BitProxy /endlink object for the current position of the iterator.
 */
template <typename IntType>
template <bool IsConst>
constexpr [[nodiscard]] auto BitVectorT<IntType>::BitIterator<IsConst>::createProxy() const noexcept -> value_type
{
  return value_type{data + regionIndex, indexInRegion};
}

/**
 * @brief calculates the index of the bit the iterator is pointing to.
 */
template <typename IntType>
template <bool IsConst>
constexpr [[nodiscard]] auto BitVectorT<IntType>::BitIterator<IsConst>::bitIndex() const noexcept -> size_type
{
  return regionSize * regionIndex + indexInRegion;
}

/**
 * @brief Checks if the iterator is pointing behind the last bit.
 */
template <typename IntType>
template <bool IsConst>
constexpr [[nodiscard]] auto BitVectorT<IntType>::BitIterator<IsConst>::isAtEnd() const noexcept -> bool
{
  return bitIndex() >= bitFieldSize;
}

/**
 * @brief Sets the iterator to one past the last bit.
 */
template <typename IntType>
template <bool IsConst>
inline void BitVectorT<IntType>::BitIterator<IsConst>::moveToEnd() noexcept
{
  regionIndex   = bitFieldSize / regionSize;
  indexInRegion = bitFieldSize % regionSize;
}

/**
 * @brief Sets the iterator to the first bit.
 */
template <typename IntType>
template <bool IsConst>
inline void BitVectorT<IntType>::BitIterator<IsConst>::moveToBegin() noexcept
{
  regionIndex   = 0ULL;
  indexInRegion = 0ULL;
  bitProxy      = createProxy();
}



/**
 * @brief Constructor. Sets the initial size of the vector, calculates its capacity
 *        And allocates memory.
 */
template <typename IntType>
inline BitVectorT<IntType>::BitVectorT(size_type s)
  : currentSize     {s}
  , currentCapacity {cpptools::alignUp(currentSize, regionSize)}
  , data            {allocateMemory()}
{}

/**
 * @brief Constructor. Sets the initial size of the vector, calculates its capacity
 *        And allocates memory.
 */
template <typename IntType>
inline BitVectorT<IntType>::BitVectorT(std::initializer_list<int> l)
  : BitVectorT ()
{
  reserve(l.size());
  for(const auto b : l)
  {
    push_back(b);
  }
}

/**
 * @brief Returns the size of the bit vector.
 */
template <typename IntType>
inline [[nodiscard]] auto BitVectorT<IntType>::size() const noexcept -> size_type
{
  return currentSize;
}

/**
 * @brief Checks whether the vector is empty.
 */
template <typename IntType>
inline [[nodiscard]] auto BitVectorT<IntType>::empty() const noexcept -> bool
{
  return currentSize == 0ULL;
}

/**
 * @brief Returns the current capacity of the bit vector.
 */
template <typename IntType>
inline [[nodiscard]] auto BitVectorT<IntType>::capacity() const noexcept -> size_type
{
  return currentCapacity;
}

/**
 * @brief  Increase the capacity of the vector to a value that's greater or equal to newCapacity.
 *         If newCapacity is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
 * @remark If newCapacity is greater than capacity(), all iterators, including the past-the-end iterator,
 *         and all references to the elements are invalidated. Otherwise, no iterators or references are invalidated. 
 */
template <typename IntType>
void BitVectorT<IntType>::reserve(size_type newCapacity)
{
  if(newCapacity > currentCapacity)
  {
    newCapacity = cpptools::alignUp(newCapacity, regionSize);
    auto* newStorage = new(std::nothrow) IntType[newCapacity / regionSize];

    if(newStorage != nullptr)
    {
      if(data)
      {
        std::memcpy(newStorage, data.get(), currentCapacity / 8ULL);
      }
      data.reset(newStorage);
      currentCapacity = newCapacity;
    }
  }
}

/**
 * @brief  Accesses the bit at a given position.
 * @return returns a const /link #BitProxy /endlink object providing
 *         reading access to the underlying bit.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::operator[](size_type index) const -> const_reference
{
  return const_reference{data.get() + index / regionSize, index % regionSize};
}

/**
 * @brief  Accesses the bit at a given position.
 * @return returns a const /link #BitProxy /endlink object providing
 *         writing access to the underlying bit.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::operator[](size_type index) -> reference
{
  return reference{data.get() + index / regionSize, index % regionSize};
}

/**
 * @brief  Accesses the bit at a given position.
 * @remark If index is not within the range of the container, an exception of type std::out_of_range is thrown.
 * @return returns a const /link #BitProxy /endlink object providing
 *         reading access to the underlying bit.
 * @throw  std::out_of_range
 */
template <typename IntType>
inline auto BitVectorT<IntType>::at(size_type index) const -> const_reference
{
  performBoundsCheck(index);
  return (*this)[index];
}

/**
 * @brief  Accesses the bit at a given position.
 * @remark If index is not within the range of the container, an exception of type std::out_of_range is thrown.
 * @return returns a const /link #BitProxy /endlink object providing
 *         reading access to the underlying bit.
 * @throw  std::out_of_range
 */
template <typename IntType>
inline auto BitVectorT<IntType>::at(size_type index) -> reference
{
  performBoundsCheck(index);
  return (*this)[index];
}

/**
 * @brief Throws an exception of type std::out_of_range, if index is not within the range of the container.
 * @throw std::out_of_range.
 */
template <typename IntType>
void BitVectorT<IntType>::performBoundsCheck(size_type index)
{
  if(index >= currentSize)
  {
    std::stringstream errMsg;
    errMsg << "Error : index [which is " << index << "] >= size [which is " << currentSize << "]";
    throw std::out_of_range(errMsg.str());
  }
}

/**
 * @brief Appends a new value to the given container.
 */
template <typename IntType>
void BitVectorT<IntType>::push_back(value_type b)
{
  const auto newSize = currentSize + 1ULL;
  if(newSize > currentCapacity)
  {
    const auto cap = currentCapacity;
    reserve(newSize);

    if(currentCapacity > cap)
    {
      (*this)[currentSize] = b;
      ++currentSize;
    }
  }
  else
  {
    (*this)[currentSize] = b;
    ++currentSize;
  }
}

/**
 * @brief Removes the last element of the container.
 */
template <typename IntType>
inline void BitVectorT<IntType>::pop_back() noexcept
{
  if(currentSize > 0ULL)
  {
    --currentSize;
  }
}

/**
 * @brief Returns a const reference to the first bit.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::front() const -> const_reference
{
  return (*this)[0ULL];
}

/**
 * @brief Returns a reference to the first bit.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::front() -> reference
{
  return (*this)[0ULL];
}

/**
 * @brief Returns a const reference to the last bit.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::back() const -> const_reference
{
  return (*this)[currentSize - 1ULL];
}

/**
 * @brief Returns a reference to the last bit.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::back() -> reference
{
  return (*this)[currentSize - 1ULL];
}

/**
 * @brief Begin method. returns a non-const begin iterator.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::begin() noexcept -> iterator
{
  return iterator(data.get(), 0ULL, currentSize);
}

/**
 * @brief Returns a non-const end iterator.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::end() noexcept -> iterator
{
  return iterator(data.get(), currentSize, currentSize);
}

/**
 * @brief Returns a const begin iterator.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <typename IntType>
inline auto BitVectorT<IntType>::begin() const noexcept -> const_iterator
{
  return const_iterator(data.get(), 0ULL, currentSize);
}

/**
 * @brief Returns a const begin iterator.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <typename IntType>
inline auto BitVectorT<IntType>::end() const noexcept -> const_iterator
{
  return const_iterator(data.get(), currentSize, currentSize);
}

/**
 * @brief Returns a const begin iterator.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::cbegin() const noexcept -> const_iterator
{
  return const_iterator(data.get(), 0ULL, currentSize);
}

/**
 * @brief Returns a const end iterator.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::cend() const noexcept -> const_iterator
{
  return const_iterator(data.get(), currentSize, currentSize);
}

/**
 * @brief Returns the number of atomic regions.
 */
template <typename IntType>
auto BitVectorT<IntType>::numberOfRegions() const noexcept -> size_type
{
  return currentCapacity / regionSize;
}

/**
 * @brief Allocates memory according the current capacity and return a dataPtr
 *        referring to the newly allocated memory chunk.
 */
template <typename IntType>
auto BitVectorT<IntType>::allocateMemory() const -> dataPtr
{
  return std::make_unique<byte[]>(numberOfRegions());
}


}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITVECTOR_H_315772235374352685828765467722392194654870 
