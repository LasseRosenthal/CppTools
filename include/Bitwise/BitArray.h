/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitArray.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    27.08.2021 
 */ 
 
#ifndef BITARRAY_H_212652356445020102219561271260551447991 
#define BITARRAY_H_212652356445020102219561271260551447991 
 
 
// includes
#include <Bitwise/BitFieldIterator.h>
#include <Meta/Utility.h>
#include <Utils/miscellaneous.h>

#include <array>


namespace bws {

 
/** 
 * @class BitArray
 * @brief 
 */
template <std::size_t Size>
class BitArray {

  static_assert(Size > 0ULL, "BitArray requires Size > 0");

public:
  // ---------------------------------------------------
  // private types
  using storageType = std::conditional_t<std::is_same_v<meta::MinIntegralType<Size>, void>,
                                         std::uint64_t, meta::MinIntegralType<Size>>;

  // ---------------------------------------------------
  //  private constants
  static constexpr std::size_t byteSize   = sizeof(storageType);
  static constexpr std::size_t regionSize = 8ULL * byteSize;
  static constexpr std::size_t numRegions = cpptools::alignUp<regionSize>(Size) / regionSize;

public:

  // ---------------------------------------------------
  // public types
  using size_type       = std::size_t;
  using value_type      = bool;
  using reference       = BitProxy<storageType, false>;
  using const_reference = BitProxy<storageType, true>;
  using iterator        = BitFieldIterator<storageType, false>;
  using const_iterator  = BitFieldIterator<storageType, true>;

  static constexpr size_type size = Size;

  // ---------------------------------------------------
  // ctor & dtor
  constexpr BitArray (value_type v = {}) noexcept;

private:

  // ---------------------------------------------------
  // private data
  bool b;
  storageType data[numRegions];
//  std::array<storageType, numRegions> data;



  //template <std::size_t... Is>
  //static constexpr bool setImpl(storageType (&a)[numRegions], std::index_sequence<Is...>, value_type v)
  //{
  //  (..., [&a, v](std::size_t i) { reference(a[i / regionSize], i % regionSize) = v; }(Is));
  //  return true;
  //}
  //
  //template <std::size_t N>
  //static constexpr bool set(storageType (&a)[numRegions], value_type v)
  //{
  //  return setImpl(a, std::make_index_sequence<N>{}, v);
  //}


};

//template <std::size_t Size>
//constexpr BitArray<Size>::BitArray(value_type v) noexcept
//  : b{set<Size>(data, v)}
//{
//
//}




}   // namespace bws


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITARRAY_H_212652356445020102219561271260551447991 
