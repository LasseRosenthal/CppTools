/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    RatioUtils.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.11.2020 
 */ 
 
#ifndef RATIOUTILS_H_58411862223143257441033059201473228283456 
#define RATIOUTILS_H_58411862223143257441033059201473228283456 
 
 
// includes
#include <Meta/CompileTimeArithmetic.h>

#include <ratio>
#include <type_traits>


namespace meta {


/** 
 * @struct IsIntegralRatio
 * @brief  IsIntegralRatio checks if a given Ratio is integral, i.e. 
 *         a rational number, where the nominator and the denominator
 *         can be cancelled to leave an integer value.
 */
template <typename Ratio>
struct IsIntegralRatioT : std::bool_constant<Ratio::num % Ratio::den == 0> {};

template <typename Ratio>
constexpr bool IsIntegralRatio = IsIntegralRatioT<Ratio>::value;

/** 
 * @struct HasZeroNominator
 * @brief  HasZeroNominator checks if the nominator of a given ratio is zero.
 */
template <typename Ratio>
struct HasZeroNominatorT : std::bool_constant<Ratio::num == 0> {};

template <typename Ratio>
constexpr bool HasZeroNominator = HasZeroNominatorT<Ratio>::value;

/** 
 * @struct  RatioPowT
 * @brief   RatioPowT computes the value of a given ration to the power of a given integer exponent.
 * @remarks The result is reduced to lowest terms.
 */
template <typename Ratio, int Exponent, typename = void>
struct RatioPowT;

template <typename Ratio, int Exponent>
using RatioPow = typename RatioPowT<Ratio, Exponent>::type;

template <typename Ratio, int Exponent>
struct RatioPowT<Ratio, Exponent, std::enable_if_t<(Exponent >= 0)>>  {
  using type = std::ratio_multiply<Ratio, RatioPow<Ratio, Exponent - 1>>; 
};

template <typename Ratio, int Exponent>
struct RatioPowT<Ratio, Exponent, std::enable_if_t<(Exponent < 0)>>  {
  using type = std::ratio<RatioPow<Ratio, -Exponent>::den,
                          RatioPow<Ratio, -Exponent>::num>;
};

template <typename Ratio>
struct RatioPowT<Ratio, 0> {
  using type = std::ratio<1>;
};

template <typename Ratio, int Exponent>
using RatioPow = typename RatioPowT<Ratio, Exponent>::type;

/** 
 * @struct InvertT
 * @brief  InvertT computes the inversion of a given ratio.
 */
template <typename Ratio>
struct InvertT : RatioPowT<Ratio, -1> {};

template <typename Ratio>
using Invert = typename InvertT<Ratio>::type;

/** 
 * @struct NegativeT
 * @brief  NegativeT computes the negative value of a given ratio.
 */
template <typename Ratio>
struct NegativeT {
  using type = std::ratio<-Ratio::num, Ratio::den>;
};

template <typename Ratio>
using Negative = typename NegativeT<Ratio>::type;

/** 
 * @struct RatioPowerT
 * @brief  RatioPowerT computes the rational power of a given fraction.
 */
template <typename RatioBase, typename RatioExp>
struct RatioPowerT {
  using value_type = double;
  static constexpr value_type RatioBaseVal = static_cast<value_type>(RatioBase::num) / RatioBase::den;
  static constexpr value_type value = NthRoot<RatioExp::den>(Power<RatioExp::num>(RatioBaseVal));

//  static constexpr value_type value = NthRoot<RatioExp::den>(Power<RatioExp::num, value_type>(RatioBase::num)) / 
//                                      NthRoot<RatioExp::den>(Power<RatioExp::num, value_type>(RatioBase::den));
};

template <typename RatioBase, typename RatioExp>
constexpr auto RatioPower = RatioPowerT<RatioBase, RatioExp>::value;


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // RATIOUTILS_H_58411862223143257441033059201473228283456 
