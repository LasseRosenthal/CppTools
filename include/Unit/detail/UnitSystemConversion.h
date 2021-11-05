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
template <typename UnitSystemTo, typename UnitSystemFrom, typename T,
          typename EnforceSameDimension = void, typename RationalExponentsTag = void>
struct ConversionFactorT;

/** 
 * @struct ConversionFactorT
 * @brief  Version for dimension with integer exponents only.
 */
template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
class ConversionFactorT<UnitSystemTo, UnitSystemFrom, T, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
                        std::enable_if_t<UnitSystemTo::dimension::isRational>> {

  template <typename Period, typename Exponent>
  struct PeriodToInteger : meta::RatioPowT<Period, Exponent::num> {};

  using fractionList           = meta::typelist::TransformBinary<PeriodListOf<UnitSystemFrom>, PeriodListOf<UnitSystemTo>, std::ratio_divide>;
  using fractionToExponentList = meta::typelist::TransformBinary<fractionList, dimension::ExponentsOf<DimensionOf<UnitSystemTo>>, PeriodToInteger>;
  using scaling                = std::ratio_divide<ScalingOf<UnitSystemFrom>, ScalingOf<UnitSystemTo>>;

public:
  using factor = std::ratio_multiply<scaling, meta::typelist::Accumulate<fractionToExponentList, std::ratio_multiply, std::ratio<1>>>;

  static constexpr T value = meta::AsDecimal<factor, T>;
};

/** 
 * @struct ConversionFactorT
 * @brief  Version for dimension with at least one rational exponent.
 */
template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
class ConversionFactorT<UnitSystemTo, UnitSystemFrom, T, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
                        std::enable_if_t<!UnitSystemTo::dimension::isRational>> {

  template <typename Period, typename RationalExponent>
  struct PeriodToRationalExponentialT {
    using value_type                  = double;
    static constexpr value_type value = meta::RatioPower<Period, RationalExponent>;
  };

  using fractionList = meta::typelist::TransformBinary<PeriodListOf<UnitSystemFrom>, PeriodListOf<UnitSystemTo>, std::ratio_divide>;
  using scaling      = std::ratio_divide<ScalingOf<UnitSystemFrom>, ScalingOf<UnitSystemTo>>;

  static constexpr auto fractionToExponentList =
    meta::typelist::TransformInvoke<fractionList, dimension::ExponentsOf<DimensionOf<UnitSystemTo>>,
                                    PeriodToRationalExponentialT>();

public:

  static constexpr T value = (static_cast<T>(scaling::num) * meta::accumulate(fractionToExponentList, [](auto& a, auto b) { a *= b; }, 1.0)) / scaling::denum;
};

template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
constexpr auto ConversionFactor = ConversionFactorT<UnitSystemTo, UnitSystemFrom, T>::value;


/** 
 * @struct ShiftT
 * @brief  ShiftT computes the shift between two units with same dimension but different units of measure.
 */
template <typename UnitSystemTo, typename UnitSystemFrom, typename T,
          typename EnforceSameDimension = void, typename = void>
struct ShiftT;

template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
struct ShiftT<UnitSystemTo, UnitSystemFrom, T, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
              std::enable_if_t<!IsShifted<UnitSystemTo> && IsShifted<UnitSystemFrom>>
             > {
  static constexpr auto value = -ShiftValue<UnitSystemFrom, T> * ConversionFactor<UnitSystemTo, UnitSystemFrom, T>;
};

template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
struct ShiftT<UnitSystemTo, UnitSystemFrom, T, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
              std::enable_if_t<IsShifted<UnitSystemTo> && !IsShifted<UnitSystemFrom>>
             > {
  static constexpr auto value = ShiftValue<UnitSystemTo, T>;
};

template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
struct ShiftT<UnitSystemTo, UnitSystemFrom, T, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
              std::enable_if_t<IsShifted<UnitSystemTo> && IsShifted<UnitSystemFrom>>
             > {
  static constexpr double value = ShiftValue<UnitSystemTo, T> - ShiftValue<UnitSystemFrom, T> * ConversionFactor<UnitSystemTo, UnitSystemFrom, T>;
};

template <typename UnitSystemTo, typename UnitSystemFrom, typename T>
struct ShiftT<UnitSystemTo, UnitSystemFrom, T, RequiresSameDimension<UnitSystemTo, UnitSystemFrom>,
              std::enable_if_t<!IsShifted<UnitSystemTo> && !IsShifted<UnitSystemFrom>>
             > {
  static constexpr T value = 0.0;
};

template <typename UnitSystemTo, typename UnitSystemFrom, typename T = double>
constexpr auto Shift = ShiftT<UnitSystemTo, UnitSystemFrom, T>::value;


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITSYSTEMCONVERSION_H_77531353103211610817575640425774315164220 
