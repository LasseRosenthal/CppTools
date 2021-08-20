/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file    MultiIndexVector.h
 * @brief
 *
 * @author  Lasse Rosenthal
 * @date    25.08.2020
 */

#ifndef MULTIINDEXVECTOR_H_249948450192443135928416979411680353020846
#define MULTIINDEXVECTOR_H_249948450192443135928416979411680353020846


// includes
#include <MultiIndexVector/MultiArrayAccessor.h>

#include <initializer_list>
#include <utility>
#include <vector>


namespace multiindex {


/**
 * @class MultiIndexVector
 * @brief MultiIndexVector provides multi-dimensional access to a dynamic linear array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
class MultiIndexVector {

  // ---------------------------------------------------
  // private SFINAE type
  template <typename... Is>
  using enableIndices = std::enable_if_t<sizeof...(Is) == Dimension>;

  using ArrayAccessor = MultiArrayAccessor<T, Dimension, Ordering>;

public:

  // ---------------------------------------------------
  // public types
  using value_type      = typename ArrayAccessor::value_type;
  using reference       = typename ArrayAccessor::reference;
  using const_reference = typename ArrayAccessor::const_reference;
  using pointer         = typename ArrayAccessor::pointer;
  using const_pointer   = typename ArrayAccessor::const_pointer;
  using size_type       = typename ArrayAccessor::size_type;

  // ---------------------------------------------------
  // public constants
  static constexpr std::size_t     rank     = Dimension;
  static constexpr storageOrdering ordering = Ordering;

  // ---------------------------------------------------
  // ctor & dtor
  MultiIndexVector();
  template <typename... Dimensions, typename = enableIndices<Dimensions...>>
  MultiIndexVector(Dimensions... dimensions);
  template <typename... Dimensions, typename = enableIndices<Dimensions...>>
  MultiIndexVector(std::initializer_list<T>&& vals, Dimensions... dimensions);

  

  // ---------------------------------------------------
  // public api
  [[nodiscard]] auto size  () const noexcept -> size_type;
  [[nodiscard]] auto size  (size_type i) const -> size_type;
  [[nodiscard]] auto empty () const noexcept -> bool;
  template <typename... Is>
  auto operator()          (Is... is) -> reference;
  template <typename... Is>
  auto operator()          (Is... is) const -> const_reference;
  auto data                () -> pointer;
  auto data                () const -> const_pointer;

  template <typename... Dimensions, typename = enableIndices<Dimensions...>>
  void resize(Dimensions... dimensions);

private:

  // ---------------------------------------------------
  // private data
  std::vector<value_type>     values;
  std::array<size_type, rank> dimensions;
  ArrayAccessor               arrayAccessor;
  size_type                   totalSize{};

  // ---------------------------------------------------
  // private methods
  template <std::size_t... Is>
  constexpr auto zeroArray(std::index_sequence<Is...>);
  template <typename Tuple, std::size_t... Is>
  constexpr void setDimensions(Tuple&&, std::index_sequence<Is...>);
};


/**
 * @brief Constructor.
 * @param dims a parameter pack that determines the dimensions of the array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <typename... Dimensions, typename>
inline MultiIndexVector<T, Dimension, Ordering>::MultiIndexVector(Dimensions... dims)
  : values        ((... * dims))
  , dimensions    {dims...}
  , arrayAccessor {dimensions.data(), values.data()}
  , totalSize     {(... * dims)}
{}

template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <typename... Dimensions, typename>
inline MultiIndexVector<T, Dimension, Ordering>::MultiIndexVector(std::initializer_list<T>&& vals, Dimensions... dims)
  : values        {std::forward<decltype(vals)>(vals)}
  , dimensions    {dims...}
  , arrayAccessor {dimensions.data(), values.data()}
  , totalSize     {(... * dims)}
{}

/**
 * @brief Default constructor. Construct an empty array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
inline MultiIndexVector<T, Dimension, Ordering>::MultiIndexVector()
  : dimensions {zeroArray(std::make_index_sequence<rank>{})}
{}

/**
 * @brief Returns the total size of the array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
inline auto MultiIndexVector<T, Dimension, Ordering>::size() const noexcept -> size_type
{
  return totalSize;
}

/**
 * @brief Returns the size of the i-th dimension of the array.
 * @param i the zero based index of the dimension.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
inline auto MultiIndexVector<T, Dimension, Ordering>::size(size_type i) const -> size_type
{
  return dimensions[i];
}

/**
 * @brief Checks if the array is empty, i.e if its size equals zero.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
inline auto MultiIndexVector<T, Dimension, Ordering>::empty() const noexcept -> bool
{
  return totalSize == 0ULL;
}

/**
 * @brief Index operator. Returns a reference to the element spcified by the given indices.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <typename... Is>
inline auto MultiIndexVector<T, Dimension, Ordering>::operator()(Is... is) -> reference
{
  return arrayAccessor(is...);
}

/**
 * @brief Index operator. Returns a const reference to the element spcified by the given indices.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <typename... Is>
inline auto MultiIndexVector<T, Dimension, Ordering>::operator()(Is... is) const -> const_reference
{
  return arrayAccessor(is...);
}

/**
 * @brief Returns a raw pointer to the underlying array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
inline auto MultiIndexVector<T, Dimension, Ordering>::data() -> pointer
{
  return values.data();
}

/**
 * @brief Returns a const raw pointer to the underlying array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
inline auto MultiIndexVector<T, Dimension, Ordering>::data() const -> const_pointer
{
  return values.data();
}


template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <typename Tuple, std::size_t... Is>
constexpr void MultiIndexVector<T, Dimension, Ordering>::setDimensions(Tuple&& dimVals, std::index_sequence<Is...>)
{
  (..., [this, &dimVals](std::size_t index, std::size_t val) { dimensions[index] = val;}(Is, std::get<Is>(dimVals)));
}

/**
 * @brief Resizes the array.
 * @param dims a parameter pack that determines the dimensions of the array.
 */
template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <typename... Dimensions, typename>
void MultiIndexVector<T, Dimension, Ordering>::resize(Dimensions... dims)
{
  totalSize = (... * dims);
  values.resize(totalSize);
  setDimensions(std::forward_as_tuple(dims...), std::index_sequence_for<Dimensions...>{});
  arrayAccessor.data       = values.data();
  arrayAccessor.dimensions = dimensions.data();
}


template <typename T, std::size_t Dimension, storageOrdering Ordering>
template <std::size_t... Is>
constexpr auto MultiIndexVector<T, Dimension, Ordering>::zeroArray(std::index_sequence<Is...>)
{
  return std::array<std::size_t, sizeof...(Is)>{std::get<Is>(std::make_tuple(Is...)) - Is...};
}


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 

#endif   // MULTIINDEXVECTOR_H_249948450192443135928416979411680353020846
