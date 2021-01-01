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

#include <cmath>
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
  using intType                                   = std::int32_t;
  static constexpr std::size_t size               = 32ULL;
  static constexpr std::size_t mantissaBits       = 23ULL;
  static constexpr std::size_t exponentBits       = 8ULL;
  static constexpr std::size_t signBits           = 1ULL;
  static constexpr intType     bias               = 127;
  static constexpr intType     maxCharacteristic  = 255;
  static constexpr intType     maxExponent        = 127;
  static constexpr intType     maxMantissa        = (1 << mantissaBits) - 1;
  static constexpr intType     maxValInt          = 0x7F7FFFFF;
  static constexpr intType     minValNormalInt    = 0x00800000;
  static constexpr intType     minValSubNormalInt = 0x00000001;
  static constexpr intType     lowestValInt       = 0xFF7FFFFF;
  static constexpr intType     MSBValue           = 0x80000000;
};

template <>
struct IEEE754Specification<double> {
  using intType                                   = std::int64_t;
  static constexpr std::size_t size               = 64ULL;
  static constexpr std::size_t mantissaBits       = 52ULL;
  static constexpr std::size_t exponentBits       = 11ULL;
  static constexpr std::size_t signBits           = 1ULL;
  static constexpr intType     bias               = 1023LL;
  static constexpr intType     maxCharacteristic  = 2047LL;
  static constexpr intType     maxExponent        = 1023LL;
  static constexpr intType     maxMantissa        = (1LL << mantissaBits) - 1LL;
  static constexpr intType     maxValInt          = 0x7FEFFFFFFFFFFFFFLL;
  static constexpr intType     minValNormalInt    = 0x0010000000000000LL;
  static constexpr intType     minValSubNormalInt = 0x0000000000000001LL;
  static constexpr intType     lowestValInt       = 0xFFEFFFFFFFFFFFFFLL;
  static constexpr intType     MSBValue           = 0x8000000000000000LL;
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
  constexpr FloatingPoint (FloatingPoint const& src) noexcept;
  constexpr FloatingPoint (FloatingPoint&& src) noexcept;
  auto operator=          (FloatingPoint const& src) noexcept -> FloatingPoint&;
  auto operator=          (FloatingPoint&& src) noexcept -> FloatingPoint&;

  // ---------------------------------------------------
  // methods
  [[nodiscard]] constexpr operator value_type         () const noexcept;
  [[nodiscard]] constexpr operator intType            () const noexcept;
  [[nodiscard]] constexpr auto mantissa               () const noexcept -> intType;
  [[nodiscard]] constexpr auto exponent               () const noexcept -> intType;
  [[nodiscard]] constexpr auto characteristic         () const noexcept -> intType;
  [[nodiscard]] constexpr auto signBit                () const noexcept -> intType;
  [[nodiscard]] constexpr auto isPositive             () const noexcept -> bool;
  [[nodiscard]] constexpr auto isNegative             () const noexcept -> bool;
  [[nodiscard]] constexpr auto isZero                 () const noexcept -> bool;
  [[nodiscard]] constexpr auto isFiniteNumber         () const noexcept -> bool;
  [[nodiscard]] constexpr auto isNan                  () const noexcept -> bool;
  [[nodiscard]] constexpr auto isInfinity             () const noexcept -> bool;
  [[nodiscard]] constexpr auto isNormal               () const noexcept -> bool;
  [[nodiscard]] constexpr auto isSubNormal            () const noexcept -> bool;
  [[nodiscard]] constexpr auto next                   () const noexcept -> FloatingPoint;
  [[nodiscard]] constexpr auto previous               () const noexcept -> FloatingPoint;

  [[nodiscard]] static auto distanceInULP (FloatingPoint const& f1, FloatingPoint const& f2) noexcept -> std::size_t;
  auto epsilon () const noexcept -> FloatingPoint;

  [[nodiscard]] static constexpr auto maxVal          () noexcept -> FloatingPoint;
  [[nodiscard]] static constexpr auto minValNormal    () noexcept -> FloatingPoint;
  [[nodiscard]] static constexpr auto minValSubNormal () noexcept -> FloatingPoint;
  [[nodiscard]] static constexpr auto lowest          () noexcept -> FloatingPoint;
  [[nodiscard]] static constexpr auto infinity        () noexcept -> FloatingPoint;


  auto operator++ () noexcept -> FloatingPoint&;
  auto operator++ (int) noexcept -> FloatingPoint;


  template <typename CharT, typename CharTraitsT = std::char_traits<CharT>>
  auto put(std::basic_ostream<CharT, CharTraitsT>& ostr) const -> std::basic_ostream<CharT, CharTraitsT>&;

private:

  // ---------------------------------------------------
  // store the components as a union of bit fields
  union {
    value_type value;
    intType bits;
    BitField<IEEESpec::mantissaBits, 0ULL> mantissa_;
    BitField<IEEESpec::exponentBits, IEEESpec::mantissaBits> characteristic_;
    BitField<IEEESpec::signBits, IEEESpec::size - 1ULL> signBit_;
  };

  auto distanceToZeroInULP    () const noexcept -> intType;
  static constexpr auto distanceToZeroToIntRep (intType) noexcept -> intType;
};


/**
 * @brief Constructor.
 * @param d the double value represented by the FloatingPoint.
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(value_type d) noexcept
  : value(d)
{}

/**
 * @brief Constructor.
 * @param i the integer value.
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(intType i) noexcept
  : bits(i)
{}

/**
 * @brief Constructor.
 * @param m the mantissa.
 * @param e the characteristic.
 * @param s the sign bit.
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(intType m, intType c, intType s) noexcept
  : bits{s << (IEEESpec::size - 1ULL) | c << IEEESpec::mantissaBits | m}
{}

/**
 * @brief Copy Constructor.
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(FloatingPoint const& src) noexcept
  : value{src.value}
{}

/**
 * @brief Move Constructor.
 */
template <typename FloatT>
constexpr FloatingPoint<FloatT>::FloatingPoint(FloatingPoint&& src) noexcept
  : value{std::move(src.value)}
{}

/**
 * @brief Copy assignment.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::operator=(FloatingPoint const& src) noexcept -> FloatingPoint&
{
  value = src.value;
  return *this;
}

/**
 * @brief Move assignment.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::operator=(FloatingPoint&& src) noexcept -> FloatingPoint&
{
  value = std::move(src.value);
  return *this;
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
 * @brief Returns the bits representation of the value interpreted as a signed integer.
 */
template <typename FloatT>
[[nodiscard]] constexpr FloatingPoint<FloatT>::operator intType() const noexcept
{
  return bits;
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
 * @brief Checks the given floating point number is positive.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::isPositive() const noexcept -> bool
{
  return bits >= 0;
//  return signBit() == 0;
}

/**
 * @brief Checks the given floating point number is negative.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::isNegative() const noexcept -> bool
{
  return bits < 0;
//  return signBit() == 1;
}

/**
 * @brief Checks the given floating point equals plus or minus zero.
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::isZero() const noexcept -> bool
{
  return mantissa() == 0 && characteristic() == 0;
}

/**
 * @brief Checks the given floating point number is a valid number, i.e. the characteristic
 *        is smaller than \link #IEEESpec::maxCharacteristic \endlink.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::isFiniteNumber() const noexcept -> bool
{
  return characteristic() != IEEESpec::maxCharacteristic;
}

/**
 * @brief Checks the given floating point number is NAN (not a number)
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::isNan() const noexcept -> bool
{
  return !isFiniteNumber() && mantissa() != 0;
}

/**
 * @brief Checks the given floating point number represents inifinity.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::isInfinity() const noexcept -> bool
{
  return !isFiniteNumber() && mantissa() == 0;
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
constexpr auto FloatingPoint<FloatT>::isSubNormal() const noexcept -> bool
{
  return characteristic() == 0;
}

/**
 * @brief Returns the next representable floating point number.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::next() const noexcept -> FloatingPoint
{
  return {distanceToZeroToIntRep(distanceToZeroInULP() + 1)};

  //return !isFiniteNumber() ? FloatingPoint{bits} :
  //        isPositive() ? FloatingPoint{bits + 1} :
  //        isZero() ? minValSubNormal() : FloatingPoint{bits - 1};
}

/**
 * @brief Returns the previous representable floating point number.
 */
template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::previous() const noexcept -> FloatingPoint
{
  return {distanceToZeroToIntRep(distanceToZeroInULP() - 1)};
}

/**
 * @brief Returns the distance between f1 and f2 in terms of ulps.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::distanceInULP(FloatingPoint const& f1, FloatingPoint const& f2) noexcept -> std::size_t
{
  if((f1.isPositive() && f2.isPositive()) || (f1.isNegative() && f2.isNegative()))
  {
    return static_cast<std::size_t>(std::abs(f1.distanceToZeroInULP() - f2.distanceToZeroInULP()));
  }

  return static_cast<std::size_t>(std::abs(f1.distanceToZeroInULP())) +
         static_cast<std::size_t>(std::abs(f2.distanceToZeroInULP()));
}

template <typename FloatT>
inline auto FloatingPoint<FloatT>::epsilon() const noexcept -> FloatingPoint
{
  return isNormal() ? FloatingPoint(0, 0, 0) : minValSubNormal();
}

/**
 * @brief Returns the largest representable floating point number.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::maxVal() noexcept -> FloatingPoint
{
  return {IEEESpec::maxValInt};
}

/**
 * @brief Returns the smallest normalized representable positive floating point number.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::minValNormal() noexcept -> FloatingPoint
{
  return {IEEESpec::minValNormalInt};
}

/**
 * @brief Returns the smallest denormalized representable positive floating point number.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::minValSubNormal() noexcept -> FloatingPoint
{
  return {IEEESpec::minValSubNormalInt};
}

/**
 * @brief Returns the lowest representable floating point number.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::lowest() noexcept -> FloatingPoint
{
  return {IEEESpec::lowestValInt};
}

/**
 * @brief Returns the floating point number that represents positive infinity.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::infinity() noexcept -> FloatingPoint
{
  return {0, IEEESpec::maxCharacteristic, 0};
}

/**
 * @brief Increments the given floating point number.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::operator++() noexcept -> FloatingPoint&
{
  bits = distanceToZeroToIntRep(distanceToZeroInULP() + 1);

  //if(isPositive() && isFiniteNumber())
  //{
  //  ++bits;
  //}
  //else
  //{
  //  if(isZero())
  //  {
  //    bits = IEEESpec::minValSubNormalInt;
  //  }
  //  else
  //  {
  //    --bits;
  //  }
  //}

  return *this;
}

template <typename FloatT>
inline auto FloatingPoint<FloatT>::operator++(int) noexcept -> FloatingPoint
{
  FloatingPoint tmp{*this};
  ++(*this);
  return tmp;
}

/**
 * @brief Returns the distance in terms of ulps of the given floating point number to zero.
 */
template <typename FloatT>
inline auto FloatingPoint<FloatT>::distanceToZeroInULP() const noexcept -> intType
{
  return bits < 0 ? IEEESpec::MSBValue - bits : bits;
//  return static_cast<std::size_t>(bits & (~(static_cast<intType>(1) << (IEEESpec::size - 1ULL))));
}

template <typename FloatT>
constexpr auto FloatingPoint<FloatT>::distanceToZeroToIntRep(intType i) noexcept -> intType
{
  return i >= 0 ? i : IEEESpec::MSBValue - i;
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
