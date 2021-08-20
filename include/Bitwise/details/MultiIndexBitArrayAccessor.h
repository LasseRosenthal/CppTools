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
#include <Bitwise/BitProxy.h>
#include <MultiIndexVector/AdressCalculator.h>

#include <type_traits>


namespace bws {


/** 
 * @class  MultiIndexBitArrayAccessor 
 * @brief  MultiIndexBitArrayAccessor provides multidimensional bitwise access to linear storage.
 * @tparam storageOrdering a value of type /link #storageOrdering /endlink that specifies the method of calculation.
 * @see    see https://en.wikipedia.org/wiki/Row-_and_column-major_order
 */
template <typename IntType, std::size_t Dimension, multiindex::storageOrdering Ordering>
class MultiIndexBitArrayAccessor {

  // ---------------------------------------------------
  // private types
  using byte_type = IntType;
  using ptr       = byte_type*;

  // ---------------------------------------------------
  // private SFINAE type

public:

  // ---------------------------------------------------
  // public types
  using size_type       = std::size_t;
  using value_type      = bool;
  using reference       = BitProxy<byte_type, false>;
  using const_reference = BitProxy<byte_type, true>;

  static constexpr storageOrdering ordering  = Ordering;
  static constexpr std::size_t     dimension = Dimension;

private:

  // ---------------------------------------------------
  // private types
  using addresCalculator = AdressCalculator<Dimension, ordering>;

  // ---------------------------------------------------
  // private data
  byte_type* data       {};
  size_type* dimensions {};


};




template <typename IntType, std::size_t N, std::size_t Dimension, multiindex::storageOrdering Ordering>
class MultiIndexBitArrayAccessor<IntType[N], Dimension, Ordering> {


  // ---------------------------------------------------
  // private types
  using byte_type = IntType;

public:

  // ---------------------------------------------------
  // public types & constants
  using size_type       = std::size_t;
  using value_type      = bool;
  using reference       = BitProxy<byte_type, false>;
  using const_reference = BitProxy<byte_type, true>;

  static constexpr storageOrdering ordering  = Ordering;
  static constexpr std::size_t     dimension = Dimension;


private:

  // ---------------------------------------------------
  // private types
  using addresCalculator = AdressCalculator<Dimension, ordering>;

  // ---------------------------------------------------
  // private data
  const size_type (&dimensions)[Dimension];
  byte_type (&data)[N];

};




}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTIINDEXBITARRAYACCESSOR_H_25219104532175230521288479997444623318456 
