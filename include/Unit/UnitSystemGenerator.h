/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    UnitSystemGenerator.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.11.2020 
 */ 
 
#ifndef UNITSYSTEMGENERATOR_H_6476190729126361129484958965248965452 
#define UNITSYSTEMGENERATOR_H_6476190729126361129484958965248965452 
 
 
// includes
#include <Unit/UnitSystem.h>
 

namespace unit {


/**
 * @struct UnitSystemBaseTagLess
 * @brief  provides a compile-time weak ordering of \link #UnitSystemBaseTag \endlink
 *         with respect to their indices.
 */
template <typename UnitSystemBaseTagT1, typename UnitSystemBaseTagT2>
struct UnitSystemBaseTagLess : dimension::DimensionTagLess<BaseDimTagOf<UnitSystemBaseTagT1>, BaseDimTagOf<UnitSystemBaseTagT2>> {};

/** 
 * @struct HasIndexT
 * @brief  HasIndexT defines a nested template metafunction
 *         that checks if the nested \link #BaseDimensionTag \endlink of a given
 *         \link #UnitSystemBaseTag \endlink has a specific index.
 */
template <std::size_t Index>
struct HasIndexT {
  template <typename UnitSystemBaseTagT>
  struct nestedTemplate : std::conditional_t<BaseDimIndexOf<UnitSystemBaseTagT> == Index, std::true_type, std::false_type> {};
};

template <std::size_t Index>
using HasIndex = HasIndexT<Index>::template nestedTemplate;

/**
 * @struct UnitSystemBaseTagAt
 * @brief  UnitSystemBaseTagAt provides an alias for the UnitSystemBaseTag at a certain position
 *         of a given list of UnitSystemBaseTag. If no element with the given index is present,
 *         the alias is identical to the UnitSystemBaseTag nesting a BaseDimensionTag
 *         with the given index and an exponent of zero.
 */
template <std::size_t Index, typename BaseDimTagIndicesList, typename UnitSystemBaseTagList>
using UnitSystemBaseTagAt = std::conditional_t<
    meta::valuelist::HasValue<BaseDimTagIndicesList, Index>,
    meta::typelist::NthElement<UnitSystemBaseTagList, meta::typelist::FindIf<UnitSystemBaseTagList, HasIndex<Index>>>,
    UnitSystemBaseTag<dimension::DimensionTagGenerator<Index, std::ratio<0>>, std::ratio<1>>
  >;

/** 
 * @struct BaseDimensionTagListGeneratorT
 * @brief  BaseDimensionTagListGeneratorT bloats a given list of UnitSystemBaseTag up to a list
 *         containing UnitSystemBaseTags for all predefined base dimension tags.
 */
template <typename BaseDimTagIndicesList, typename UnitSystemBaseTagList, typename>
struct BaseDimensionTagListGeneratorT;

template <typename BaseDimTagIndicesList, typename UnitSystemBaseTagList, std::size_t... Is>
struct BaseDimensionTagListGeneratorT<BaseDimTagIndicesList, UnitSystemBaseTagList, std::index_sequence<Is...>> 
  : meta::IdentityT<meta::Typelist<UnitSystemBaseTagAt<Is, BaseDimTagIndicesList, UnitSystemBaseTagList>...>> {};

template <typename BaseDimTagIndicesList, typename UnitSystemBaseTagList>
using BaseDimensionTagListGenerator = typename BaseDimensionTagListGeneratorT<BaseDimTagIndicesList, UnitSystemBaseTagList, dimension::BaseDimensionIndices>::type;


/**
 * @class UnitSystemGeneratorT
 * @brief 
 */
template <typename UnitSystemBaseTagList, typename ScalingFactor = std::ratio<1>>
struct UnitSystemGeneratorT {

private:

  using unitSystemBaseTagsProvided = meta::typelist::InsertionSort<UnitSystemBaseTagList, UnitSystemBaseTagLess>;
  using baseDimTagIndices          = meta::valuelist::TypeToValuelist<unitSystemBaseTagsProvided, BaseDimIndexOfT>;
  using unitSystemBaseTags         = BaseDimensionTagListGenerator<baseDimTagIndices, unitSystemBaseTagsProvided>;

public:
  using type                       = UnitSystem<unitSystemBaseTags, ScalingFactor>;
};

template <typename UnitSystemBaseTagList, typename ScalingFactor = std::ratio<1>>
using UnitSystemGenerator = typename UnitSystemGeneratorT<UnitSystemBaseTagList, ScalingFactor>::type;


/** 
 * @brief Generators for common unit systems.
 */
template <typename Period, typename ScalingFactor = std::ratio<1>>
using TimeUnitGenerator =
  UnitSystemGenerator<meta::Typelist<UnitSystemBaseTag<dimension::TimeTag<std::ratio<1>>, Period>>,
                      ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using LengthUnitGenerator = UnitSystemGenerator<
  meta::Typelist<UnitSystemBaseTag<dimension::LengthTag<std::ratio<1>>, Period>>, ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using MassUnitGenerator =
  UnitSystemGenerator<meta::Typelist<UnitSystemBaseTag<dimension::MassTag<std::ratio<1>>, Period>>,
                      ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using FrequencyUnitGenerator =
  UnitSystemGenerator<meta::Typelist<UnitSystemBaseTag<dimension::TimeTag<std::ratio<-1>>, Period>>,
                      ScalingFactor>;

template <typename TimePeriod, typename LengthPeriod, typename ScalingFactor = std::ratio<1>>
using VelocityUnitGenerator = UnitSystemGenerator<
  meta::Typelist<UnitSystemBaseTag<dimension::TimeTag<std::ratio<-1>>, TimePeriod>,
                 UnitSystemBaseTag<dimension::LengthTag<std::ratio<1>>, LengthPeriod>>,
  ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using StorageAmountUnitGenerator = UnitSystemGenerator<
  meta::Typelist<UnitSystemBaseTag<dimension::StorageAmountTag<std::ratio<1>>, Period>>,
  ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using AreaUnitGenerator = UnitSystemGenerator<
  meta::Typelist<UnitSystemBaseTag<dimension::LengthTag<std::ratio<2>>, Period>>, ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using PixelsUnitGenerator =
  UnitSystemGenerator<meta::Typelist<UnitSystemBaseTag<dimension::PixelTag<std::ratio<1>>, Period>>,
                      ScalingFactor>;

template <typename Period, typename ScalingFactor = std::ratio<1>>
using AngleUnitGenerator =
  UnitSystemGenerator<meta::Typelist<UnitSystemBaseTag<dimension::AngleTag<std::ratio<1>>, Period>>,
                      ScalingFactor>;

template <typename PixelPeriod, typename LengthPeriod, typename ScalingFactor = std::ratio<1>>
using ResolutionUnitGenerator = UnitSystemGenerator<
  meta::Typelist<UnitSystemBaseTag<dimension::PixelTag<std::ratio<1>>, PixelPeriod>,
                 UnitSystemBaseTag<dimension::LengthTag<std::ratio<-1>>, LengthPeriod>>,
  ScalingFactor>;


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITSYSTEMGENERATOR_H_6476190729126361129484958965248965452 
