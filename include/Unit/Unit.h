/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Unit.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    26.11.2020 
 */ 
 
#ifndef UNIT_H_179732885328477293061676127622711153282600 
#define UNIT_H_179732885328477293061676127622711153282600 
 
 
// includes
#include <Unit/UnitSystemGenerator.h>
#include <Unit/detail/Arithmetic.h>
#include <Unit/detail/UnitSystemConversion.h>
#include <Utils/FloatingPoint.h>

#include <cmath>
#include <ostream>
#include <string_view>
 
 
namespace unit {


/** 
 * @class Unit
 * @brief 
 */
template <typename T, typename UnitSystemT>
class Unit {

public:

  // ---------------------------------------------------
  // public types
  using value_type = T;
  using unitSystem = UnitSystemT;
  using dimension  = typename unitSystem::dimension;

private:

  // private type alias for SFINAE
  template <typename U>
  using RequiresSameDimension = RequiresSameDimension<unitSystem, typename U::unitSystem>;

public:

  // ---------------------------------------------------
  // constructor, destructor & assignment
  constexpr Unit           () = default;
  constexpr Unit           (value_type v) noexcept;
  constexpr Unit           (Unit const&) noexcept  = default;
  constexpr Unit           (Unit&&) noexcept = default;
  constexpr auto operator= (Unit const&) noexcept -> Unit& = default;
  constexpr auto operator= (Unit &&) noexcept -> Unit& = default;
  ~Unit() = default;

  template <typename U, typename = RequiresSameDimension<U>>
  constexpr Unit           (U const u) noexcept;
  template <typename U, typename = RequiresSameDimension<U>>
  constexpr auto operator= (U const u) noexcept -> Unit&;
  auto operator=           (value_type v) noexcept -> Unit&;

  // ---------------------------------------------------
  // get and set methods
  [[nodiscard]] constexpr auto value    () const noexcept -> value_type;
  void                         setValue (value_type v) noexcept;

  // ---------------------------------------------------
  // arithmetic methods
  template <typename U, typename = RequiresSameDimension<U>>
  constexpr auto operator+= (U const u) noexcept -> Unit&;
  template <typename U, typename = RequiresSameDimension<U>>
  constexpr auto operator-= (U const u) noexcept -> Unit&;
  constexpr auto operator*= (value_type v) noexcept -> Unit&;
  constexpr auto operator/= (value_type v) -> Unit&;

private:

  // ---------------------------------------------------
  // private data
  value_type val{};
};


/** 
 * @brief CommonType is alias for the common value_type of two Units.
 */
template <typename U1, typename U2>
using CommonType = std::common_type_t<typename U1::value_type, typename U2::value_type>;

/**
 * @struct IsUnitT
 * @brief  IsUnitT is a metafunction that detects if a given type T is a Unit,
 *         i.e. it checks whether T has member types unitSystem and value_type.
 */
template <typename U, typename = std::void_t<>>
struct IsUnitT : std::false_type
{};

template <typename U>
struct IsUnitT<U, std::void_t<typename U::value_type, typename U::unitSystem, typename U::dimension>>
  : std::true_type
{};

template <typename U>
constexpr bool IsUnit = IsUnitT<U>::value;

/**
 * @brief EnableForUnit is a convenient alias template to simplify SFINAE expressions.
 */
template <typename U>
using EnableForUnit = std::enable_if_t<IsUnit<U>>;

/**
 * @brief EnableForSameDimensions is a convenient alias template to simplify SFINAE expressions.
 */
template <typename U1, typename U2>
using EnableForSameDimensions = RequiresSameDimension<typename U1::unitSystem, typename U2::unitSystem>;

/** 
 * @brief UnitCast converts a Unit to a different Unit with same dimension.
 */
template <typename U1, typename U2>
[[nodiscard]] auto constexpr UnitCast(U2 const u) noexcept -> U1
{
  using commonValueType = std::common_type_t<CommonType<U1, U2>, double>; // use at least double as the conversion factor type
  using S1 = typename U1::unitSystem;
  using S2 = typename U2::unitSystem;
  return U1{u.value() * ConversionFactor<S1, S2, commonValueType> + Shift<S1, S2, commonValueType>};
}


/** 
 * @brief parametrized constructor. Initializes the stored value.
 */
template <typename T, typename UnitSystemT>
constexpr Unit<T, UnitSystemT>::Unit(value_type v) noexcept
  : val {v}
{}

/** 
 * @brief   Templated copy constructor.
 * @remarks Only enabled if source and dest unit have the same dimensions.
 */
template <typename T, typename UnitSystemT>
template <typename U, typename>
constexpr Unit<T, UnitSystemT>::Unit(U const u) noexcept
  : val {UnitCast<Unit>(u).value()}
{}

/** 
 * @brief   Templated assignment operator.
 * @remarks Only enabled if source and dest unit have the same dimension.
 */
template <typename T, typename UnitSystemT>
template <typename U, typename>
constexpr auto Unit<T, UnitSystemT>::operator=(U const u) noexcept -> Unit&
{
  val = UnitCast<Unit>(u).value();
  return *this;
}

/**        
 * @brief  Assigns a new value to the unit.
 * @param  v the new value.
 * @return a reference to *this.
 */
template <typename T, typename UnitSystemT>
inline auto Unit<T, UnitSystemT>::operator=(value_type v) noexcept -> Unit&
{
  val = v;
  return *this;
}

/** 
 * @brief Returns the current value.
 */
template <typename T, typename UnitSystemT>
[[nodiscard]] constexpr auto Unit<T, UnitSystemT>::value() const noexcept -> value_type
{
  return val;
}

/** 
 * @brief Assigns a new value.
 */
template<typename T, typename UnitSystemT>
inline void Unit<T, UnitSystemT>::setValue(value_type v) noexcept
{
  val = v;
}

/** 
 * @brief Addition with a unit of same dimension.
 */
template <typename T, typename UnitSystemT>
template <typename U, typename>
constexpr auto Unit<T, UnitSystemT>::operator+=(U const u) noexcept -> Unit&
{
  val += UnitCast<Unit>(u).value();
  return *this;
}

/** 
 * @brief Subtraction with a unit of same dimension.
 */
template <typename T, typename UnitSystemT>
template <typename U, typename>
constexpr auto Unit<T, UnitSystemT>::operator-=(U const u) noexcept -> Unit&
{
  val -= UnitCast<Unit>(u).value();
  return *this;
}

/** 
 * @brief muliplicates the given unit with a given value.
 */
template <typename T, typename UnitSystemT>
constexpr auto Unit<T, UnitSystemT>::operator*=(value_type v) noexcept -> Unit&
{
  val *= v;
  return *this;
}

/** 
 * @brief divides the given unit by a given value.
 */
template <typename T, typename UnitSystemT>
constexpr auto Unit<T, UnitSystemT>::operator/=(value_type v) -> Unit&
{
  val /= v;
  return *this;
}

/** 
 * @brief   Templated plus operator.
 * @remarks Only enabled if source and dest unit have the same dimensions, e.g. the same exponent list.
 */
template <typename U1, typename U2, typename = EnableForSameDimensions<U1, U2>>
constexpr auto operator+(U1 const ul, U2 const ur) noexcept -> U1
{
  U1 copy{ul};
  return copy += ur;
}

/** 
 * @brief   Templated plus operator.
 * @remarks Only enabled if source and dest unit have the same dimensions, e.g. the same exponent list.
 */
template <typename U1, typename U2, typename = EnableForSameDimensions<U1, U2>>
constexpr auto operator-(U1 const ul, U2 const ur) noexcept -> U1
{
  U1 copy{ul};
  return copy -= ur;
}


template <typename U>
using BaseUnit = Unit<typename U::value_type, unit::BaseUnitGenerator<typename U::unitSystem>>;

/** 
 * @brief N-th power of units.
 */
template <typename U, std::size_t N>
using UnitPowerType = Unit<typename U::value_type, unit::arithmetic::NthPower<typename U::unitSystem, N>>;

template <std::size_t N, typename U>
[[nodiscard]] auto constexpr pow(U const u) noexcept
{
  return UnitPowerType<U, N>{meta::Power<N>(u.value())};
}

/** 
 * @brief Multiplication of two equal units.
 */
template <typename U>
[[nodiscard]] auto constexpr operator*(U const u1, U const u2) noexcept
{
  return UnitPowerType<U, 2ULL>{u1.value() * u2.value()};
}

/** 
 * @brief Templated multiplication of different units.
 */
template <typename U1, typename U2>
using MultiplicationType =
  Unit<CommonType<U1, U2>, arithmetic::MultiplicationType<typename U1::unitSystem, typename U2::unitSystem>>;

template <typename U1, typename U2>
[[nodiscard]] auto constexpr operator*(U1 const ul, U2 const ur) noexcept -> MultiplicationType<U1, U2>
{
  return MultiplicationType<U1, U2>{UnitCast<BaseUnit<U1>>(ul).value() * UnitCast<BaseUnit<U2>>(ur).value()};
}

/**
 * @brief Templated division of units.
 */
template <typename U1, typename U2>
using DivisionType = std::conditional_t<
  dimension::HasOnlyZeroExponents<dimension::DivisionType<DimensionOf<U1>, DimensionOf<U2>>>,
  CommonType<U1, U2>,
  Unit<CommonType<U1, U2>, arithmetic::DivisionType<typename U1::unitSystem, typename U2::unitSystem>>
>;

template <typename U1, typename U2, typename = std::void_t<typename U1::value_type, typename U2::value_type, typename U1::unitSystem>>
[[nodiscard]] auto constexpr operator/(U1 const ul, U2 const ur) -> DivisionType<U1, U2>
{
  return DivisionType<U1, U2>{UnitCast<BaseUnit<U1>>(ul).value() / UnitCast<BaseUnit<U2>>(ur).value()};
}

/**
 * @brief Multiplication with scalars.
 */
template <typename U>
[[nodiscard]] auto constexpr operator*(U const unit, typename U::value_type const v) noexcept -> U
{
  return U{unit.value() * v};
}

template <typename U>
[[nodiscard]] auto constexpr operator*(typename U::value_type const v, U const unit) noexcept -> U
{
  return unit * v;
}

/**
 * @brief Division by a scalar.
 */
template <typename U>
[[nodiscard]] auto constexpr operator/(U const unit, typename U::value_type const v) -> U
{
  return U{unit.value() / v};
}

template <typename U>
using UnitInversionType = Unit<typename U::value_type, unit::arithmetic::InversionOf<typename U::unitSystem>>;

template <typename U>
auto constexpr invertUnit(U const unit) -> UnitInversionType<U>
{
  return UnitInversionType<U>{1.0 / unit.value()};
}

template <typename U, typename = std::void_t<typename U::value_type>>
auto constexpr operator/(typename U::value_type v, U const unit) -> UnitInversionType<U>
{
  return UnitInversionType<U>{v / unit.value()};
}

/**
 * @brief calculates the N-th root of a given unit.
 */
template <typename U, std::size_t N>
using UnitNthRootType = Unit<typename U::value_type, unit::arithmetic::NthRoot<typename U::unitSystem, N>>;

template <std::size_t N, typename U>
[[nodiscard]] inline auto root(U const unit) -> UnitNthRootType<U, N>
{
  return UnitNthRootType<U, N>{std::pow(unit.value(), 1.0 / N)};
}

/**
 * @brief calculates the square root of a given unit.
 */
template <typename U>
[[nodiscard]] inline auto sqrt(U const unit) -> UnitNthRootType<U, 2ULL>
{
  return root<2ULL>(unit);
}

/**
 * @brief Comparison for almost equality of two units. Returns true if the distance in ULP (units in the last place)
 *        of the unit values is less than a maximum which is given by a template parameter.
 */
template <typename U, std::size_t MaxULPDist = 4ULL, typename = EnableForUnit<U>>
inline [[nodiscard]] auto equals(U const ul, U const ur) -> bool
{
  return cpptools::FloatingPoint<typename U::value_type>::almostEqual<MaxULPDist>(ul.value(), ur.value());
}

template <typename U, typename = EnableForUnit<U>>
inline [[nodiscard]] auto operator==(U const ul, U const ur) -> bool
{
  return equals<U, 1ULL>(ul, ur);
}

/**
 * @brief Comparison for almost equality of two units of same dimension but different units.
 *        Returns true if the distance in ULP (units in the last place)
 *        of the unit values is less than a maximum which is given by a template parameter.
 */
template <typename U1, typename U2, std::size_t MaxULPDist = 4ULL, typename = EnableForSameDimensions<U1, U2>>
inline [[nodiscard]] auto equals(U1 const ul, U2 const ur) -> bool
{
  return equals<U1, MaxULPDist>(ul, UnitCast<U1>(ur));
}

template <typename U1, typename U2, typename = EnableForSameDimensions<U1, U2>>
inline [[nodiscard]] auto operator==(U1 const ul, U2 const ur) -> bool
{
  return equals<U1, U2, 1ULL>(ul, ur);
}

/**
 * @brief Comparison for almost equality of a unit and a scalar. Returns true if the distance in ULP (units in the last place)
 *        of the unit values is less than a maximum which is given by a template parameter.
 */
template <typename U, std::size_t MaxULPDist = 4ULL, typename = EnableForUnit<U>>
inline [[nodiscard]] auto equals(U const u, typename U::value_type const v) -> bool
{
  return cpptools::FloatingPoint<typename U::value_type>::almostEqual<MaxULPDist>(u.value(), v);
}

template <typename U, typename = EnableForUnit<U>>
inline [[nodiscard]] auto operator==(U const u, typename U::value_type const v) -> bool
{
  return equals<U, 1ULL>(u, v);
}

template <typename U, typename = EnableForUnit<U>>
inline [[nodiscard]] auto operator==(typename U::value_type const v, U const u) -> bool
{
  return equals<U, 1ULL>(u, v);
}

/**
 * @brief less operator for two units
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator<(U const ul, U const ur) noexcept -> bool
{
  return ul.value() < ur.value();
}

/**
 * @brief less or equal operator for two units
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator<=(U const ul, U const ur) noexcept -> bool
{
  return ul.value() <= ur.value();
}

/**
 * @brief greater operator for two units
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator>(U const ul, U const ur) noexcept -> bool
{
  return ur < ul;
}

/**
 * @brief greater or equal operator for two units
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator>=(U const ul, U const ur) noexcept -> bool
{
  return ur <= ul;
}

/**
 * @brief less operator for a unit and a scalar
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator<(U const u, typename U::value_type const v) noexcept -> bool
{
  return u.value() < v;
}

/**
 * @brief less or equal operator for a unit and a scalar
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator<=(U const u, typename U::value_type const v) noexcept -> bool
{
  return u.value() <= v;
}

/**
 * @brief greater operator for a unit and a scalar
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator>(U const u, typename U::value_type const v) noexcept -> bool
{
  return u.value() > v;
}

/**
 * @brief greater or equal operator for a unit and a scalar
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator>=(U const u, typename U::value_type const v) noexcept -> bool
{
  return u.value() >= v;
}

/**
 * @brief less operator for a scalar and a unit
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator<(typename U::value_type const v, U const u) noexcept -> bool
{
  return u > v;
}

/**
 * @brief less or equal operator for a scalar and a unit
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator<=(typename U::value_type const v, U const u) noexcept -> bool
{
  return u >= v;
}

/**
 * @brief greator operator for a scalar and a unit
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator>(typename U::value_type const v, U const u) noexcept -> bool
{
  return u < v;
}

/**
 * @brief greter or equal operator for a scalar and a unit
 */
template <typename U, typename = EnableForUnit<U>>
[[nodiscard]] constexpr auto operator>=(typename U::value_type const v, U const u) noexcept -> bool
{
  return u <= v;
}


/**
 * @brief Computes a unit with the largest integer value not bigger than the value of unit.
 */
template <typename U, typename = std::void_t<typename U::value_type>>
auto constexpr floor(U const u) -> U
{
//  constexpr auto i = static_cast<std::int64_t>(unit.value());
  return U(static_cast<typename U::value_type>(static_cast<std::int64_t>(u.value())));
//  return U(i < unit.value() ? i : i - 1LL);
}

/**
 * @brief Computes a unit with the smallest integer value not less than the value of unit.
 */
template <typename U, typename = std::void_t<typename U::value_type>>
auto constexpr ceil(U const u) -> U
{
  return U(static_cast<typename U::value_type>(static_cast<std::int64_t>(u.value() + 1.0)));
}

/**
 * @brief Rounds to the nearest integer value, rounding halfway cases away from zero.
 */
template <typename U, typename = std::void_t<typename U::value_type>>
inline auto round(U const u) -> U
{
//  return U(static_cast<typename U::value_type>(static_cast<std::int64_t>(unit.value() + 0.5)));
  return U(static_cast<typename U::value_type>(std::round(u.value())));
}


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNIT_H_179732885328477293061676127622711153282600 
