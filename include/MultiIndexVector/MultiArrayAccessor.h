/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    MultiArrayAccessor.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.08.2020 
 */ 
 
#ifndef MULTIARRAYACCESSOR_H_29973719537402120331661167725556265542181 
#define MULTIARRAYACCESSOR_H_29973719537402120331661167725556265542181 
 
 
// includes
#include <MultiIndexVector/AdressCalculator.h>
#include <MultiIndexVector/IteratorRange.h>

#include <type_traits>
 

namespace multiindex {

 
/** 
 * @class  MultiArrayAccessor 
 * @brief  MultiArrayAccessor provides multidimensional access to linear storage.
 * @tparam storageOrdering a value of type /link #storageOrdering /endlink that specifies the method of calculation.
 * @see    see https://en.wikipedia.org/wiki/Row-_and_column-major_order
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
class MultiArrayAccessor {

  // ---------------------------------------------------
  // private SFINAE type
  template <typename... Is>
  using enableIndices = std::enable_if_t<sizeof...(Is) == Dimension>;

public:

  // ---------------------------------------------------
  // public types
  using value_type      = T;
  using reference       = value_type&;
  using const_reference = value_type const&;
  using pointer         = value_type*;
  using const_pointer   = value_type const*;
  using iterator        = pointer;
  using const_iterator  = const_pointer;
  using size_type       = std::size_t;

  static constexpr storageOrdering ordering  = Ordering;
  static constexpr std::size_t     dimension = Dimension;

  // ---------------------------------------------------
  // public api
  template<typename... Is, typename = enableIndices<Is...>>
  auto operator ()(Is... is) -> reference;
  template<typename... Is, typename = enableIndices<Is...>>
  auto operator ()(Is... is) const -> const_reference;

  // ---------------------------------------------------
  // private types
  using addressCalculator = AdressCalculator<Dimension, ordering>;

  // ---------------------------------------------------
  // private data
  std::size_t* dimensions;
  value_type*  data;
};


template <typename T, std::size_t Dimension, storageOrdering Ordering>
template<typename... Is, typename>
inline auto MultiArrayAccessor<T, Dimension, Ordering>::operator()(Is... is) -> reference
{
  return data[addressCalculator::transform(dimensions, is...)];
}

template <typename T, std::size_t Dimension, storageOrdering Ordering>
template<typename... Is, typename>
inline auto MultiArrayAccessor<T, Dimension, Ordering>::operator()(Is... is) const -> const_reference
{
  return data[addressCalculator::transform(dimensions, is...)];
}


/**
 * @class  MultiArrayAccessor
 * @brief  Specialization for array types.
 */
template <typename T, std::size_t N, std::size_t Dimension, storageOrdering Ordering>
struct MultiArrayAccessor<T[N], Dimension, Ordering> {

   // ---------------------------------------------------
   // private SFINAE type
   template <typename... Is>
   using enableIndices = std::enable_if_t<sizeof...(Is) == Dimension>;

   // ---------------------------------------------------
   // public types
   using value_type           = T;
   using reference            = value_type&;
   using const_reference      = value_type const&;
   using pointer              = value_type*;
   using const_pointer        = value_type const*;
   using iterator             = pointer;
   using const_iterator       = const_pointer;
   using size_type            = std::size_t;
   using iterator_range       = IteratorRange<iterator>;
   using const_iterator_range = IteratorRange<const_iterator>;


   static constexpr storageOrdering ordering  = Ordering;
   static constexpr std::size_t     dimension = Dimension;

   // ---------------------------------------------------
   // public api
   template <typename... Is, typename = enableIndices<Is...>>
   constexpr auto operator ()(Is... is) -> reference;
   template <typename... Is, typename = enableIndices<Is...>>
   constexpr auto operator ()(Is... is) const -> const_reference;
   constexpr auto operator [](size_type i) -> iterator_range;
   constexpr auto operator [](size_type i) const -> const_iterator_range;

   // ---------------------------------------------------
   // private types
   using addressCalculator = AdressCalculator<Dimension, ordering>;

   // ---------------------------------------------------
   // private data
   const size_type (&dimensions)[Dimension];
   value_type (&data)[N];
};


template <typename T, std::size_t N, std::size_t Dimension, storageOrdering Ordering>
template <typename... Is, typename>
constexpr auto MultiArrayAccessor<T[N], Dimension, Ordering>::operator()(Is... is) -> reference
{
  return data[addressCalculator::transform(dimensions, is...)];
}

template <typename T, std::size_t N, std::size_t Dimension, storageOrdering Ordering>
template <typename... Is, typename>
constexpr auto MultiArrayAccessor<T[N], Dimension, Ordering>::operator()(Is... is) const -> const_reference
{
  return data[addressCalculator::transform(dimensions, is...)];
}

template <typename T, std::size_t N, std::size_t Dimension, storageOrdering Ordering>
constexpr auto MultiArrayAccessor<T[N], Dimension, Ordering>::operator[](size_type i) -> iterator_range
{
  return iterator_range{data + addressCalculator::subDivision(dimensions, i), data + addressCalculator::subDivision(dimensions, i+1ULL)};
}

template <typename T, std::size_t N, std::size_t Dimension, storageOrdering Ordering>
constexpr auto MultiArrayAccessor<T[N], Dimension, Ordering>::operator[](size_type i) const -> const_iterator_range
{
  return const_iterator_range{data + addressCalculator::subDivision(dimensions, i), data + addressCalculator::subDivision(dimensions, i+1ULL)};
}


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTIARRAYACCESSOR_H_29973719537402120331661167725556265542181 
