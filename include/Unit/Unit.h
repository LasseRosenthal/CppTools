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
#include <Unit/detail/UnitSystemConversion.h>
 
 
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
  template <typename UnitT>
  using RequiresSameDimension = RequiresSameDimension<unitSystem, typename UnitT::unitSystem>;

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

  template <typename UnitFrom, typename = RequiresSameDimension<UnitFrom>>
  constexpr Unit           (UnitFrom const& unitFrom) noexcept;
  template <typename UnitFrom, typename = RequiresSameDimension<UnitFrom>>
  constexpr auto operator= (UnitFrom const& unitFrom) noexcept -> Unit&;

  constexpr auto value() const noexcept -> value_type;

  // ---------------------------------------------------
  // arithmetic methods
  template <typename UnitFrom, typename = RequiresSameDimension<UnitFrom>>
  constexpr auto operator+= (UnitFrom const& unitFrom) noexcept -> Unit&;
  template <typename UnitFrom, typename = RequiresSameDimension<UnitFrom>>
  constexpr auto operator-= (UnitFrom const& unitFrom) noexcept -> Unit&;
  auto operator*=           (value_type v) noexcept -> Unit&;
  auto operator/=           (value_type v) -> Unit&;

private:

  // ---------------------------------------------------
  // private data
  value_type val{};
};


/** 
 * @brief CommonType is alias for the common value_type of two Units.
 */
template <typename UnitT1, typename Unit2>
using CommonType = std::common_type_t<typename UnitT1::value_type, typename Unit2::value_type>;

/** 
 * @brief UnitCast converts a Unit to a different Unit with same dimension.
 */
template <typename UnitTo, typename UnitFrom>
auto constexpr UnitCast(UnitFrom const& unitFrom) noexcept -> std::enable_if_t<UnitFrom::dimension::isRational, UnitTo>
{
  using factor = typename ConversionFactorT<typename UnitTo::unitSystem, typename UnitFrom::unitSystem>::factor;
  return UnitTo{unitFrom.value() * meta::AsDecimal<factor, CommonType<UnitTo, UnitFrom>>};

//  return UnitTo{(unitFrom.value() * factor::num) / factor::den};
}

template <typename UnitTo, typename UnitFrom>
auto constexpr UnitCast(UnitFrom const& unitFrom) noexcept -> std::enable_if_t<!UnitFrom::dimension::isRational, UnitTo>
{
  return UnitTo{unitFrom.value() * ConversionFactorT<UnitTo, UnitFrom>::value};
}



/** 
 * @brief parametrized constructor. Initializes the stored value.
 */
template <typename T, typename UnitSystemT>
constexpr Unit<T, UnitSystemT>::Unit(value_type v) noexcept
  : val{v}
{}

/** 
 * @brief   Templated copy constructor.
 * @remarks Only enabled if source and dest unit have the same dimensions.
 */
template <typename T, typename UnitSystemT>
template <typename UnitFrom, typename>
constexpr Unit<T, UnitSystemT>::Unit(UnitFrom const& unitFrom) noexcept
  : val{UnitCast<Unit>(unitFrom).value()}
{}

/** 
 * @brief   Templated assignment operator.
 * @remarks Only enabled if source and dest unit have the same dimension.
 */
template <typename T, typename UnitSystemT>
template <typename UnitFrom, typename>
constexpr auto Unit<T, UnitSystemT>::operator=(UnitFrom const& unitFrom) noexcept -> Unit&
{
  val = UnitCast<Unit>(unitFrom).value();
  return *this;
}

/** 
 * @brief Returns the current value.
 */
template <typename T, typename UnitSystemT>
constexpr auto Unit<T, UnitSystemT>::value() const noexcept -> value_type
{
  return val;
}

/** 
 * @brief Addition with a unit of same dimension.
 */
template <typename T, typename UnitSystemT>
template <typename UnitFrom, typename>
constexpr auto Unit<T, UnitSystemT>::operator+=(UnitFrom const& unitFrom) noexcept -> Unit&
{
  val += UnitCast<Unit>(unitFrom).value();
  return *this;
}

/** 
 * @brief Subtraction with a unit of same dimension.
 */
template <typename T, typename UnitSystemT>
template <typename UnitFrom, typename>
constexpr auto Unit<T, UnitSystemT>::operator-=(UnitFrom const& unitFrom) noexcept -> Unit&
{
  val -= UnitCast<Unit>(unitFrom).value();
  return *this;
}

/** 
 * @brief muliplicates the given unit with a given value.
 */
template <typename T, typename UnitSystemT>
auto Unit<T, UnitSystemT>::operator*=(value_type v) noexcept -> Unit&
{
  val *= v;
  return *this;
}

/** 
 * @brief divides the given unit by a given value.
 */
template <typename T, typename UnitSystemT>
auto Unit<T, UnitSystemT>::operator/=(value_type v) -> Unit&
{
  val /= v;
  return *this;
}



template <typename UnitT>
using BaseUnit = Unit<typename UnitT::value_type, unit::BaseUnitGenerator<typename UnitT::unitSystem>>;

template <typename UnitT1, typename UnitT2>
using MultiplicationType =
  Unit<CommonType<UnitT1, UnitT2>,
       unit::arithmetic::MultiplicationType<typename UnitT1::unitSystem, typename UnitT2::unitSystem>>;

template <typename UnitT1, typename UnitT2>
using DivisionType =
  Unit<CommonType<UnitT1, UnitT2>,
       unit::arithmetic::DivisionType<typename UnitT1::unitSystem, typename UnitT2::unitSystem>>;

template <typename UnitT1, typename UnitT2>
auto constexpr operator*(UnitT1 const& unit1, UnitT2 const& unit2) noexcept -> MultiplicationType<UnitT1, UnitT2>
{
  using returnType = MultiplicationType<UnitT1, UnitT2>;
  return returnType{UnitCast<BaseUnit<UnitT1>>(unit1).value() * UnitCast<BaseUnit<UnitT2>>(unit2).value()};
}

template <typename UnitT1, typename UnitT2>
auto constexpr operator/(UnitT1 const& unit1, UnitT2 const& unit2) -> DivisionType<UnitT1, UnitT2>
{
  using returnType = DivisionType<UnitT1, UnitT2>;
  return returnType{UnitCast<BaseUnit<UnitT1>>(unit1).value() / UnitCast<BaseUnit<UnitT2>>(unit2).value()};
}






// ---------------------------------------------------
// convenient alias templates for time units
template <typename Rep, typename Period = std::ratio<1>>
using Time = Unit<double, unit::TimeUnitGenerator<Period>>;

using picoseconds  = Time<double, std::pico>;
using nanoseconds  = Time<double, std::nano>;
using microseconds = Time<double, std::micro>;
using milliseconds = Time<double, std::milli>;
using seconds      = Time<double>;
using minutes      = Time<double, std::ratio<60>>;
using hours        = Time<double, std::ratio<3600>>;
using days         = Time<double, std::ratio<86400>>;
using weeks        = Time<double, std::ratio<604800>>; 
using months       = Time<double, std::ratio<2629746>>;    
using years        = Time<double, std::ratio<31556952>>;

// literal operators
namespace literals {

/** 
 * @brief Literal operator for picoseconds.
 */
constexpr auto operator"" _ps(long double ps) noexcept { return picoseconds(ps);}

/** 
 * @brief Literal operator for nanoseconds.
 */
constexpr auto operator"" _ns(long double ns) noexcept { return nanoseconds(ns);}

/** 
 * @brief Literal operator for microseconds.
 */
constexpr auto operator"" _us(long double us) noexcept { return microseconds(us);}

/** 
 * @brief Literal operator for milliseconds.
 */
constexpr auto operator"" _ms(long double ms) noexcept { return milliseconds(ms);}

/** 
 * @brief Literal operator for seconds.
 */
constexpr auto operator"" _s(long double s) noexcept { return seconds(s);}

/** 
 * @brief Literal operator for minutes.
 */
constexpr auto operator"" _min(long double min) noexcept { return minutes(min);}

/** 
 * @brief Literal operator for hours.
 */
constexpr auto operator"" _h(long double h) noexcept { return hours(h);}

/** 
 * @brief Literal operator for days.
 */
constexpr auto operator"" _d(long double d) noexcept { return days(d);}

}

// ---------------------------------------------------
// convenient alias templates for length units
template <typename Rep, typename Period = std::ratio<1>>
using Length = Unit<double, unit::LengthUnitGenerator<Period>>;

using picometers  = Length<double, std::pico>;
using nanometers  = Length<double, std::nano>;
using micrometers = Length<double, std::micro>;
using millimeters = Length<double, std::milli>;
using centimeters = Length<double, std::centi>;
using decimeters  = Length<double, std::deci>;
using meters      = Length<double>;
using kilometers  = Length<double, std::kilo>;
using inch        = Length<double, std::ratio<254LL, 10'000LL>>;
using point       = Length<double, std::ratio<127LL, 360'000LL>>;
using pica        = Length<double, std::ratio<127LL, 30'000LL>>;
using lightyear   = Length<double, std::ratio<9'460'730'472'580'800LL>>;
using mile        = Length<double, std::ratio<1'609'344LL, 1'000LL>>;

// literal operators
namespace literals {
constexpr auto operator"" _nm(long double nm) { return nanometers(nm);}
constexpr auto operator"" _mm(long double mm) { return millimeters(mm);}
constexpr auto operator"" _cm(long double cm) { return centimeters(cm);}
constexpr auto operator"" _m (long double m)  { return meters(m);}
constexpr auto operator"" _km(long double km) { return kilometers(km);}
constexpr auto operator"" _in(long double in) { return inch(in);}
constexpr auto operator"" _pt(long double pt) { return point(pt);}
constexpr auto operator"" _pc(long double pc) { return pica(pc);  }
}


// ---------------------------------------------------
// convenient alias templates for Velocity units
template <typename Rep, typename TimePeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>>
using Velocity = Unit<double, unit::VelocityUnitGenerator<TimePeriod, LengthPeriod>>;

using metersPerSecond   = Velocity<double>;
using kilometersPerHour = Velocity<double, std::ratio<3600>, std::kilo>;

namespace literals {
constexpr auto operator"" _kmh(long double kmh) { return kilometersPerHour(kmh);}
}



}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNIT_H_179732885328477293061676127622711153282600 
