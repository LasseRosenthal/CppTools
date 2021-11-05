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
 * @struct InversionOfT
 * @brief  InversionOfT provides an alias for the inverted type of a \link #UnitSystemBaseTag \endlink,
 *         i.e. the internal \link #BaseDimensionTag \endlink and the period are inverted.
 */
template <typename UnitSystemBaseTagT>
struct InversionOfUnitSystemBaseTagT
  : meta::IdentityT<UnitSystemBaseTag<dimension::InvertBaseTag<BaseDimTagOf<UnitSystemBaseTagT>>,
                                      meta::Invert<PeriodOf<UnitSystemBaseTagT>>>> {};

template <typename UnitSystemBaseTagT>
using InversionOfUnitSystemBaseTag = typename InversionOfUnitSystemBaseTagT<UnitSystemBaseTagT>::type;

/**
 * @struct NthPowerOfUnitSystemBaseTagT
 * @brief  NthPowerOfUnitSystemBaseTagT provides an alias for the N-th power of a \link #UnitSystemBaseTag \endlink.
 */
template <typename UnitSystemBaseTagT, std::size_t N>
struct NthPowerOfUnitSystemBaseTagT
  : meta::IdentityT<UnitSystemBaseTag<dimension::NthPowerOfBaseTag<BaseDimTagOf<UnitSystemBaseTagT>, N>,
                                      PeriodOf<UnitSystemBaseTagT>>> {};

template <std::size_t N>
struct NthPowerOfUnitSystemBaseTagGeneratorT {
  template <typename UnitSystemBaseTagT>
  using nestedTemplate = NthPowerOfUnitSystemBaseTagT<UnitSystemBaseTagT, N>;
};

template <std::size_t N>
using NthPowerOfUnitSystemBaseTagGenerator = NthPowerOfUnitSystemBaseTagGeneratorT<N>::template nestedTemplate;

/**
 * @struct NthRootOfUnitSystemBaseTagT
 * @brief  NthRootOfUnitSystemBaseTagT provides an alias for the N-th root of a \link #UnitSystemBaseTag \endlink.
 */
template <typename UnitSystemBaseTagT, std::size_t N>
struct NthRootOfUnitSystemBaseTagT
  : meta::IdentityT<UnitSystemBaseTag<dimension::NthRootOfBaseTag<BaseDimTagOf<UnitSystemBaseTagT>, N>,
                                      PeriodOf<UnitSystemBaseTagT>>> {};

template <std::size_t N>
struct NthRootOfUnitSystemBaseTagGeneratorT {
  template <typename UnitSystemBaseTagT>
  using nestedTemplate = NthRootOfUnitSystemBaseTagT<UnitSystemBaseTagT, N>;
};

template <std::size_t N>
using NthRootOfUnitSystemBaseTagGenerator = NthRootOfUnitSystemBaseTagGeneratorT<N>::template nestedTemplate;
 

/**
 * @struct UnitSystem
 * @brief  UnitSystem defines a unit as a composition of several UnitSystemBaseTags.
 * @tparam UnitSystemBaseTagList a typelist of different UnitSystemBaseTag structures.
 */
template <typename UnitSystemBaseTagList, typename ScalingFactor = std::ratio<1>, typename Shift = std::ratio<0>>
struct UnitSystem {
  using unitSystemBaseTags = UnitSystemBaseTagList;
  using baseDimTags        = meta::typelist::Transform<unitSystemBaseTags, BaseDimTagOfT>;  ///> the corresponding typelist of BaseDimensionTags
  using dimension          = dimension::Dimension<baseDimTags>;                             ///> the dimension of this UnitSystem
  using periods            = meta::typelist::Transform<unitSystemBaseTags, PeriodOfT>;      ///> the typelist of the BaseDimensionTag periods
  using scalingFactor      = ScalingFactor;
  using shift              = Shift;
};

/**
 * @struct BaseUnitGeneratorT
 * @brief  BaseUnitGeneratorT maps a given UnitSystem on a UnitSystem with same dimension but all periods
 *         beeing equal to std::ratio<1>
 */
template <typename UnitSystemT>
struct BaseUnitGeneratorT {
  using unitDimensionBaseTags = meta::typelist::Transform<typename UnitSystemT::unitSystemBaseTags, UnitSystemBaseTagUnityPeriodT>;
  using type                  = UnitSystem<unitDimensionBaseTags, typename UnitSystemT::scalingFactor>;
};

template <typename UnitSystemT>
using BaseUnitGenerator = typename BaseUnitGeneratorT<UnitSystemT>::type;

/**
 * @brief UnitSystemBaseTagListOf is an alias for the list of UnitSystemBaseTag of a UnitSystem.
 */
template <typename UnitSystemT>
using UnitSystemBaseTagListOf = typename UnitSystemT::unitSystemBaseTags;

/**
 * @struct DimensionOfT
 * @brief  DimensionOfT provides an alias for the dimension of a UnitSystem.
 */
template <typename UnitSystemT>
struct DimensionOfT : meta::IdentityT<typename UnitSystemT::dimension> {};

template <typename UnitSystemT>
using DimensionOf = typename DimensionOfT<UnitSystemT>::type;

/**
 * @brief PeriodListOf is an alias for the period list of a UnitSystem.
 */
template <typename UnitSystemT>
using PeriodListOf = typename UnitSystemT::periods;

/**
 * @brief ScalingOf is an alias for the scalingFactor of a UnitSystem.
 */
template <typename UnitSystemT>
using ScalingOf = typename UnitSystemT::scalingFactor;

/**
 * @brief ShiftOf is an alias for the shift of a UnitSystem.
 */
template <typename UnitSystemT>
using ShiftOf = typename UnitSystemT::shift;

template <typename UnitSystemT, typename T = double>
constexpr auto ShiftValue = meta::AsDecimal<ShiftOf<UnitSystemT>, T>;

/**
 * @brief IsShifted is a compile time boolean indicating whether a given
 *        UnitSystem has a non-zero shift.
 */
template <typename UnitSystemT>
constexpr bool IsShifted = !meta::HasZeroNominator<ShiftOf<UnitSystemT>>;


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITSYSTEM_H_286926169253693490116142721512411129692808 
