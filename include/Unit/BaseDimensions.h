/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BaseDimensions.h 
 * @brief   Definition of BaseDimensionTags for different units.
 * 
 * @author  Lasse Rosenthal 
 * @date    17.11.2020 
 */ 
 
#ifndef BASEDIMENSIONS_H_918622400129167386558817557257782122424977 
#define BASEDIMENSIONS_H_918622400129167386558817557257782122424977 
 
 
// includes
#include <Unit/BaseDimensionTag.h>


namespace unit {
namespace dimension {


// ---------------------------------------------------
// tags for the seven SI-base Units
// see https://en.wikipedia.org/wiki/International_System_of_Units
template <typename Exponent = std::ratio<0>>
struct TimeTag : BaseDimensionTag<0, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct LengthTag : BaseDimensionTag<1, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct MassTag : BaseDimensionTag<2, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct CurrentTag : BaseDimensionTag<3, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct TemperatureTag : BaseDimensionTag<4, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct SubstanceTag : BaseDimensionTag<5, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct LuminosityTag : BaseDimensionTag<6, Exponent> {};

// ---------------------------------------------------
// tags for other quantities
template <typename Exponent = std::ratio<0>>
struct StorageAmountTag : BaseDimensionTag<7, Exponent> {};

template <typename Exponent = std::ratio<0>>
struct PixelTag : BaseDimensionTag<8, Exponent> {};


/** 
 * @brief  numBaseDimensions defines the number of base dimensions.
 * @remark this constant has to be adjusted if new Base Tags are added.
 */
inline constexpr std::size_t numBaseDimensions = 9ULL;

/** 
 * @brief BaseDimensionIndices is an index_sequence ranging from 0 to numBaseDimensions-1.
 */
using BaseDimensionIndices = std::make_index_sequence<numBaseDimensions>;


/** 
 * @struct DimensionTagGeneratorT
 * @brief  DimensionTagGeneratorT is a metafunction that is used 
 *         to generate base dimension tags for a given index and
 *         a rational exponent.
 * @remark If new base tags are added, corresponding specializations for
 *         DimensionTagGeneratorT have to be provided.
 */
template <std::size_t Index, typename Exponent>
struct DimensionTagGeneratorT;

template <int Index, typename Exponent = std::ratio<0>>
using DimensionTagGenerator = typename DimensionTagGeneratorT<Index, Exponent>::type;

template <typename Exponent>
struct DimensionTagGeneratorT<0, Exponent> : meta::IdentityT<TimeTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<1, Exponent> : meta::IdentityT<LengthTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<2, Exponent> : meta::IdentityT<MassTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<3, Exponent> : meta::IdentityT<CurrentTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<4, Exponent> : meta::IdentityT<TemperatureTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<5, Exponent> : meta::IdentityT<SubstanceTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<6, Exponent> : meta::IdentityT<LuminosityTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<7, Exponent> : meta::IdentityT<StorageAmountTag<Exponent>> {};

template <typename Exponent>
struct DimensionTagGeneratorT<8, Exponent> : meta::IdentityT<PixelTag<Exponent>> {};


/**
 * @struct InvertBaseTagT
 * @brief  provides a type alias for a BaseDimensionTag that arises through multiplication
 *         of the exponent of a given BaseDimensionTag with minus one.
 */
template <typename BaseDimTag>
struct InvertBaseTagT : meta::IdentityT<DimensionTagGenerator<BaseDimTag::index, meta::Negative<ExponentOf<BaseDimTag>>>>{};

template <typename BaseDimTag>
using InvertBaseTag = typename InvertBaseTagT<BaseDimTag>::type;


}   // namespace dimension
}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BASEDIMENSIONS_H_918622400129167386558817557257782122424977 
