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
#include <Meta/Utility.h>

#include <ratio>
#include <type_traits>


namespace meta {


/** 
 * @struct AsDecimalT
 * @brief  AsDecimalT converts a ratio into a decimal representation. 
 */
template <typename Ratio, typename T = double>
struct AsDecimalT {
  static constexpr T value = static_cast<T>(Ratio::num) / static_cast<T>(Ratio::den);
};

template <typename Ratio, typename T = double>
constexpr auto AsDecimal = AsDecimalT<Ratio, T>::value;

/** 
 * @struct NegativeT
 * @brief  NegativeT computes the negative value of a given ratio.
 */
template <typename Ratio, typename = void>
struct NegativeT;

template <typename Ratio>
struct NegativeT<Ratio, std::enable_if_t<Ratio::num == 0>> : meta::IdentityT<Ratio> {};

template <typename Ratio>
struct NegativeT<Ratio, std::enable_if_t<Ratio::num != 0>> : meta::IdentityT<std::ratio<-Ratio::num, Ratio::den>> {};

template <typename Ratio>
using Negative = typename NegativeT<Ratio>::type;

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
struct RatioPowT<Ratio, Exponent, std::enable_if_t<(Exponent >= 0)>> 
  : IdentityT<std::ratio_multiply<Ratio, RatioPow<Ratio, Exponent - 1>>> {};

template <typename Ratio, int Exponent>
struct RatioPowT<Ratio, Exponent, std::enable_if_t<(Exponent < 0)>>
  : IdentityT<std::ratio<RatioPow<Ratio, -Exponent>::den, RatioPow<Ratio, -Exponent>::num>> {};

template <typename Ratio>
struct RatioPowT<Ratio, 0> : IdentityT<std::ratio<1>> {};

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
 * @struct RatioPowerT
 * @brief  RatioPowerT computes the rational power of a given fraction.
 */
template <typename RatioBase, typename RatioExp, typename T = double>
struct RatioPowerT {
  using value_type = T;
  static constexpr value_type value = NthRoot<RatioExp::den>(Power<RatioExp::num>(AsDecimal<RatioBase, value_type>));
};

template <typename RatioBase, typename RatioExp, typename T = double>
constexpr auto RatioPower = RatioPowerT<RatioBase, RatioExp, T>::value;


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // RATIOUTILS_H_58411862223143257441033059201473228283456 
