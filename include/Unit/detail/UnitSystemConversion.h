/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    UnitSystemConversion.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.11.2020 
 */ 
 
#ifndef UNITSYSTEMCONVERSION_H_77531353103211610817575640425774315164220 
#define UNITSYSTEMCONVERSION_H_77531353103211610817575640425774315164220 
 
 
// includes
#include <Meta/RatioUtils.h>
#include <Unit/UnitSystem.h>


namespace unit {


/**
 * @struct RequiresSameDimension
 * @brief  RequiresSameDimension is a concept like SFINAE type to enforce
 *         same dimension of two units.
 */
template <typename UnitSystemT1, typename UnitSystemT2>
using RequiresSameDimension = std::enable_if_t<std::is_same_v<DimensionOf<UnitSystemT1>, DimensionOf<UnitSystemT2>>>;

/** 
 * @struct ConversionFactorT
 * @brief  ConversionFactorT computes the conversion factor between two units with same dimension but different units of measure.
 */
template <typename UnitSystemTo, typename UnitSystemFrom, typename EnforceSameDimension = void, typename RationalExponentsTag = void>
struct ConversionFactorT;

/** 
 * @struct ConversionFactorT
 * @brief  Version for dimension with integer exponents only.
 */
template <typename UnitSystemTo, typename UnitSystemFrom>
class ConversionFactorT<UnitSystemTo, UnitSystemFrom, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
                        std::enable_if_t<UnitSystemTo::dimension::isRational>> {

  template <typename Period, typename Exponent>
  struct PeriodToInteger : meta::RatioPowT<Period, Exponent::num> {};

  using fractionList           = meta::typelist::TransformBinary<PeriodListOf<UnitSystemFrom>, PeriodListOf<UnitSystemTo>, std::ratio_divide>;
  using fractionToExponentList = meta::typelist::TransformBinary<fractionList, dimension::ExponentsOf<DimensionOf<UnitSystemTo>>, PeriodToInteger>;
  using scaling                = std::ratio_divide<ScalingOf<UnitSystemFrom>, ScalingOf<UnitSystemTo>>;

public:
  using factor = std::ratio_multiply<scaling, meta::typelist::Accumulate<fractionToExponentList, std::ratio_multiply, std::ratio<1>>>;
};

/** 
 * @struct ConversionFactorT
 * @brief  Version for dimension with at least one rational exponent.
 */
template <typename UnitSystemTo, typename UnitSystemFrom>
class ConversionFactorT<UnitSystemTo, UnitSystemFrom, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
                        std::enable_if_t<!UnitSystemTo::dimension::isRational>> {

  template <typename Period, typename RationalExponent>
  struct PeriodToRationalExponentialT {
    using value_type                  = double;
    static constexpr value_type value = meta::RatioPower<Period, RationalExponent>;
  };

  using fractionList = meta::typelist::TransformBinary<PeriodListOf<UnitSystemFrom>, PeriodListOf<UnitSystemTo>, std::ratio_divide>;
  using scaling      = std::ratio_divide<ScalingOf<UnitSystemFrom>, ScalingOf<UnitSystemTo>>;

  static constexpr auto fractionToExponentList =
    meta::typelist::TransformTypeListBinary<fractionList,
                                            dimension::ExponentsOf<DimensionOf<UnitSystemTo>>,
                                            PeriodToRationalExponentialT>();

public:


};



}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITSYSTEMCONVERSION_H_77531353103211610817575640425774315164220 
