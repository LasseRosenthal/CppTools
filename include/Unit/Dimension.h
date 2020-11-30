/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Dimension.h 
 * @brief   Definition of the Dimension structure which models
 *          a dimension as a composition of base units with 
 *          associated exponents.
 * 
 * @author  Lasse Rosenthal 
 * @date    17.11.2020 
 */ 
 
#ifndef DIMENSION_H_68871110916156110299726379270221051429837 
#define DIMENSION_H_68871110916156110299726379270221051429837 
 
 
// includes
#include <Meta/Typelist.h>
#include <Meta/Valuelist.h>
#include <Unit/BaseDimensions.h>
 

namespace unit {
namespace dimension {


/** 
 * @struct Dimension
 * @brief  Dimension defines a dimension as a composition
 *         of several base dimensions.
 * @tparam BaseDimTagList a \link #typelist \endlink of different
 *         \link #BaseDimensionTag \endlink structures.
 */
template <typename BaseDimTagList>
struct Dimension {
  using baseDimTags                = BaseDimTagList;
  using exponents                  = meta::typelist::Transform<baseDimTags, ExponentOfT>;
  static constexpr bool isRational = meta::typelist::AllOf<exponents, meta::IsIntegralRatioT>;
};

/** 
 * @struct ExponentsOfT
 * @brief  ExponentsOfT provides an alias for the exponent list of a dimension.
 */
template <typename DimensionT>
struct ExponentsOfT : meta::IdentityT<typename DimensionT::exponents> {};

template <typename DimensionT>
using ExponentsOf = typename ExponentsOfT<DimensionT>::type;

/**
 * @struct DimensionAt
 * @brief  DimensionAt provides an alias for the dimension at a certain position
 *         of a given list of base dimension tags. If the dimension tag list has no element
 *         with the given index, the alias is identical to the tag returned
 *         by \link #DimensionTagGeneratorT \endlink for the given index and a zero exponent.
 */
template <std::size_t Index, typename BaseDimTagList>
using DimensionAt = std::conditional_t<
    meta::valuelist::HasValue<meta::valuelist::TypeToValuelist<BaseDimTagList, IndexOfT>, Index>,
    meta::typelist::NthElement<BaseDimTagList, meta::typelist::FindIf<BaseDimTagList, HasIndex<Index>>>,
    DimensionTagGenerator<Index, std::ratio<0>>
  >;

/** 
 * @struct TagListGeneratorT
 * @brief  TagListGeneratorT bloats a given list of base dimension tags up to a list
 *         containing tags for all predefined base dimension tags.
 */
template <typename BaseDimTagList, typename>
struct BaseDimensionTagListGeneratorT;

template <typename BaseDimTagList, std::size_t... Is>
struct BaseDimensionTagListGeneratorT<BaseDimTagList, std::index_sequence<Is...>>
  : meta::IdentityT<meta::Typelist<DimensionAt<Is, BaseDimTagList>...>> {};

template <typename BaseDimTagList>
using BaseDimensionTagListGenerator =
  typename BaseDimensionTagListGeneratorT<BaseDimTagList, BaseDimensionIndices>::type;

/** 
 * @struct DimensionGeneratorT
 * @brief  DimensionGeneratorT generates a Dimension structure from a parameter pack
 *         of Base dimension tags.
 */
template <typename... BaseDimensionTags>
struct DimensionGeneratorT {

private:
  using SortedDimensionTags =
    meta::typelist::InsertionSort<meta::Typelist<BaseDimensionTags...>, DimensionTagLess>;

public:
  using type = Dimension<BaseDimensionTagListGenerator<SortedDimensionTags>>;
};

template <typename... BaseDimensionTags>
using DimensionGenerator = typename DimensionGeneratorT<BaseDimensionTags...>::type;

/** 
 * @struct DimensionTagListGeneratorT
 * @brief  DimensionTagListGeneratorT creates a list of BaseDimensionTags from a list of exponents
 */
template <typename Exponents, typename>
struct DimensionTagListGeneratorT;

template <typename Exponents, std::size_t... Is>
struct DimensionTagListGeneratorT<Exponents, std::index_sequence<Is...>>
  : meta::IdentityT<
      meta::Typelist<DimensionTagGenerator<Is, meta::typelist::NthElement<Exponents, Is>>...>> {};

template <typename Exponents>
using DimensionTagListGenerator =
  typename DimensionTagListGeneratorT<Exponents, BaseDimensionIndices>::type;

/** 
 * @brief MultiplicationType is an alias for the dimension of the product of two dimensions.
 */
template <typename Dimension1, typename Dimension2>
using MultiplicationType =
  Dimension<DimensionTagListGenerator<meta::typelist::TransformBinary<
    ExponentsOf<Dimension1>, ExponentsOf<Dimension2>, std::ratio_add>>
  >;

/** 
 * @brief DivisionType is an alias for the dimension of the quotient of two dimensions.
 */
template <typename Dimension1, typename Dimension2>
using DivisionType =
  Dimension<DimensionTagListGenerator<meta::typelist::TransformBinary<
    ExponentsOf<Dimension1>, ExponentsOf<Dimension2>, std::ratio_subtract>>
  >;

/** 
 * @brief DivisionType is an alias for the dimension of the quotient of two dimensions.
 */
template <typename DimensionT>
struct InversionTypeT : meta::IdentityT<Dimension<meta::typelist::Transform<typename DimensionT::baseDimTags, InvertBaseTagT>>> {};

template <typename DimensionT>
using InversionType = typename InversionTypeT<DimensionT>::type;


// ---------------------------------------------------
// convenient alias templates for common dimensions
using Time            = DimensionGenerator<TimeTag<std::ratio<1>>>;
using Length          = DimensionGenerator<LengthTag<std::ratio<1>>>;
using Mass            = DimensionGenerator<MassTag<std::ratio<1>>>;
using ElectricCurrent = DimensionGenerator<CurrentTag<std::ratio<1>>>;
using Temperature     = DimensionGenerator<TemperatureTag<std::ratio<1>>>;
using SubstanceAmount = DimensionGenerator<SubstanceTag<std::ratio<1>>>;
using Luminousity     = DimensionGenerator<LuminosityTag<std::ratio<1>>>;
using StorageAmount   = DimensionGenerator<StorageAmountTag<std::ratio<1>>>;
using Pixel           = DimensionGenerator<PixelTag<std::ratio<1>>>;
using Velocity        = DimensionGenerator<TimeTag<std::ratio<-1>>, LengthTag<std::ratio<1>>>;
using Acceleration    = DimensionGenerator<TimeTag<std::ratio<-2>>, LengthTag<std::ratio<1>>>;
using Force           = DimensionGenerator<TimeTag<std::ratio<-2>>, LengthTag<std::ratio<1>>, MassTag<std::ratio<1>>>;
using Energy          = DimensionGenerator<TimeTag<std::ratio<-2>>, LengthTag<std::ratio<2>>, MassTag<std::ratio<1>>>;
using Power           = DimensionGenerator<TimeTag<std::ratio<-3>>, LengthTag<std::ratio<2>>, MassTag<std::ratio<1>>>;
using Frequency       = DimensionGenerator<TimeTag<std::ratio<-1>>>;
using Resolution      = DimensionGenerator<LengthTag<std::ratio<-1>>, PixelTag<std::ratio<1>>>;


}   // namespace dimension
}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DIMENSION_H_68871110916156110299726379270221051429837 
