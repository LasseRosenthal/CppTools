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
#include <Bitwise/BitFieldIterator.h>
#include <Utils/miscellaneous.h>

#include <algorithm>
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

public:

  // ---------------------------------------------------
  // public types
  using size_type       = std::size_t;
  using value_type      = bool;
  using reference       = BitProxy<byte, false>;
  using const_reference = BitProxy<byte, true>;
  using iterator        = BitFieldIterator<byte, false>;
  using const_iterator  = BitFieldIterator<byte, true>;
  
  // ---------------------------------------------------
  // ctor & dtor
  BitVectorT     () = default;
  BitVectorT     (size_type s, value_type val = value_type{});
  BitVectorT     (std::initializer_list<int> l);
  template <typename T, std::size_t N>
  BitVectorT     (T const (&src)[N]);
  BitVectorT     (BitVectorT const& src);
  BitVectorT     (BitVectorT&& src) noexcept;
  auto operator= (BitVectorT src) noexcept -> BitVectorT&;

  // ---------------------------------------------------
  // public api
  [[nodiscard]] auto size     () const noexcept -> size_type;
  [[nodiscard]] auto empty    () const noexcept -> bool;
  [[nodiscard]] auto capacity () const noexcept -> size_type;
  void swap                   (BitVectorT&) noexcept;
  void reserve                (size_type c);
  void shrink_to_fit          ();
  void resize                 (size_type size, value_type val = value_type{});
  void assign                 (size_type size, value_type val);
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
  auto allocateMemory     (size_type numRegions) const -> dataPtr;
  void performBoundsCheck (size_type);
  auto minCapacity        (size_type size) const -> size_type;
  auto realloc            (size_type const c) -> bool;
};


/**
 * @brief Constructor. Sets the initial size of the vector, calculates its capacity
 *        And allocates memory.
 */
template <typename IntType>
inline BitVectorT<IntType>::BitVectorT(size_type s, value_type val)
  : currentSize     {s}
  , currentCapacity {minCapacity(currentSize)}
  , data            {allocateMemory(currentCapacity / regionSize)}
{
  assign(s, val);
}

/**
 * @brief Constructor. Sets the initial size of the vector, calculates its capacity
 *        And allocates memory.
 */
template <typename IntType>
inline BitVectorT<IntType>::BitVectorT(std::initializer_list<int> l)
  : BitVectorT()
{
  reserve(l.size());
  for(const auto b : l)
  {
    push_back(b);
  }
}

/**
 * @brief Constructor. Sets the initial size of the vector, calculates its capacity,
 *        allocates memory and copies the content of the given array
 */
template <typename IntType>
template <typename T, std::size_t N>
inline BitVectorT<IntType>::BitVectorT(T const (&src)[N])
  : BitVectorT(N * sizeof(T) * 8ULL)
{
  std::memcpy(data.get(), src, N * sizeof(T));
}

/**
 * @brief copy constructor.
 */
template <typename IntType>
inline BitVectorT<IntType>::BitVectorT(BitVectorT const& src)
  : BitVectorT(src.size())
{
  std::memcpy(data.get(), src.data.get(), currentCapacity / 8ULL);
}

/**
 * @brief move constructor.
 */
template <typename IntType>
inline BitVectorT<IntType>::BitVectorT(BitVectorT&& src) noexcept
  : currentSize     {std::move(src.currentSize)}
  , currentCapacity {std::move(src.currentCapacity)}
  , data            {std::move(src.data)}
{
  src.currentSize     = 0ULL;
  src.currentCapacity = 0ULL;
}

/**
 * @brief assignment operator.
 */
template <typename IntType>
inline auto BitVectorT<IntType>::operator=(BitVectorT src) noexcept -> BitVectorT&
{
  this->swap(src);
  return *this;
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
 * @brief Swaps the content of the bit vector with the content of src.
 */
template <typename IntType>
void BitVectorT<IntType>::swap(BitVectorT& src) noexcept
{
  using std::swap;
  swap(currentSize, src.currentSize);
  swap(currentCapacity, src.currentCapacity);
  data.swap(src.data);
}

/**
 * @brief  Increase the capacity of the vector to a value that's greater or equal to newCapacity.
 *         If newCapacity is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
 * @remark If newCapacity is greater than capacity(), all iterators, including the past-the-end iterator,
 *         and all references to the elements are invalidated. Otherwise, no iterators or references are invalidated. 
 */
template <typename IntType>
inline void BitVectorT<IntType>::reserve(size_type newCapacity)
{
  if(newCapacity > currentCapacity)
  {
    realloc(cpptools::alignUp(newCapacity, regionSize));
  }
}

/**
 * @brief  Requests the removal of unused capacity.
 * @remark If reallocation occurs, all iterators, including the past the end iterator,
 *         and all references to the elements are invalidated. If no reallocation takes place,
 *         no iterators or references are invalidated. 
 */
template <typename IntType>
inline void BitVectorT<IntType>::shrink_to_fit()
{
  if(const auto newCapacity = minCapacity(currentSize); newCapacity < currentCapacity)
  {
    realloc(newCapacity);
  }
}

/**
 * @brief  Resizes the container to contain size elements.
 * @remark If the current size is greater than size, the container is reduced to its first size elements.
 *         If the current size is less than size, additional copies of val are appended.
 */
template <typename IntType>
void BitVectorT<IntType>::resize(size_type size, value_type val)
{
  if(size < currentSize)
  {
    currentSize = size;
    shrink_to_fit();
  }
  else if(size > currentSize)
  {
    if(const auto requiredCapacity = minCapacity(size); requiredCapacity > currentCapacity)
    {
      realloc(requiredCapacity);
    }

    const auto currentMinCapacity = minCapacity(currentSize);
    if(size >= currentMinCapacity)
    {
      auto numNewElements = size - currentSize;

      while(currentSize < currentMinCapacity)
      {
        (*this)[currentSize] = val;
        ++currentSize;
        --numNewElements;
      }

      if(const auto numCompleteRegions = numNewElements / regionSize; numCompleteRegions > 0)
      {
        std::memset(data.get() + (currentSize / regionSize), val ? static_cast<int>(~0) : 0, numCompleteRegions * byteSize);
        currentSize += numCompleteRegions * regionSize;
      }

      if(const auto numRemainingElements = numNewElements % regionSize; numRemainingElements > 0ULL)
      {
        data[currentSize / regionSize] = val ? punchMask<byte>(numRemainingElements) : byte{};
      }

      currentSize = size;
    }
    else
    {
      for(const auto s = currentSize; size > s; --size)
      {
        push_back(val);
      }
    }
  }
}

/**
 * @brief Replaces the content with size copies of val.
 */
template <typename IntType>
void BitVectorT<IntType>::assign(size_type size, value_type val)
{
  if(size < regionSize)
  {
    data[0ULL] = punchMask<byte>(size);
  }
  else
  {
    if(const auto requiredCapacity = minCapacity(size); requiredCapacity > currentCapacity)
    {
      data            = allocateMemory(requiredCapacity / regionSize);
      currentCapacity = requiredCapacity;
    }

    const auto numCompleteRegions = size / regionSize;
    std::memset(data.get(), val ? static_cast<int>(~0) : 0, numCompleteRegions * byteSize);
    data[numCompleteRegions] = val ? punchMask<byte>(size % regionSize) : byte{};
  }

  currentSize = size;
}

/**
 * @brief  Reallocates the memory area holding the bits.
 * @remark the new capacity has to be aligned to the regionSize.
 */
template <typename IntType>
auto BitVectorT<IntType>::realloc(size_type const newCapacity) -> bool
{
  auto* const newStorage = new(std::nothrow) IntType[newCapacity / regionSize];
  if(newStorage != nullptr)
  {
    if(data)
    {
      std::memcpy(newStorage, data.get(), newCapacity / 8ULL);
    }
    data.reset(newStorage);
    currentCapacity = newCapacity;

    return true;
  }

  return false;
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
 * @brief Computes the minimum capacity that is required to hold the stored bits.
 */
template <typename IntType>
auto BitVectorT<IntType>::minCapacity(size_type size) const -> size_type
{
  return cpptools::alignUp(size, regionSize);
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
auto BitVectorT<IntType>::allocateMemory(size_type numRegions) const -> dataPtr
{
  return std::make_unique<byte[]>(numRegions);
}

/**
 * @brief Checks if the content of lhs and rhs are equal, that is they have the same size
 *        and all bits are equal.
 */
template <typename IntType1, typename IntType2>
auto operator==(BitVectorT<IntType1> const& lhs, BitVectorT<IntType2> const& rhs)
{
  if(const auto s = lhs.size(); s == rhs.size())
  {
    for(std::size_t i = 0ULL; i < s; ++i)
    {
      if(lhs[i] != rhs[i])
      {
        return false;
      }
    }

    return true;
  }

  return false;
}

template <typename IntType1, typename IntType2>
auto operator!=(BitVectorT<IntType1> const& lhs, BitVectorT<IntType2> const& rhs)
{
  return !(lhs == rhs);
}


}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITVECTOR_H_315772235374352685828765467722392194654870 
