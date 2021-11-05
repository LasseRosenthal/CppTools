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
using TimeTag = BaseDimensionTag<0ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using LengthTag = BaseDimensionTag<1ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using MassTag = BaseDimensionTag<2ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using CurrentTag = BaseDimensionTag<3ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using TemperatureTag = BaseDimensionTag<4ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using SubstanceTag = BaseDimensionTag<5ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using LuminosityTag = BaseDimensionTag<6ULL, Exponent>;

// ---------------------------------------------------
// tags for other quantities
template <typename Exponent = std::ratio<0>>
using StorageAmountTag = BaseDimensionTag<7ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using PixelTag = BaseDimensionTag<8ULL, Exponent>;

template <typename Exponent = std::ratio<0>>
using AngleTag = BaseDimensionTag<9ULL, Exponent>;


/** 
 * @brief  numBaseDimensions defines the number of base dimensions.
 * @remark this constant has to be adjusted if new Base Tags are added.
 */
constexpr std::size_t numBaseDimensions = 10ULL;

/** 
 * @brief BaseDimensionIndices is an index_sequence ranging from 0 to numBaseDimensions-1.
 */
using BaseDimensionIndices = std::make_index_sequence<numBaseDimensions>;


/**
 * @struct InvertBaseTagT
 * @brief  provides a type alias for a BaseDimensionTag that arises through multiplication
 *         of the exponent of a given BaseDimensionTag with minus one.
 */
template <typename BaseDimTag>
struct InvertBaseTagT : meta::IdentityT<BaseDimensionTag<BaseDimTag::index, meta::Negative<ExponentOf<BaseDimTag>>>>{};

template <typename BaseDimTag>
using InvertBaseTag = typename InvertBaseTagT<BaseDimTag>::type;



/**
 * @struct NthPowerOfBaseTagT
 * @brief  provides a type alias for a BaseDimensionTag that arises through taking
 *         the nthpower of the exponent of a given BaseDimensionTag.
 */
template <typename BaseDimTag, std::size_t N>
struct NthPowerOfBaseTagT : meta::IdentityT<BaseDimensionTag<BaseDimTag::index, std::ratio_multiply<ExponentOf<BaseDimTag>, std::ratio<N>>>>{};

template <typename BaseDimTag, std::size_t N>
using NthPowerOfBaseTag = typename NthPowerOfBaseTagT<BaseDimTag, N>::type;

/**
 * @struct NthPowerGeneratorT
 * @brief  NthPowerGeneratorT defines a nested template metafunction
 *         that computes the nth power of a given dimension
 */
template <std::size_t N>
struct NthPowerGeneratorT {
  template <typename BaseDimTag>
  using nestedTemplate = NthPowerOfBaseTagT<BaseDimTag, N>;
};

template <std::size_t N>
using NthPowerGenerator = NthPowerGeneratorT<N>::template nestedTemplate;

/**
 * @struct NthRootOfBaseTagT
 * @brief  provides a type alias for a BaseDimensionTag that arises through taking
 *         the nth-root of the exponent of a given BaseDimensionTag.
 */
template <typename BaseDimTag, std::size_t N>
struct NthRootOfBaseTagT : meta::IdentityT<BaseDimensionTag<BaseDimTag::index, std::ratio_multiply<ExponentOf<BaseDimTag>, std::ratio<1, N>>>>{};

template <typename BaseDimTag, std::size_t N>
using NthRootOfBaseTag = typename NthRootOfBaseTagT<BaseDimTag, N>::type;

/**
 * @struct NthRootGeneratorT
 * @brief  NthRootGeneratorT defines a nested template metafunction
 *         that computes the N-th root of a given dimension
 */
template <std::size_t N>
struct NthRootGeneratorT {
  template <typename BaseDimTag>
  using nestedTemplate = NthRootOfBaseTagT<BaseDimTag, N>;
};

template <std::size_t N>
using NthRootGenerator = NthRootGeneratorT<N>::template nestedTemplate;


}   // namespace dimension
}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BASEDIMENSIONS_H_918622400129167386558817557257782122424977 
