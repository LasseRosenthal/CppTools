/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FloatingPoint.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    23.12.2020 
 */ 
 
#ifndef FLOATINGPOINT_H_2071327478314233146020486281318879262553033 
#define FLOATINGPOINT_H_2071327478314233146020486281318879262553033 
 
 
// includes
#include <Utils/BitField.h>

#include <ostream>
 

namespace cpptools {


/** 
 * @struct IEEE754Specification
 * @brief  IEEE754Specification defines constants according to the IEEE specifications
 *         for single (float) and double precision (double).
 * @see    https://en.wikipedia.org/wiki/IEEE_754
 */
template <typename FloatT>
struct IEEE754Specification;

template <>
struct IEEE754Specification<float> {
  using intType                                  = std::int32_t;
  static constexpr std::size_t mantissaBits      = 23ULL;
  static constexpr std::size_t exponentBits      = 8ULL;
  static constexpr std::size_t signBits          = 1ULL;
  static constexpr intType     bias              = 127;
  static constexpr intType     maxCharacteristic = 255;
  static constexpr intType     maxExponent       = 127;
  static constexpr intType     maxMantissa       = (1 << mantissaBits) - 1;
};

template <>
struct IEEE754Specification<double> {
  using intType                                  = std::int64_t;
  static constexpr std::size_t mantissaBits      = 52ULL;
  static constexpr std::size_t exponentBits      = 11ULL;
  static constexpr std::size_t signBits          = 1ULL;
  static constexpr intType     bias              = 1023LL;
  static constexpr intType     maxCharacteristic = 2047LL;
  static constexpr intType     maxExponent       = 1023LL;
  static constexpr intType     maxMantissa       = (1LL << mantissaBits) - 1LL;
};


/** 
 * @class FloatingPoint
 * @brief FloatingPoint is a class that allows for the exploration of the
 *        different components of floating point numbers.
 */
template <typename FloatT>
class FloatingPoint {

public:

  // ---------------------------------------------------
  // public types
  using IEEESpec   = IEEE754Specification<FloatT>;
  using value_type = FloatT;
  using intType    = typename IEEESpec::intType;

  // ---------------------------------------------------
  // construction
  constexpr FloatingPoint (value_type v = 0.0) noexcept;
  constexpr FloatingPoint (intType v) noexcept;
  constexpr FloatingPoint (intType m, intType c, intType s) noexcept;

  // ---------------------------------------------------
  // methods
  [[nodiscard]] constexpr operator value_type () const noexcept;
  [[nodiscard]] constexpr auto mantissa       () const noexcept -> intType;
  [[nodiscard]] constexpr auto exponent       () const noexcept -> intType;
  [[nodiscard]] constexpr auto characteristic () const noexcept -> intType;
  [[nodiscard]] constexpr auto signBit        () const noexcept -> intType;
  [[nodiscard]] constexpr auto isNegative     () const noexcept -> bool;
  [[nodiscard]] constexpr auto isNan          () const noexcept -> bool;
  [[nodiscard]] constexpr auto isInfinity     () const noexcept -> bool;
  [[nodiscard]] constexpr auto isNormal       () const noexcept -> bool;
  [[nodiscard]] constexpr auto isSubNormal    () const noexcept -> bool;

  template <typename CharT, typename CharTraitsT = std::char_traits<CharT>>
  auto put(std::basic_ostream<CharT, CharTraitsT>& ostr) const -> std::basic_ostream<CharT, CharTraitsT>&;

  static auto maxVal() noexcept -> FloatingPoint;
  static auto minVal() noexcept -> FloatingPoint;


private:

  // ---------------------------------------------------
  // store the components as a union of bit fields
  union {
    value_type value;
    intType bits;
    BitField<IEEESpec::mantissaBits, 0ULL> mantissa_;
    BitField<IEEESpec::exponentBits, IEEESpec::mantissaBits> characteristic_;
    BitField<IEEESpec::signBits, IEEESpec::mantissaBits + IEEESpec::exponentBits> signBit_;
  };
};


/**
 * @brief Constructor. 
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(value_type v) noexcept
  : value(v)
{}

/**
 * @brief Constructor. 
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(intType v) noexcept
  : bits(v)
{}

/**
 * @brief Constructor.
 * @param m the mantissa.
 * @param e the characteristic.
 * @param s the sign bit.
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(intType m, intType c, intType s) noexcept
  : mantissa_{m}
{
  characteristic_ = c;
  signBit_        = s;
}

/**
 * @brief Returns the floating point value.
 */
template <typename FloatT>
[[nodiscard]] constexpr FloatingPoint<FloatT>::operator value_type() const noexcept
{
  return value;
}

/**
 * @brief Returns the value of the mantissa as an integer value.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::mantissa() const noexcept -> intType
{
  return static_cast<intType>(mantissa_);
}

/**
 * @brief Returns the value of the exponent, i.e. the biased characterictisc as an integer value.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::exponent() const noexcept -> intType
{
  return isNormal() ? characteristic() - IEEESpec::bias : characteristic() - IEEESpec::bias + 1;
}

/**
 * @brief Returns the value of the characterictisc as an integer value.
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::characteristic() const noexcept -> intType
{
  return static_cast<intType>(characteristic_);
}

/**
 * @brief Returns the value of the sign bit as an integer value.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::signBit() const noexcept -> intType
{
  return static_cast<intType>(signBit_);
}

/**
 * @brief Checks the given floating point number is negative
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::isNegative() const noexcept -> bool
{
  return signBit() == 1;
}

/**
 * @brief Checks the given floating point number is NAN (not a number)
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::isNan() const noexcept -> bool
{
  return static_cast<intType>(characteristic_) == IEEESpec::maxCharacteristic && mantissa() != 0;
}

/**
 * @brief Checks the given floating point number represents inifinity.
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::isInfinity() const noexcept -> bool
{
  return static_cast<intType>(characteristic_) == IEEESpec::maxCharacteristic && mantissa() == 0;
}

/**
 * @brief Checks if the given number is normal, i.e the characteristic is larger than zero.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::isNormal() const noexcept -> bool
{
  return characteristic() > 0 && characteristic() != IEEESpec::maxCharacteristic;
}

/**
 * @brief Checks if the given number is subNormal, i.e the characteristic is equal to zero.
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::isSubNormal() const noexcept -> bool
{
  return characteristic() == 0;
}

/**
 * @brief Returns the largest representable floating point number.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::maxVal() noexcept -> FloatingPoint
{
  return FloatingPoint(IEEESpec::maxMantissa, IEEESpec::maxCharacteristic - 1, 0);
}

/**
 * @brief Returns the smallest representable floating point number.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::minVal() noexcept -> FloatingPoint
{
  return FloatingPoint(IEEESpec::maxMantissa, IEEESpec::maxCharacteristic - 1, 1);
}

template <typename FloatT>
template <typename CharT, typename CharTraitsT>
inline auto FloatingPoint<FloatT>::put(std::basic_ostream<CharT, CharTraitsT>& ostr) const -> std::basic_ostream<CharT, CharTraitsT>&
{
  return ostr << signBit_ << ostr.widen(' ') << characteristic_ << ostr.widen(' ') << mantissa_;
}

/**
 * @brief Stream operator.
 */
template <typename FloatT, typename CharT, typename CharTraitsT>
auto operator<<(std::basic_ostream<CharT, CharTraitsT>& ostr, FloatingPoint<FloatT> const& f) -> std::basic_ostream<CharT, CharTraitsT>&
{
  return f.put(ostr);
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FLOATINGPOINT_H_2071327478314233146020486281318879262553033 
