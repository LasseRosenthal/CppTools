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

  template <typename UnitFrom, typename = RequiresSameDimension<UnitFrom>>
  constexpr Unit           (UnitFrom const& unitFrom) noexcept;
  template <typename UnitFrom, typename = RequiresSameDimension<UnitFrom>>
  constexpr auto operator= (UnitFrom const& unitFrom) noexcept -> Unit&;
  auto operator=           (value_type v) noexcept -> Unit&;

  // ---------------------------------------------------
  // get and set methods
  constexpr auto value    () const noexcept -> value_type;
  void           setValue (value_type v) noexcept;

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
template <typename U1, typename Unit2>
using CommonType = std::common_type_t<typename U1::value_type, typename Unit2::value_type>;

/**
 * @struct IsUnitT
 * @brief  IsUnitT is a metafunction that detects if a given type T is a Unit,
 *         i.e. it checks whether T has member types unitSystem and value_type.
 */
template <typename T, typename = std::void_t<>>
struct IsUnitT : std::false_type
{};

template <typename T>
struct IsUnitT<T, std::void_t<typename T::value_type, typename T::unitSystem, typename T::dimension>>
  : std::true_type
{};

template <typename T>
constexpr bool IsUnit = IsUnitT<T>::value;


template <typename U1, typename U2>
using EnableForSameDimensions = RequiresSameDimension<typename U1::unitSystem, typename U2::unitSystem>;


/** 
 * @brief UnitCast converts a Unit to a different Unit with same dimension.
 */
template <typename UnitTo, typename UnitFrom>
auto constexpr UnitCast(UnitFrom const& unitFrom) noexcept -> std::enable_if_t<UnitFrom::dimension::isRational, UnitTo>
{
  using factor = typename ConversionFactorT<typename UnitTo::unitSystem, typename UnitFrom::unitSystem>::factor;
  return UnitTo{unitFrom.value() * meta::AsDecimal<factor, CommonType<UnitTo, UnitFrom>>};
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
  : val {UnitCast<Unit>(unitFrom).value()}
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
constexpr auto Unit<T, UnitSystemT>::value() const noexcept -> value_type
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
template <typename U1, typename U2, typename = EnableForSameDimensions<U1, U2>>
constexpr auto operator+(U1 const& ul, U2 const& ur) noexcept -> U1
{
  U1 copy{ul};
  return copy += ur;
}

/** 
 * @brief   Templated plus operator.
 * @remarks Only enabled if source and dest unit have the same dimensions, e.g. the same exponent list.
 */
template <typename U1, typename U2, typename = EnableForSameDimensions<U1, U2>>
constexpr auto operator-(U1 const& ul, U2 const& ur) noexcept -> U1
{
  U1 copy{ul};
  return copy -= ur;
}


template <typename U>
using BaseUnit = Unit<typename U::value_type, unit::BaseUnitGenerator<typename U::unitSystem>>;

template <typename U1, typename U2>
using MultiplicationType =
  Unit<CommonType<U1, U2>, arithmetic::MultiplicationType<typename U1::unitSystem, typename U2::unitSystem>>;

/** 
 * @brief Templated multiplication of units.
 */
template <typename U1, typename U2>
auto constexpr operator*(U1 const& ul, U2 const& ur) noexcept -> MultiplicationType<U1, U2>
{
  using returnType = MultiplicationType<U1, U2>;
  return returnType{UnitCast<BaseUnit<U1>>(ul).value() * UnitCast<BaseUnit<U2>>(ur).value()};
}

template <typename U1, typename U2>
using DivisionType = std::conditional_t<
  dimension::HasOnlyZeroExponents<dimension::DivisionType<DimensionOf<U1>, DimensionOf<U2>>>,
  CommonType<U1, U2>,
  Unit<CommonType<U1, U2>, arithmetic::DivisionType<typename U1::unitSystem, typename U2::unitSystem>>
>;

/**
 * @brief Templated division of units.
 */
template <typename U1, typename U2, typename = std::void_t<typename U1::value_type, typename U2::value_type, typename U1::unitSystem>>
auto constexpr operator/(U1 const& ul, U2 const& ur) -> DivisionType<U1, U2>
{
  using returnType = DivisionType<U1, U2>;
  return returnType{UnitCast<BaseUnit<U1>>(ul).value() / UnitCast<BaseUnit<U2>>(ur).value()};
}

/**
 * @brief Multiplication with scalars.
 */
template <typename U>
[[nodiscard]] auto constexpr operator*(U const& unit, typename U::value_type const v) noexcept -> U
{
  return U{unit.value() * v};
}

template <typename U>
[[nodiscard]] auto constexpr operator*(typename U::value_type const v, U const& unit) noexcept -> U
{
  return unit * v;
}


/**
 * @brief Division by a scalar.
 */
template <typename U>
[[nodiscard]] auto constexpr operator/(U const& unit, typename U::value_type const v) -> U
{
  return U{unit.value() / v};
}

template <typename U>
using UnitInversionType = Unit<typename U::value_type, unit::arithmetic::InversionOf<typename U::unitSystem>>;

template <typename U>
auto constexpr invertUnit(U const& unit) -> UnitInversionType<U>
{
  return UnitInversionType<U>{1.0 / unit.value()};
}

template <typename U, typename = std::void_t<typename U::value_type>>
auto constexpr operator/(typename U::value_type v, U const& unit) -> UnitInversionType<U>
{
  return UnitInversionType<U>{v / unit.value()};
}


/**
 * @brief Comparison for almost equality of two units. Returns true if the distance in ULP (units in the last place)
 *        of the unit values is less than a maximum which is given by a template parameter.
 */
template <typename U, std::size_t MaxULPDist = 4ULL, typename = std::enable_if_t<IsUnit<U>>>
inline [[nodiscard]] auto equals(U const& ul, U const& ur) -> bool
{
  return cpptools::FloatingPoint<typename U::value_type>::almostEqual<MaxULPDist>(ul.value(), ur.value());
}

template <typename U, typename = std::enable_if_t<IsUnit<U>>>
inline [[nodiscard]] auto operator==(U const& ul, U const& ur) -> bool
{
  return equals<U, 1ULL>(ul, ur);
}

/**
 * @brief Comparison for almost equality of a unit and a scalar. Returns true if the distance in ULP (units in the last place)
 *        of the unit values is less than a maximum which is given by a template parameter.
 */
template <typename U, std::size_t MaxULPDist = 4ULL, typename = std::enable_if_t<IsUnit<U>>>
inline [[nodiscard]] auto equals(U const& u, typename U::value_type const v) -> bool
{
  return cpptools::FloatingPoint<typename U::value_type>::almostEqual<MaxULPDist>(u.value(), v);
}

template <typename U, typename = std::enable_if_t<IsUnit<U>>>
inline [[nodiscard]] auto operator==(U const& u, typename U::value_type const v) -> bool
{
  return equals<U, 1ULL>(u, v);
}

template <typename U, typename = std::enable_if_t<IsUnit<U>>>
inline [[nodiscard]] auto operator==(typename U::value_type const v, U const& u) -> bool
{
  return equals<U, 1ULL>(u, v);
}


//template <typename U1, typename U2, std::size_t MaxULPDist = 4ULL,
//          typename = std::void_t<typename U1::value_type, typename U2::value_type, typename U1::unitSystem>>
//[[nodiscard]] auto equals(U1 const& ul, U2 const& ur) -> bool
//{
//  using FloatingPoint = cpptools::FloatingPoint<typename U1::value_type>;
//  return FloatingPoint::almostEqual<MaxULPDist>(ul.value(), ur.value());
//}
//
//template <typename U1, typename U2,
//          typename = std::void_t<typename U1::value_type, typename U2::value_type, typename U1::unitSystem>>
//[[nodiscard]] auto operator==(U1 const& ul, U2 const& ur) -> bool
//{
//  return equals<U1, U2, 1ULL>(ul, ur);
//}

/**
 * @brief less operator for two units
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator<(U const& ul, U const& ur) noexcept -> bool
{
  return ul.value() < ur.value();
}

/**
 * @brief less or equal operator for two units
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator<=(U const& ul, U const& ur) noexcept -> bool
{
  return ul.value() <= ur.value();
}

/**
 * @brief greater operator for two units
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator>(U const& ul, U const& ur) noexcept -> bool
{
  return ur < ul;
}

/**
 * @brief greater or equal operator for two units
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator>=(U const& ul, U const& ur) noexcept -> bool
{
  return ur <= ul;
}

/**
 * @brief less operator for a unit and a scalar
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator<(U const& unit, typename U::value_type const v) noexcept -> bool
{
  return unit.value() < v;
}

/**
 * @brief less or equal operator for a unit and a scalar
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator<=(U const& unit, typename U::value_type const v) noexcept -> bool
{
  return unit.value() <= v;
}

/**
 * @brief greater operator for a unit and a scalar
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator>(U const& unit, typename U::value_type const v) noexcept -> bool
{
  return unit.value() > v;
}

/**
 * @brief greater or equal operator for a unit and a scalar
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator>=(U const& unit, typename U::value_type const v) noexcept -> bool
{
  return unit.value() >= v;
}

/**
 * @brief less operator for a scalar and a unit
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator<(typename U::value_type const v, U const& unit) noexcept -> bool
{
  return unit > v;
}

/**
 * @brief less or equal operator for a scalar and a unit
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator<=(typename U::value_type const v, U const& unit) noexcept -> bool
{
  return unit >= v;
}

/**
 * @brief greator operator for a scalar and a unit
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator>(typename U::value_type const v, U const& unit) noexcept -> bool
{
  return unit < v;
}

/**
 * @brief greter or equal operator for a scalar and a unit
 */
template <typename U, typename = std::enable_if_t<IsUnit<U>>>
[[nodiscard]] constexpr auto operator>=(typename U::value_type const v, U const& unit) noexcept -> bool
{
  return unit <= v;
}


/**
 * @brief Computes a unit with the largest integer value not bigger than the value of unit.
 */
template <typename U, typename = std::void_t<typename U::value_type>>
auto constexpr floor(U const& unit) -> U
{
//  constexpr auto i = static_cast<std::int64_t>(unit.value());
  return U(static_cast<typename U::value_type>(static_cast<std::int64_t>(unit.value())));
//  return U(i < unit.value() ? i : i - 1LL);
}

/**
 * @brief Computes a unit with the smallest integer value not less than the value of unit.
 */
template <typename U, typename = std::void_t<typename U::value_type>>
auto constexpr ceil(U const& unit) -> U
{
  return U(static_cast<typename U::value_type>(static_cast<std::int64_t>(unit.value() + 1.0)));
}

/**
 * @brief Rounds to the nearest integer value, rounding halfway cases away from zero.
 */
template <typename U, typename = std::void_t<typename U::value_type>>
inline auto round(U const& unit) -> U
{
//  return U(static_cast<typename U::value_type>(static_cast<std::int64_t>(unit.value() + 0.5)));
  return U(static_cast<typename U::value_type>(std::round(unit.value())));
}


template <typename U, typename CharT>
constexpr std::string_view prefix = "";

/**
 * @brief Stream operator. Prints the actual value and a unit prefix if the prefix variable
 *        template has been specialized for the given unit.
 */
template <typename CharT, typename U, typename CharTraits = std::char_traits<CharT>,
          typename = std::void_t<typename U::value_type, typename U::unitSystem>>
auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, U const& unit) -> std::basic_ostream<CharT, CharTraits>&
{
  return ostr << unit.value() << ostr.widen(' ') << prefix<U, CharT>;
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
using lightyears  = Length<double, std::ratio<9'460'730'472'580'800LL>>;
using inch        = Length<double, std::ratio<254LL, 10'000LL>>;
using points      = Length<double, std::ratio<127LL, 360'000LL>>;
using pica        = Length<double, std::ratio<127LL, 30'000LL>>;
using mile        = Length<double, std::ratio<1'609'344LL, 1'000LL>>;
using yards       = Length<double, std::ratio<9'144LL, 10'000LL>>;
using feets       = Length<double, std::ratio<3'048LL, 10'000LL>>;

// unit prefixes
template <>
constexpr std::string_view prefix<picometers, char> = "pm";
template <>
constexpr std::wstring_view prefix<picometers, wchar_t> = L"pm";
template <>
constexpr std::string_view prefix<nanometers, char> = "nm";
template <>
constexpr std::wstring_view prefix<nanometers, wchar_t> = L"nm";
template <>
constexpr std::string_view prefix<micrometers, char> = "mum";
template <>
constexpr std::wstring_view prefix<micrometers, wchar_t> = L"mum";
template <>
constexpr std::string_view prefix<millimeters, char> = "mm";
template <>
constexpr std::wstring_view prefix<millimeters, wchar_t> = L"mm";
template <>
constexpr std::string_view prefix<centimeters, char> = "cm";
template <>
constexpr std::wstring_view prefix<centimeters, wchar_t> = L"cm";
template <>
constexpr std::string_view prefix<decimeters, char> = "dm";
template <>
constexpr std::wstring_view prefix<decimeters, wchar_t> = L"dm";
template <>
constexpr std::string_view prefix<meters, char> = "m";
template <>
constexpr std::wstring_view prefix<meters, wchar_t> = L"m";
template <>
constexpr std::string_view prefix<kilometers, char> = "km";
template <>
constexpr std::wstring_view prefix<kilometers, wchar_t> = L"km";
template <>
constexpr std::string_view prefix<lightyears, char> = "ly";
template <>
constexpr std::wstring_view prefix<lightyears, wchar_t> = L"ly";
template <>
constexpr std::string_view prefix<inch, char> = "in";
template <>
constexpr std::wstring_view prefix<inch, wchar_t> = L"in";
template <>
constexpr std::string_view prefix<points, char> = "pt";
template <>
constexpr std::wstring_view prefix<points, wchar_t> = L"pt";
template <>
constexpr std::string_view prefix<pica, char> = "pica";
template <>
constexpr std::wstring_view prefix<pica, wchar_t> = L"pica";
template <>
constexpr std::string_view prefix<mile, char> = "mi";
template <>
constexpr std::wstring_view prefix<mile, wchar_t> = L"mi";
template <>
constexpr std::string_view prefix<yards, char> = "yd";
template <>
constexpr std::wstring_view prefix<yards, wchar_t> = L"yd";
template <>
constexpr std::string_view prefix<feets, char> = "ft";
template <>
constexpr std::wstring_view prefix<feets, wchar_t> = L"ft";

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
/** @brief Literal operator for lightyears. */
constexpr auto operator"" _ly(long double ly) noexcept { return lightyears(ly);}
/** @brief Literal operator for inch. */
constexpr auto operator"" _in(long double in) noexcept { return inch(in);}
/** @brief Literal operator for points. */
constexpr auto operator"" _pt(long double pt) noexcept { return points(pt);}
/** @brief Literal operator for pica. */
constexpr auto operator"" _pc(long double pc) noexcept { return pica(pc);  }
/** @brief Literal operator for mile. */
constexpr auto operator"" _mi(long double mi) noexcept { return mile(mi);  }
/** @brief Literal operator for yards. */
constexpr auto operator"" _yd(long double yd) noexcept { return yards(yd);  }
/** @brief Literal operator for feets. */
constexpr auto operator"" _ft(long double ft) noexcept { return feets(ft);  }
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

template <>
constexpr std::string_view prefix<bits, char> = "bit";
template <>
constexpr std::wstring_view prefix<bits, wchar_t> = L"bit";
template <>
constexpr std::string_view prefix<bytes, char> = "B";
template <>
constexpr std::wstring_view prefix<bytes, wchar_t> = L"B";
template <>
constexpr std::string_view prefix<kilobytes, char> = "kB";
template <>
constexpr std::wstring_view prefix<kilobytes, wchar_t> = L"kB";
template <>
constexpr std::string_view prefix<megabytes, char> = "MB";
template <>
constexpr std::wstring_view prefix<megabytes, wchar_t> = L"MB";
template <>
constexpr std::string_view prefix<gigabytes, char> = "GB";
template <>
constexpr std::wstring_view prefix<gigabytes, wchar_t> = L"GB";
template <>
constexpr std::string_view prefix<terabytes, char> = "TB";
template <>
constexpr std::wstring_view prefix<terabytes, wchar_t> = L"TB";

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






}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNIT_H_179732885328477293061676127622711153282600 
