/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    MultiIndexBitArrayAccessor.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    18.08.2021 
 */ 
 
#ifndef MULTIINDEXBITARRAYACCESSOR_H_25219104532175230521288479997444623318456 
#define MULTIINDEXBITARRAYACCESSOR_H_25219104532175230521288479997444623318456 
 
 
// includes
#include <Bitwise/BitFieldIterator.h>
#include <MultiIndexVector/AdressCalculator.h>

#include <type_traits>


namespace bws {


/** 
 * @class  MultiIndexBitArrayAccessor 
 * @brief  MultiIndexBitArrayAccessor provides multidimensional bitwise access to linear storage.
 * @tparam storageOrdering a value of type /link #storageOrdering /endlink that specifies the method of calculation.
 * @see    see https://en.wikipedia.org/wiki/Row-_and_column-major_order
 */
template <typename IntType, std::size_t Rank, multiindex::storageOrdering Ordering>
class MultiIndexBitArrayAccessor {

  // ---------------------------------------------------
  // private types
  using byteType = IntType;
  using ptr      = byteType*;

  // ---------------------------------------------------
  // private SFINAE type
  template <typename... Is>
  using enableIndices = std::enable_if_t<sizeof...(Is) == Rank>;

public:

  // ---------------------------------------------------
  // public types
  using size_type       = std::size_t;
  using value_type      = BitProxy<byteType, false>;
  using reference       = BitProxy<byteType, false>;
  using const_reference = BitProxy<byteType, true>;
  using pointer         = value_type*;
  using const_pointer   = value_type const*;
  using iterator        = BitFieldIterator<byteType>;
  using const_iterator  = BitFieldIterator<byteType, true>;

  // ---------------------------------------------------
  // public api
  template <typename... Is, typename = enableIndices<Is...>>
  auto operator ()(Is... is) -> reference;
  template <typename... Is, typename = enableIndices<Is...>>
  constexpr auto operator ()(Is... is) const -> const_reference;

  constexpr auto regionSize () const noexcept -> size_type { return 8ULL * sizeof(byteType);}

  // ---------------------------------------------------
  // private types
  using addressCalculator = multiindex::AdressCalculator<Rank, Ordering>;

  // ---------------------------------------------------
  // private data
  byteType* const data;
  const size_type* const dimensions;
};

template <typename IntType, std::size_t Rank, multiindex::storageOrdering Ordering>
template <typename... Is, typename>
inline auto MultiIndexBitArrayAccessor<IntType, Rank, Ordering>::operator()(Is... is) -> reference
{
  const auto index = addressCalculator::transform(dimensions, is...);
  return reference{data + index / regionSize(), index % regionSize()};
}

template <typename IntType, std::size_t Rank, multiindex::storageOrdering Ordering>
template <typename... Is, typename>
constexpr auto MultiIndexBitArrayAccessor<IntType, Rank, Ordering>::operator()(Is... is) const -> const_reference
{
  const auto index = addressCalculator::transform(dimensions, is...);
  return const_reference{data + index / regionSize(), index % regionSize()};
}



template <typename IntType, std::size_t N, std::size_t Rank, multiindex::storageOrdering Ordering>
class MultiIndexBitArrayAccessor<IntType[N], Rank, Ordering> {

  // ---------------------------------------------------
  // private types
  using byteType = IntType;

  // ---------------------------------------------------
  // private SFINAE type
  template <typename... Is>
  using enableIndices = std::enable_if_t<sizeof...(Is) == Rank>;

public:

  // ---------------------------------------------------
  // public types & constants
  using size_type       = std::size_t;
  using value_type      = BitProxy<byteType, false>;
  using reference       = BitProxy<byteType, false>;
  using const_reference = BitProxy<byteType, true>;
  using pointer         = value_type*;
  using const_pointer   = value_type const*;
  using iterator        = BitFieldIterator<byteType>;
  using const_iterator  = BitFieldIterator<byteType, true>;

  // ---------------------------------------------------
  // public api
  template <typename... Is, typename = enableIndices<Is...>>
  auto           operator ()(Is... is) -> reference;
  template <typename... Is, typename = enableIndices<Is...>>
  constexpr auto operator ()(Is... is) const -> const_reference;

  constexpr auto regionSize () const noexcept -> size_type { return 8ULL * sizeof(byteType);}

  // ---------------------------------------------------
  // private types
  using addressCalculator = multiindex::AdressCalculator<Rank, Ordering>;

  // ---------------------------------------------------
  // private data
  byteType (&data)[N];
  size_type const (&dimensions)[Rank];
};


template <typename IntType, std::size_t N, std::size_t Rank, multiindex::storageOrdering Ordering>
template <typename... Is, typename>
inline auto MultiIndexBitArrayAccessor<IntType[N], Rank, Ordering>::operator()(Is... is) -> reference
{
  const auto index = addressCalculator::transform(dimensions, is...);
  return reference{data + index / regionSize(), index % regionSize()};
}

template <typename IntType, std::size_t N, std::size_t Rank, multiindex::storageOrdering Ordering>
template <typename... Is, typename>
constexpr auto MultiIndexBitArrayAccessor<IntType[N], Rank, Ordering>::operator()(Is... is) const -> const_reference
{
  const auto index = addressCalculator::transform(dimensions, is...);
  return const_reference{data + index / regionSize(), index % regionSize()};
}



}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTIINDEXBITARRAYACCESSOR_H_25219104532175230521288479997444623318456 
