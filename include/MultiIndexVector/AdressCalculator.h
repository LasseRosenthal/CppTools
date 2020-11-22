/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    AdressCalculator.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.08.2020 
 */ 
 
#ifndef ADRESSCALCULATOR_H_24699411323701180211415917455324182771419433 
#define ADRESSCALCULATOR_H_24699411323701180211415917455324182771419433 
 
 
// includes
#include <MultiIndexVector/StorageOrdering.h>

#include <cstddef>
#include <tuple>


namespace multiindex {


/**
 * @class  AdressCalculator
 * @brief  AdressCalculator provides a method for the adress calculation of linearly stored elements in multidimensional arrays.
 *         Additionally, it provides a method to calculate the adress of the i-th subdivision. For example, for a two dimensional
 *         array, the i-th subdivision is the adress of the i-th row (rowMajor) or the i-th column (columnMajor), respectively.
 * @tparam D the dimension of the array.
 * @tparam storageOrdering a value of type /link #storageOrdering /endlink that specifies the method of adress calculation.
 * @see    see https://en.wikipedia.org/wiki/Row-_and_column-major_order
 */
template <std::size_t D, storageOrdering Ordering> 
struct AdressCalculator;


/**
 * @brief Partial specialization for column major ordering.
 */
template <std::size_t D>
struct AdressCalculator<D, storageOrdering::columnMajor> {
  template <typename... T>
  static constexpr auto transform(std::size_t const* const Dims, std::size_t i, T... is)
  {
    return i + *Dims * AdressCalculator<D - 1ULL, storageOrdering::columnMajor>::transform(Dims + 1, is...);
  }

  static constexpr auto subDivision(std::size_t const* const Dims, std::size_t i)
  {
    return i * *Dims * AdressCalculator<D - 1ULL, storageOrdering::columnMajor>::subDivision(Dims + 1, 1ULL);
  }
};

template <>
struct AdressCalculator<1ULL, storageOrdering::columnMajor> {
  static constexpr auto transform(std::size_t const* const, std::size_t i) noexcept
  {
    return i;
  }

  static constexpr auto subDivision(std::size_t const* const, std::size_t i) noexcept
  {
    return 1ULL;
  }
};

/**
 * @brief Partial specialization for row major ordering.
 */
template <std::size_t D>
struct AdressCalculator<D, storageOrdering::rowMajor> {
  template <typename... T>
  static constexpr auto transform(std::size_t const* const Dims, T... is)
  {
    return std::get<D - 1ULL>(std::forward_as_tuple(is...)) + 
           Dims[D - 1ULL] * AdressCalculator<D - 1ULL, storageOrdering::rowMajor>::transform(Dims, is...);
  }

  static constexpr auto subDivision(std::size_t const* const Dims, std::size_t i)
  {
    return i * Dims[D - 1ULL] * AdressCalculator<D - 1ULL, storageOrdering::rowMajor>::subDivision(Dims, 1ULL);
  }
};

template <>
struct AdressCalculator<1ULL, storageOrdering::rowMajor> {
  template <typename... T>
  static constexpr auto transform(std::size_t const* const, T... is)
  {
    return std::get<0>(std::forward_as_tuple(is...));
  }

  static constexpr auto subDivision(std::size_t const* const, std::size_t i)
  {
    return 1ULL;
  }
};


}   // namespace multiindex



// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ADRESSCALCULATOR_H_24699411323701180211415917455324182771419433 
