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

template <typename UnitT1, typename UnitT2>
using EnableForSameDimensions = RequiresSameDimension<typename UnitT1::unitSystem, typename UnitT2::unitSystem>;


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

/** 
 * @brief   Templated plus operator.
 * @remarks Only enabled if source and dest unit have the same dimensions, e.g. the same exponent list.
 */
template <typename UnitT1, typename UnitT2, typename = EnableForSameDimensions<UnitT1, UnitT2>>
constexpr auto operator+(UnitT1 const& unit1, UnitT2 const& unit2) noexcept -> UnitT1
{
  UnitT1 copy{unit1};
  return copy += unit2;
}

/** 
 * @brief   Templated plus operator.
 * @remarks Only enabled if source and dest unit have the same dimensions, e.g. the same exponent list.
 */
template <typename UnitT1, typename UnitT2, typename = EnableForSameDimensions<UnitT1, UnitT2>>
constexpr auto operator-(UnitT1 const& unit1, UnitT2 const& unit2) noexcept -> UnitT1
{
  UnitT1 copy{unit1};
  return copy -= unit2;
}


template <typename UnitT>
using BaseUnit = Unit<typename UnitT::value_type, unit::BaseUnitGenerator<typename UnitT::unitSystem>>;

template <typename UnitT1, typename UnitT2>
using MultiplicationType =
  Unit<CommonType<UnitT1, UnitT2>,
       unit::arithmetic::MultiplicationType<typename UnitT1::unitSystem, typename UnitT2::unitSystem>>;

/** 
 * @brief Templated multiplication of units.
 */
template <typename UnitT1, typename UnitT2>
auto constexpr operator*(UnitT1 const& unit1, UnitT2 const& unit2) noexcept -> MultiplicationType<UnitT1, UnitT2>
{
  using returnType = MultiplicationType<UnitT1, UnitT2>;
  return returnType{UnitCast<BaseUnit<UnitT1>>(unit1).value() * UnitCast<BaseUnit<UnitT2>>(unit2).value()};
}

template <typename UnitT1, typename UnitT2>
using DivisionType = std::conditional_t<
  HasOnlyZeroExponents<DivisionType<DimensionOf<UnitT1>, DimensionOf<UnitT2>>>,
  CommonType<UnitT1, UnitT2>,
  Unit<CommonType<UnitT1, UnitT2>,
       unit::arithmetic::DivisionType<typename UnitT1::unitSystem, typename UnitT2::unitSystem>>>;

/**
 * @brief Templated division of units.
 */
template <typename UnitT1, typename UnitT2>
auto constexpr operator/(UnitT1 const& unit1, UnitT2 const& unit2) -> DivisionType<UnitT1, UnitT2>
{
  using returnType = DivisionType<UnitT1, UnitT2>;
  return returnType{UnitCast<BaseUnit<UnitT1>>(unit1).value() / UnitCast<BaseUnit<UnitT2>>(unit2).value()};
}

/**
 * @brief Multiplication with scalars.
 */
template <typename UnitT>
[[nodiscard]] auto constexpr operator*(UnitT const& unit, typename UnitT::value_type const v) noexcept -> UnitT
{
  return UnitT{unit.value() * v};
}

template <typename UnitT>
[[nodiscard]] auto constexpr operator*(typename UnitT::value_type const v, UnitT const& unit) noexcept -> UnitT
{
  return unit * v;
}

/**
 * @brief Division by a scalar.
 */
template <typename UnitT>
[[nodiscard]] auto constexpr operator/(UnitT const& unit, typename UnitT::value_type const v) -> UnitT
{
  return UnitT{unit.value() / v};
}

//template <typename UnitT>
//using UnitInversionType = Unit<typename UnitT::value_type, unit::arithmetic::InversionType<typename UnitT::unitSystem>>;

template <typename UnitT>
using UnitInversionType = Unit<typename UnitT::value_type, unit::arithmetic::InversionOf<typename UnitT::unitSystem>>;


template <typename UnitT>
auto constexpr invertUnit(UnitT const& unit) -> UnitInversionType<UnitT>
{
  return UnitInversionType<UnitT>{1.0 / unit.value()};
}

template <typename UnitT/*, typename T, typename = std::void_t<typename UnitT::value_type>*/>
auto constexpr operator/(typename UnitT::value_type v, UnitT const& unit) -> UnitInversionType<UnitT>
{
  return UnitInversionType<UnitT>{v / unit.value()};
}



// ---------------------------------------------------
// convenient alias templates for time units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Time = Unit<double, unit::TimeUnitGenerator<Period, Scaling>>;

using femtoseconds = Time<double, std::femto>;
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
/** @brief Literal operator for femtoseconds. */
constexpr auto operator"" _fs(long double fs) noexcept { return femtoseconds(fs);}
/** @brief Literal operator for picoseconds. */
constexpr auto operator"" _ps(long double ps) noexcept { return picoseconds(ps);}
/** @brief Literal operator for nanoseconds. */
constexpr auto operator"" _ns(long double ns) noexcept { return nanoseconds(ns);}
/** @brief Literal operator for microseconds. */
constexpr auto operator"" _us(long double us) noexcept { return microseconds(us);}
/** @brief Literal operator for milliseconds. */
constexpr auto operator"" _ms(long double ms) noexcept { return milliseconds(ms);}
/** @brief Literal operator for seconds. */
constexpr auto operator"" _s(long double s) noexcept { return seconds(s);}
/** @brief Literal operator for minutes. */
constexpr auto operator"" _min(long double min) noexcept { return minutes(min);}
/** @brief Literal operator for hours. */
constexpr auto operator"" _h(long double h) noexcept { return hours(h);}
/** @brief Literal operator for days. */
constexpr auto operator"" _d(long double d) noexcept { return days(d);}
}

// ---------------------------------------------------
// convenient alias templates for length units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Length = Unit<double, unit::LengthUnitGenerator<Period, Scaling>>;

using picometers  = Length<double, std::pico>;
using nanometers  = Length<double, std::nano>;
using micrometers = Length<double, std::micro>;
using millimeters = Length<double, std::milli>;
using centimeters = Length<double, std::centi>;
using decimeters  = Length<double, std::deci>;
using meters      = Length<double>;
using kilometers  = Length<double, std::kilo>;
using inch        = Length<double, std::ratio<254LL, 10'000LL>>;
using points      = Length<double, std::ratio<127LL, 360'000LL>>;
using pica        = Length<double, std::ratio<127LL, 30'000LL>>;
using lightyears  = Length<double, std::ratio<9'460'730'472'580'800LL>>;
using mile        = Length<double, std::ratio<1'609'344LL, 1'000LL>>;
using yards       = Length<double, std::ratio<9'144LL, 10'000LL>>;
using feets       = Length<double, std::ratio<3'048LL, 10'000LL>>;

// literal operators
namespace literals {
/** @brief Literal operator for nanometers. */
constexpr auto operator"" _nm(long double nm) noexcept { return nanometers(nm);}
/** @brief Literal operator for millimeters. */
constexpr auto operator"" _mm(long double mm) noexcept { return millimeters(mm);}
/** @brief Literal operator for centimeters. */
constexpr auto operator"" _cm(long double cm) noexcept { return centimeters(cm);}
/** @brief Literal operator for meters. */
constexpr auto operator"" _m (long double m)  noexcept { return meters(m);}
/** @brief Literal operator for kilometers. */
constexpr auto operator"" _km(long double km) noexcept { return kilometers(km);}
/** @brief Literal operator for inch. */
constexpr auto operator"" _in(long double in) noexcept { return inch(in);}
/** @brief Literal operator for points. */
constexpr auto operator"" _pt(long double pt) noexcept { return points(pt);}
/** @brief Literal operator for pica. */
constexpr auto operator"" _pc(long double pc) noexcept { return pica(pc);  }
/** @brief Literal operator for mile. */
constexpr auto operator"" _mile(long double mi) noexcept { return mile(mi);  }
/** @brief Literal operator for yards. */
constexpr auto operator"" _yd(long double yd) noexcept { return yards(yd);  }
/** @brief Literal operator for feets. */
constexpr auto operator"" _ft(long double ft) noexcept { return feets(ft);  }
}


// ---------------------------------------------------
// convenient alias templates for area units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Area = Unit<double, unit::AreaUnitGenerator<Period, Scaling>>;

using squarecentimeters = Area<double, std::centi>;
using squaredecimeters  = Area<double, std::deci>;
using squaremeters      = Area<double>;
using squareinches      = Area<double, std::ratio<254LL, 10'000LL>>;
using squarefeets       = Area<double, std::ratio<3'048LL, 10'000LL>>;
using Acres             = Area<double, std::ratio<3'048LL, 10'000LL>, std::ratio<43'560>>;

// literal operators
namespace literals {
/** @brief Literal operator for squarecentimeters. */
constexpr auto operator"" _qcm (long double qcm)  noexcept { return squarecentimeters(qcm);}
/** @brief Literal operator for squaredecimeters. */
constexpr auto operator"" _qdm (long double qdm)  noexcept { return squaredecimeters(qdm);}
/** @brief Literal operator for squaremeters. */
constexpr auto operator"" _qm (long double qm)  noexcept { return squaremeters(qm);}
/** @brief Literal operator for squareinches. */
constexpr auto operator"" _qin (long double qin)  noexcept { return squareinches(qin);}
/** @brief Literal operator for Acres. */
constexpr auto operator"" _ac (long double ac)  noexcept { return Acres(ac);}
}


// ---------------------------------------------------
// convenient alias templates for Velocity units
template <typename Rep, typename TimePeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>, typename Scaling = std::ratio<1>>
using Velocity = Unit<double, unit::VelocityUnitGenerator<TimePeriod, LengthPeriod, Scaling>>;

using metersPerSecond   = Velocity<double>;
using kilometersPerHour = Velocity<double, std::ratio<3600>, std::kilo>;

namespace literals {
/** @brief Literal operator for kilometers per hour. */
constexpr auto operator"" _kmh(long double kmh) noexcept { return kilometersPerHour(kmh);}
}


// ---------------------------------------------------
// convenient alias templates for storage amount units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using StorageAmount = Unit<double, unit::StorageAmountUnitGenerator<Period, Scaling>>;

using bits      = StorageAmount<double>;
using bytes     = StorageAmount<double, std::ratio<8ULL>>;
using kilobytes = StorageAmount<double, std::ratio<8'000ULL>>;
using megabytes = StorageAmount<double, std::ratio<8'000'000ULL>>;
using gigabytes = StorageAmount<double, std::ratio<8'000'000'000ULL>>;
using terabytes = StorageAmount<double, std::ratio<8'000'000'000'000ULL>>;
using kibibytes = StorageAmount<double, std::ratio<8ULL * 1'024ULL>>;
using mebibytes = StorageAmount<double, std::ratio<8ULL * 1'048'576ULL>>;
using gibibytes = StorageAmount<double, std::ratio<8ULL * 1'073'741'824ULL>>;
using tebibytes = StorageAmount<double, std::ratio<8ULL * 1'099'511'627'776ULL>>;

namespace literals {
  /** @brief Literal operator for bits. */
constexpr auto operator"" _b(long double b) noexcept { return bits(b);}
/** @brief Literal operator for bytes. */
constexpr auto operator"" _B(long double b) noexcept { return bytes(b);}
/** @brief Literal operator for kilobytes. */
constexpr auto operator"" _kB  (long double kb) noexcept { return kilobytes(kb); }
/** @brief Literal operator for megabytes. */
constexpr auto operator"" _MB  (long double mb) noexcept { return megabytes(mb); }
/** @brief Literal operator for gigabytes. */
constexpr auto operator"" _GB  (long double gb) noexcept { return gigabytes(gb); }
/** @brief Literal operator for terabytes. */
constexpr auto operator"" _TB  (long double tb) noexcept { return terabytes(tb); }
/** @brief Literal operator for kibibytes. */
constexpr auto operator"" _KiB (long double kib) noexcept { return kibibytes(kib); }
/** @brief Literal operator for mebibytes. */
constexpr auto operator"" _MiB (long double mib) noexcept { return mebibytes(mib); }
/** @brief Literal operator for gibibytes. */
constexpr auto operator"" _GiB (long double gib) noexcept { return gibibytes(gib); }
/** @brief Literal operator for tebibytes. */
constexpr auto operator"" _TiB (long double tib) noexcept { return tebibytes(tib); }
}


// ---------------------------------------------------
// convenient alias templates for pixels units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Pixels = Unit<double, unit::PixelsUnitGenerator<Period, Scaling>>;

using pixels = Pixels<double>;

namespace literals {
/** @brief Literal operator for pixels. */
constexpr auto operator"" _pix(long double pix) noexcept { return pixels(pix);}
}   // namespace literals

// ---------------------------------------------------
// convenient alias templates for PixelDensity units
template <typename Rep, typename PixelPeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>, typename Scaling = std::ratio<1>>
using PixelDensity = Unit<double, unit::ResolutionUnitGenerator<PixelPeriod, LengthPeriod, Scaling>>;

using pixelsPerInch = PixelDensity<double, std::ratio<1>, std::ratio<254LL, 10'000LL>>; 

namespace literals {
/** @brief Literal operator for pixelsPerInch. */
constexpr auto operator"" _dpi(long double dpi) noexcept { return pixelsPerInch(dpi);}
}   // namespace literals


// ---------------------------------------------------
// convenient alias templates for frequency units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Frequency = Unit<double, unit::FrequencyUnitGenerator<Period, Scaling>>;

using hertz      = Frequency<double>;
using kilohertz  = Frequency<double, std::kilo>;
using megahertz  = Frequency<double, std::mega>;
using gigahertz  = Frequency<double, std::giga>;
using terahertz  = Frequency<double, std::tera>;
using petahertz  = Frequency<double, std::peta>;
using exahertz   = Frequency<double, std::exa>;

namespace literals {
/** @brief Literal operator for hertz. */
constexpr auto operator"" _Hz(long double Hz) noexcept { return hertz(Hz);}
/** @brief Literal operator for kilohertz. */
constexpr auto operator"" _kHz(long double kHz) noexcept { return kilohertz(kHz);}
/** @brief Literal operator for megahertz. */
constexpr auto operator"" _MHz(long double MHz) noexcept { return megahertz(MHz);}
/** @brief Literal operator for gigahertz. */
constexpr auto operator"" _GHz(long double GHz) noexcept { return gigahertz(GHz);}
/** @brief Literal operator for terahertz. */
constexpr auto operator"" _THz(long double THz) noexcept { return terahertz(THz);}
/** @brief Literal operator for petahertz. */
constexpr auto operator"" _PHz(long double PHz) noexcept { return petahertz(PHz);}
/** @brief Literal operator for exahertz. */
constexpr auto operator"" _EHz(long double EHz) noexcept { return exahertz(EHz);}
}



}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNIT_H_179732885328477293061676127622711153282600 
