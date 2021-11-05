/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BaseDimensionTag.h 
 * @brief   definition of the BaseDimensionTag structure
 *          and several metafunctions 
 * 
 * @author  Lasse Rosenthal 
 * @date    17.11.2020 
 */ 
 
#ifndef BASEDIMENSIONTAG_H_2535710249281077892891128511070223426291 
#define BASEDIMENSIONTAG_H_2535710249281077892891128511070223426291 
 
 
// includes
#include <Meta/RatioUtils.h>
#include <Meta/Utility.h>
#include <Meta/Valuelist.h>
 

namespace unit {
namespace dimension {


/** 
 * @struct BaseDimensionTag
 * @brief
 * @tparam Index the index value of a given dimension. Every base dimension has a unique
 *         index which is used to build compound type lists of different base dimensions.
 * @tparam Exponent a rational exponent that can be used
 *         to build derived units.
 */
template <std::size_t Index, typename Exponent = std::ratio<0>>
struct BaseDimensionTag {
  static constexpr auto index               = Index;
  using exponent                            = Exponent;
  static constexpr bool hasRationalExponent = meta::IsIntegralRatio<exponent>;
};

/** 
 * @struct ExponentOfT
 * @brief  ExponentOfT is a metafunction returning the type of the
 *         exponent of a given Unit tag.
 */
template <typename BaseDimTag>
struct ExponentOfT : meta::IdentityT<typename BaseDimTag::exponent> {};

template <typename BaseDimTag>
using ExponentOf = typename ExponentOfT<BaseDimTag>::type;

/** 
 * @struct IndexOf
 * @brief  IndexOf is a metafunction returning the index of a given Unit tag.
 */
template <typename BaseDimTag>
struct IndexOfT : std::integral_constant<std::size_t, BaseDimTag::index> {};

/**
 * @struct DimensionTagLess
 * @brief  provides a compile-time weak ordering of dimension tags
 *         based on the comparison of their indices.
 */
template <typename Tag1, typename Tag2>
struct DimensionTagLess
  : std::conditional_t<(Tag1::index < Tag2::index), std::true_type, std::false_type> {};

/**
 * @struct HasZeroExponentT
 * @brief  provides a compile-time boolean indicating if a \link #BaseDimensionTag \endlink
 *         zero as exponent.
 */
template <typename BaseDimTag>
struct HasZeroExponentT
  : std::conditional_t<std::ratio_equal_v<ExponentOf<BaseDimTag>, std::ratio<0>>, std::true_type, std::false_type> {};

/** 
 * @struct HasIndexT
 * @brief  HasIndexT defines a nested template metafunction
 *         that checks if a given Tag has a specific index.
 */
template <std::size_t Index>
struct HasIndexT {
  template <typename BaseDimTag>
  using nestedTemplate = std::conditional_t<BaseDimTag::index == Index, std::true_type, std::false_type>;
};

template <std::size_t Index>
using HasIndex = HasIndexT<Index>::template nestedTemplate;


}   // namespace dimension
}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BASEDIMENSIONTAG_H_2535710249281077892891128511070223426291 
