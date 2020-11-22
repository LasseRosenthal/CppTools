/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StackVector.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    22.09.2020 
 */ 
 
#ifndef STACKVECTOR_H_55732479830412573150983114514584624513250 
#define STACKVECTOR_H_55732479830412573150983114514584624513250 
 
 
// includes
#include <MultiIndexVector/MultiIndexArray.h>

#include <cmath>


namespace multiindex {


/**
 * @class StackVector
 * @brief
 */
template <typename T, storageOrdering Ordering, std::size_t N,
          typename Enable = std::enable_if_t<std::is_arithmetic_v<T>>>
class StackVector : public MultiIndexArray<T, Ordering, N> {

  using Base = MultiIndexArray<T, Ordering, N>;

public:

  // ---------------------------------------------------
  // public types
  using value_type      = typename Base::value_type;
  using reference       = typename Base::reference;
  using const_reference = typename Base::const_reference;
  using pointer         = typename Base::pointer;
  using const_pointer   = typename Base::const_pointer;
  using size_type       = typename Base::size_type;
  using iterator        = pointer;
  using const_iterator  = const_pointer;

  // ---------------------------------------------------
  // inherit the constructors from Base
  using Base::Base;

  // ---------------------------------------------------
  // public api
  auto magnitude () const -> double;
};


/**
 * @brief Returns the magnitude of the vector.
 */
template <typename T, storageOrdering Ordering, std::size_t N, typename Enable>
inline auto StackVector<T, Ordering, N, Enable>::magnitude () const -> double
{
  return std::sqrt(*this * *this);
}

/**
 * @brief convenient alias for column vectors.
 */
template <typename T, std::size_t N>
using ColumnStackVector = StackVector<T, storageOrdering::columnMajor, N>;

/**
 * @brief convenient alias for row vectors.
 */
template <typename T, std::size_t N>
using RowStackVector = StackVector<T, storageOrdering::rowMajor, N>;



/**
 * @brief multiplication of a StackVector with a scalar.
 */
template <typename T, storageOrdering Ordering, std::size_t N>
inline auto operator*(StackVector<T, Ordering, N> const& x, T factor)
{
  StackVector<T, Ordering, N> result(x);
  result *= factor;
  return result;
}

template <typename T, storageOrdering Ordering, std::size_t N>
inline auto operator*(T factor, StackVector<T, Ordering, N> const& x)
{
  return x * factor;
}

/**
 * @brief dot product of a two vectors with same ordering.
 */
template <typename T, typename S, storageOrdering Ordering, std::size_t N>
inline auto operator*(StackVector<T, Ordering, N> const& x, StackVector<S, Ordering, N> const& y)
{
  return simpleDotProduct(x.data(), y.data(), N);
}

/**
 * @brief dot product of a column vector and a row vector.
 */
template <typename T, typename S, std::size_t N>
inline auto operator*(RowStackVector<T, N> const& x, ColumnStackVector<S, N> const& y) 
{
  return simpleDotProduct(x.data(), y.data(), N);
}

/**
 * @brief returns the oriented angle between two vectors.
 */
template <typename T, typename S, storageOrdering Ordering, std::size_t N>
inline auto orientedAngle(StackVector<T, Ordering, N> const& x, StackVector<S, Ordering, N> const& y)
{
  return std::acos((x * y) / (x.magnitude() * y.magnitude()));
}


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STACKVECTOR_H_55732479830412573150983114514584624513250 
