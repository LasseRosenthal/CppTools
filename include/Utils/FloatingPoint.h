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
  using intType = std::int32_t;
  static constexpr std::size_t mantissaBits = 23ULL;
  static constexpr std::size_t exponentBits = 8ULL;
  static constexpr std::size_t signBits     = 1ULL;
  static constexpr intType     bias         = 127;
  static constexpr intType     maxExponent  = 255;
};

template <>
struct IEEE754Specification<double> {
  using intType = std::int64_t;
  static constexpr std::size_t mantissaBits = 52ULL;
  static constexpr std::size_t exponentBits = 11ULL;
  static constexpr std::size_t signBits     = 1ULL;
  static constexpr intType     bias         = 1023;
  static constexpr intType     maxExponent  = 2047;
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

  // ---------------------------------------------------
  // methods
  [[nodiscard]] constexpr operator value_type () const noexcept;
  [[nodiscard]] constexpr auto mantissa       () const noexcept -> intType;
  [[nodiscard]] constexpr auto exponent       () const noexcept -> intType;
  [[nodiscard]] constexpr auto signBit        () const noexcept -> intType;
  [[nodiscard]] constexpr auto isNegative     () const noexcept -> bool;
  [[nodiscard]] constexpr auto isNan          () const noexcept -> bool;
  [[nodiscard]] constexpr auto isInfinity     () const noexcept -> bool;

private:

  // ---------------------------------------------------
  // store the components as a union of bit fields
  union {
    value_type value;
    intType bits;
    BitField<IEEESpec::mantissaBits, 0ULL> mantissa_;
    BitField<IEEESpec::exponentBits, IEEESpec::mantissaBits> exponent_;
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
 * @brief Returns the value of the exponent as an integer value.
 */
template <typename FloatT>
[[nodiscard]] constexpr auto FloatingPoint<FloatT>::exponent() const noexcept -> intType
{
  return static_cast<intType>(exponent_);
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
  return static_cast<intType>(exponent_) == IEEESpec::maxExponent && mantissa() != 0;
}

/**
 * @brief Checks the given floating point number represents inifinity.
 */
template <typename FloatT>
inline constexpr auto FloatingPoint<FloatT>::isInfinity() const noexcept -> bool
{
  return static_cast<intType>(exponent_) == IEEESpec::maxExponent && mantissa() == 0;
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FLOATINGPOINT_H_2071327478314233146020486281318879262553033 
