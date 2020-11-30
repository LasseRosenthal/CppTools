/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    UnitSystem.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.11.2020 
 */ 
 
#ifndef UNITSYSTEM_H_286926169253693490116142721512411129692808 
#define UNITSYSTEM_H_286926169253693490116142721512411129692808 
 
 
// includes 
#include <Unit/Dimension.h>


namespace unit {


/**
 * @struct UnitSystemBaseTag
 * @brief  UnitSystemBaseTag
 */
template <typename BaseDimTag, typename Period>
struct UnitSystemBaseTag {
  using baseDimTag = BaseDimTag;
  using period     = Period;
};

/**
 * @struct BaseDimTagOfT
 * @brief  BaseDimTagOfT is a metafunction providing a type alias for the BaseDimensionTag of a UnitSystemBaseTag.
 */
template <typename UnitSystemBaseTagT>
struct BaseDimTagOfT : meta::IdentityT<typename UnitSystemBaseTagT::baseDimTag> {};

template <typename UnitSystemBaseTagT>
using BaseDimTagOf = typename BaseDimTagOfT<UnitSystemBaseTagT>::type;

/**
 * @struct PeriodOfT
 * @brief  PeriodOfT is a metafunction providing a type alias for the period of a UnitSystemBaseTag.
 */
template <typename UnitSystemBaseTagT>
struct PeriodOfT : meta::IdentityT<typename UnitSystemBaseTagT::period> {};

template <typename UnitSystemBaseTagT>
using PeriodOf = typename PeriodOfT<UnitSystemBaseTagT>::type;

/**
 * @struct BaseDimIndexOfT
 * @brief  BaseDimIndexOfT provides a type alias for the index of the nested BaseDimensionTag of a UnitSystemBaseTag.
 */
template <typename UnitSystemBaseTagT>
struct BaseDimIndexOfT : dimension::IndexOfT<BaseDimTagOf<UnitSystemBaseTagT>> {};

template <typename UnitSystemBaseTagT>
constexpr auto BaseDimIndexOf = BaseDimIndexOfT<UnitSystemBaseTagT>::value;

/**
 * @struct UnitSystemBaseTagUnityPeriodT
 * @brief  UnitSystemBaseTagUnityPeriodT maps UnitSystemBaseTag to a UnitSystemBaseTag
 *         with same BaseDimensionTag and a period of 1.
 */
template <typename UnitSystemBaseTagT>
struct UnitSystemBaseTagUnityPeriodT : meta::IdentityT<UnitSystemBaseTag<BaseDimTagOf<UnitSystemBaseTagT>, std::ratio<1>>> {};

/**
 * @struct UnitSystem
 * @brief  UnitSystem defines a unit as a composition of several UnitSystemBaseTag.
 */
template <typename UnitSystemBaseTagList, typename ScalingFactor = std::ratio<1>>
struct UnitSystem {
  using unitSystemBaseTags = UnitSystemBaseTagList;
  using baseDimTags        = meta::typelist::Transform<unitSystemBaseTags, BaseDimTagOfT>;
  using dimension          = dimension::Dimension<baseDimTags>;
  using periods            = meta::typelist::Transform<unitSystemBaseTags, PeriodOfT>;
  using scalingFactor      = ScalingFactor;
};

template <typename UnitSystemT>
struct BaseUnitGeneratorT {
  using unitDimensionBaseTags = meta::typelist::Transform<typename UnitSystemT::unitSystemBaseTags, UnitSystemBaseTagUnityPeriodT>;
  using type                  = UnitSystem<unitDimensionBaseTags, typename UnitSystemT::scalingFactor>;
};

template <typename UnitSystemT>
using BaseUnitGenerator = typename BaseUnitGeneratorT<UnitSystemT>::type;

/**
 * @struct DimensionOfT
 * @brief  DimensionOfT provides an alias for the dimension of a UnitSystem.
 */
template <typename UnitSystemT>
struct DimensionOfT : meta::IdentityT<typename UnitSystemT::dimension> {};

template <typename UnitSystemT>
using DimensionOf = typename DimensionOfT<UnitSystemT>::type;

/**
 * @brief PeriodListOf ia an alias for the period list of a UnitSystem.
 */
template <typename UnitSystemT>
using PeriodListOf = typename UnitSystemT::periods;

/**
 * @brief ScalingOf ia an alias for the scalingFactor of a UnitSystem.
 */
template <typename UnitSystemT>
using ScalingOf = typename UnitSystemT::scalingFactor;


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITSYSTEM_H_286926169253693490116142721512411129692808 
