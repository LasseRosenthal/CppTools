/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Arithmetic.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.11.2020 
 */ 
 
#ifndef ARITHMETIC_H_1981130352311351360013118273808660180774532 
#define ARITHMETIC_H_1981130352311351360013118273808660180774532 
 
 
// includes
#include <Unit/UnitSystem.h>
 

namespace unit {
namespace arithmetic {


/**
 * @brief UnitDimensionTagListGeneratorT creates a list of UnitSystemBaseTags from a list of BaseDimensionTags.
 *        All UnitSystemBaseTags have a period of one.
 */
template <typename BaseDimTagList, typename>
struct UnitDimensionTagListGeneratorT;

template <typename BaseDimTagList, std::size_t... Is>
struct UnitDimensionTagListGeneratorT<BaseDimTagList, std::index_sequence<Is...>>
  : meta::IdentityT<meta::Typelist<UnitSystemBaseTag<meta::typelist::NthElement<BaseDimTagList, Is>, std::ratio<1>>...>> {};

template <typename BaseDimTagList>
using UnitDimensionTagListGenerator = typename UnitDimensionTagListGeneratorT<BaseDimTagList, dimension::BaseDimensionIndices>::type;


/**
 * @brief UnitSystemForDimension is an alias for a UnitSystem belonging to a given dimension.
 */
template <typename Dimension, typename ScalingFactor = std::ratio<1>>
using UnitSystemForDimension = UnitSystem<UnitDimensionTagListGenerator<typename Dimension::baseDimTags>, ScalingFactor>;


/**
 * @brief MultiplicationType is an alias for the UnitSystem that results from the multiplication
 *        of two UnitSystems.
 */
template <typename UnitSystemT1, typename UnitSystemT2>
using MultiplicationType =
  UnitSystemForDimension<dimension::MultiplicationType<DimensionOf<UnitSystemT1>, DimensionOf<UnitSystemT2>>>;

/**
 * @brief DivisionType is an alias for the UnitSystem that results from the division
 *        of two UnitSystems.
 */
template <typename UnitSystemT1, typename UnitSystemT2>
using DivisionType = UnitSystemForDimension<dimension::DivisionType<DimensionOf<UnitSystemT1>, DimensionOf<UnitSystemT2>>>;

/**
 * @brief InversionType is an alias for the UnitSystem that results from the inversion
 *        of a unit system.
 */
template <typename UnitSystemT>
using InversionType = UnitSystemForDimension<dimension::InversionType<DimensionOf<UnitSystemT>>, meta::Invert<ScalingOf<UnitSystemT>>>;

template <typename UnitSystemT>
using InversionOf = UnitSystem<meta::typelist::Transform<UnitSystemBaseTagListOf<UnitSystemT>, InversionOfUnitSystemBaseTagT>,
                               meta::Invert<ScalingOf<UnitSystemT>>>;

/**
 * @brief NthPower is an alias for the UnitSystem that results from taking the N-th power
 *        of a unit system.
 */
template <typename UnitSystemT, std::size_t N>
using NthPower = UnitSystem<meta::typelist::Transform<UnitSystemBaseTagListOf<UnitSystemT>, NthPowerOfUnitSystemBaseTagGenerator<N>>,
                            ScalingOf<UnitSystemT>>;

/**
 * @brief NthRoot is an alias for the UnitSystem that results from taking the N-th root
 *        of a unit system.
 */
template <typename UnitSystemT, std::size_t N>
using NthRoot = UnitSystem<meta::typelist::Transform<UnitSystemBaseTagListOf<UnitSystemT>, NthRootOfUnitSystemBaseTagGenerator<N>>,
                           ScalingOf<UnitSystemT>>;


}   // namespace arithmetic
}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ARITHMETIC_H_1981130352311351360013118273808660180774532 
