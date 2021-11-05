/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Units.h 
 * @brief   Definition of alias templates and literal operators for common units.
 * 
 * @author  Lasse Rosenthal 
 * @date    13.09.2021 
 */ 
 
#ifndef UNITS_H_5766234103479994529869111222549189613122 
#define UNITS_H_5766234103479994529869111222549189613122 
 
 
// includes
#include <Unit/Unit.h>
 

namespace unit {


/**
 * @brief returns a prefix string for a given unit.
 */
template <typename U, typename CharT>
constexpr auto prefix() -> const CharT* const;

/**
 * @brief Stream operator. Prints the actual value and a unit prefix if the prefix variable
 *        template has been specialized for the given unit.
 */
template <typename CharT, typename U, typename CharTraits = std::char_traits<CharT>,
          typename = std::void_t<typename U::value_type, typename U::unitSystem>>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, U const u) -> std::basic_ostream<CharT, CharTraits>&
{
  return ostr << u.value() << ostr.widen(' ') << prefix<U, CharT>();
}

// ---------------------------------------------------
// convenient alias templates for time units
// ---------------------------------------------------
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

// unit prefixes
template <>
auto prefix<femtoseconds, char>() -> const char* const { return "fs";}
template <>
auto prefix<femtoseconds, wchar_t>() -> const wchar_t* const { return L"fs";}
template <>
auto prefix<picoseconds, char>() -> const char* const { return "ps";}
template <>
auto prefix<picoseconds, wchar_t>() -> const wchar_t* const { return L"ps";}
template <>
auto prefix<nanoseconds, char>() -> const char* const { return "ns";}
template <>
auto prefix<nanoseconds, wchar_t>() -> const wchar_t* const { return L"ns";}
template <>
auto prefix<microseconds, char>() -> const char* const { return "mus";}
template <>
auto prefix<microseconds, wchar_t>() -> const wchar_t* const { return L"mus";}
template <>
auto prefix<milliseconds, char>() -> const char* const { return "ms";}
template <>
auto prefix<milliseconds, wchar_t>() -> const wchar_t* const { return L"ms";}
template <>
auto prefix<seconds, char>() -> const char* const { return "s";}
template <>
auto prefix<seconds, wchar_t>() -> const wchar_t* const { return L"s";}
template <>
auto prefix<minutes, char>() -> const char* const { return "min";}
template <>
auto prefix<minutes, wchar_t>() -> const wchar_t* const { return L"min";}
template <>
auto prefix<hours, char>() -> const char* const { return "h";}
template <>
auto prefix<hours, wchar_t>() -> const wchar_t* const { return L"h";}
template <>
auto prefix<days, char>() -> const char* const { return "d";}
template <>
auto prefix<days, wchar_t>() -> const wchar_t* const { return L"d";}
template <>
auto prefix<weeks, char>() -> const char* const { return "weeks";}
template <>
auto prefix<weeks, wchar_t>() -> const wchar_t* const { return L"weeks";}
template <>
auto prefix<months, char>() -> const char* const { return "mon";}
template <>
auto prefix<months, wchar_t>() -> const wchar_t* const { return L"mon";}
template <>
auto prefix<years, char>() -> const char* const { return "a";}
template <>
auto prefix<years, wchar_t>() -> const wchar_t* const { return L"a";}

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
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Length = Unit<Rep, unit::LengthUnitGenerator<Period, Scaling>>;

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
auto prefix<picometers, char>() -> const char* const { return "pm";}
template <>
auto prefix<picometers, wchar_t>() -> const wchar_t* const { return L"pm";}
template <>
auto prefix<nanometers, char>() -> const char* const { return "nm";}
template <>
auto prefix<nanometers, wchar_t>() -> const wchar_t* const { return L"nm";}
template <>
auto prefix<micrometers, char>() -> const char* const { return "mum";}
template <>
auto prefix<micrometers, wchar_t>() -> const wchar_t* const { return L"mum";}
template <>
auto prefix<millimeters, char>() -> const char* const { return "mm";}
template <>
auto prefix<millimeters, wchar_t>() -> const wchar_t* const { return L"mm";}
template <>
auto prefix<centimeters, char>() -> const char* const { return "cm";}
template <>
auto prefix<centimeters, wchar_t>() -> const wchar_t* const { return L"cm";}
template <>
auto prefix<decimeters, char>() -> const char* const { return "dm";}
template <>
auto prefix<decimeters, wchar_t>() -> const wchar_t* const { return L"dm";}
template <>
auto prefix<meters, char>() -> const char* const { return "m";}
template <>
auto prefix<meters, wchar_t>() -> const wchar_t* const { return L"m";}
template <>
auto prefix<kilometers, char>() -> const char* const { return "km";}
template <>
auto prefix<kilometers, wchar_t>() -> const wchar_t* const { return L"km";}
template <>
auto prefix<lightyears, char>() -> const char* const { return "ly";}
template <>
auto prefix<lightyears, wchar_t>() -> const wchar_t* const { return L"ly";}
template <>
auto prefix<inch, char>() -> const char* const { return "in";}
template <>
auto prefix<inch, wchar_t>() -> const wchar_t* const { return L"in";}
template <>
auto prefix<points, char>() -> const char* const { return "pt";}
template <>
auto prefix<points, wchar_t>() -> const wchar_t* const { return L"pt";}
template <>
auto prefix<pica, char>() -> const char* const { return "pica";}
template <>
auto prefix<pica, wchar_t>() -> const wchar_t* const { return L"pica";}
template <>
auto prefix<mile, char>() -> const char* const { return "mi";}
template <>
auto prefix<mile, wchar_t>() -> const wchar_t* const { return L"mi";}
template <>
auto prefix<yards, char>() -> const char* const { return "yd";}
template <>
auto prefix<yards, wchar_t>() -> const wchar_t* const { return L"yd";}
template <>
auto prefix<feets, char>() -> const char* const { return "ft";}
template <>
auto prefix<feets, wchar_t>() -> const wchar_t* const { return L"ft";}

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
// convenient alias templates for temperature units
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Shift = std::ratio<0>>
using Temperature = Unit<Rep, unit::TemperatureUnitGenerator<Period, Shift>>;

using kelvin     = Temperature<double>;
using celsius    = Temperature<double, std::ratio<1>, std::ratio<-27'315LL, 100LL>>;
using fahrenheit = Temperature<double, std::ratio<5LL, 9LL>, std::ratio<-45'967LL, 100LL>>;
using rankine    = Temperature<double, std::ratio<5LL, 9LL>>;

// unit prefixes
template <>
auto prefix<kelvin, char>() -> const char* const { return "K";}
template <>
auto prefix<kelvin, wchar_t>() -> const wchar_t* const { return L"K";}
template <>
auto prefix<celsius, char>() -> const char* const { return "C";}
template <>
auto prefix<celsius, wchar_t>() -> const wchar_t* const { return L"C";}
template <>
auto prefix<fahrenheit, char>() -> const char* const { return "F";}
template <>
auto prefix<fahrenheit, wchar_t>() -> const wchar_t* const { return L"F";}
template <>
auto prefix<rankine, char>() -> const char* const { return "Ra";}
template <>
auto prefix<rankine, wchar_t>() -> const wchar_t* const { return L"Ra";}

namespace literals {
  /** @brief Literal operator for kelvin. */
constexpr auto operator"" _K(long double k) noexcept { return kelvin(k);}
  /** @brief Literal operator for celsius. */
constexpr auto operator"" _C(long double c) noexcept { return celsius(c);}
  /** @brief Literal operator for fahrenheit. */
constexpr auto operator"" _F(long double f) noexcept { return fahrenheit(f);}
  /** @brief Literal operator for rankine. */
constexpr auto operator"" _Ra(long double ra) noexcept { return rankine(ra);}
}


// ---------------------------------------------------
// convenient alias templates for storage amount units
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using StorageAmount = Unit<Rep, unit::StorageAmountUnitGenerator<Period, Scaling>>;

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

// unit prefixes
template <>
auto prefix<bits, char>() -> const char* const { return "bit";}
template <>
auto prefix<bits, wchar_t>() -> const wchar_t* const { return L"bit";}
template <>
auto prefix<bytes, char>() -> const char* const { return "B";}
template <>
auto prefix<bytes, wchar_t>() -> const wchar_t* const { return L"B";}
template <>
auto prefix<kilobytes, char>() -> const char* const { return "kB";}
template <>
auto prefix<kilobytes, wchar_t>() -> const wchar_t* const { return L"kB";}
template <>
auto prefix<megabytes, char>() -> const char* const { return "MB";}
template <>
auto prefix<megabytes, wchar_t>() -> const wchar_t* const { return L"MB";}
template <>
auto prefix<gigabytes, char>() -> const char* const { return "GB";}
template <>
auto prefix<gigabytes, wchar_t>() -> const wchar_t* const { return L"GB";}
template <>
auto prefix<terabytes, char>() -> const char* const { return "TB";}
template <>
auto prefix<terabytes, wchar_t>() -> const wchar_t* const { return L"TB";}

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
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Pixels = Unit<Rep, unit::PixelsUnitGenerator<Period, Scaling>>;

using pixels = Pixels<double>;

// unit prefixes
template <>
constexpr auto prefix<pixels, char>() -> const char* const { return "pix";}
template <>
constexpr auto prefix<pixels, wchar_t>() -> const wchar_t* const { return L"pix";}
template <>
constexpr auto prefix<Pixels<std::int64_t>, char>() -> const char* const { return "pix";}
template <>
constexpr auto prefix<Pixels<std::int64_t>, wchar_t>() -> const wchar_t* const { return L"pix";}

namespace literals {
/** @brief Literal operator for pixels. */
constexpr auto operator"" _pix(long double pix) noexcept { return pixels(pix);}
}   // namespace literals


// ---------------------------------------------------
// convenient alias templates for PixelDensity units
// ---------------------------------------------------
template <typename Rep, typename PixelPeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>, typename Scaling = std::ratio<1>>
using PixelDensity = Unit<Rep, unit::ResolutionUnitGenerator<PixelPeriod, LengthPeriod, Scaling>>;

using pixelsPerInch = PixelDensity<double, std::ratio<1>, std::ratio<254LL, 10'000LL>>; 

// unit prefixes
template <>
auto prefix<pixelsPerInch, char>() -> const char* const { return "dpi";}
template <>
auto prefix<pixelsPerInch, wchar_t>() -> const wchar_t* const { return L"dpi";}

namespace literals {
/** @brief Literal operator for pixelsPerInch. */
constexpr auto operator"" _dpi(long double dpi) noexcept { return pixelsPerInch(dpi);}
}   // namespace literals


// ---------------------------------------------------
// convenient alias templates for angle units
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Angle = Unit<Rep, unit::AngleUnitGenerator<Period, Scaling>>;

using radians = Angle<double>;
using degrees = Angle<double, std::ratio<10'000'000'000'000'000LL, 572'957'795'130'823'208LL>>;

// unit prefixes
template <>
auto prefix<radians, char>() -> const char* const { return "rad";}
template <>
auto prefix<radians, wchar_t>() -> const wchar_t* const { return L"rad";}
template <>
auto prefix<degrees, char>() -> const char* const { return "deg";}
template <>
auto prefix<degrees, wchar_t>() -> const wchar_t* const { return L"deg";}

// literal operators
namespace literals {
/** @brief Literal operator for angles. */
constexpr auto operator"" _rad(long double rad) noexcept { return radians(rad);}
constexpr auto operator"" _deg(long double deg) noexcept { return degrees(deg);}
}   // namespace literals


// ---------------------------------------------------
// convenient alias templates for frequency units
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Frequency = Unit<double, unit::FrequencyUnitGenerator<Period, Scaling>>;

using hertz     = Frequency<double>;
using kilohertz = Frequency<double, std::kilo>;
using megahertz = Frequency<double, std::mega>;
using gigahertz = Frequency<double, std::giga>;
using terahertz = Frequency<double, std::tera>;
using petahertz = Frequency<double, std::peta>;
using exahertz  = Frequency<double, std::exa>;

// unit prefixes
template <>
auto prefix<hertz, char>() -> const char* const { return "Hz";}
template <>
auto prefix<hertz, wchar_t>() -> const wchar_t* const { return L"Hz";}
template <>
auto prefix<kilohertz, char>() -> const char* const { return "kHz";}
template <>
auto prefix<kilohertz, wchar_t>() -> const wchar_t* const { return L"kHz";}
template <>
auto prefix<megahertz, char>() -> const char* const { return "MHz";}
template <>
auto prefix<megahertz, wchar_t>() -> const wchar_t* const { return L"MHz";}
template <>
auto prefix<gigahertz, char>() -> const char* const { return "GHz";}
template <>
auto prefix<gigahertz, wchar_t>() -> const wchar_t* const { return L"GHz";}
template <>
auto prefix<terahertz, char>() -> const char* const { return "THz";}
template <>
auto prefix<terahertz, wchar_t>() -> const wchar_t* const { return L"THz";}
template <>
auto prefix<petahertz, char>() -> const char* const { return "PHz";}
template <>
auto prefix<petahertz, wchar_t>() -> const wchar_t* const { return L"PHz";}
template <>
auto prefix<exahertz, char>() -> const char* const { return "EHz";}
template <>
auto prefix<exahertz, wchar_t>() -> const wchar_t* const { return L"EHz";}

// literal operators
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


// ---------------------------------------------------
// convenient alias templates for area units
// ---------------------------------------------------
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Area = Unit<double, unit::AreaUnitGenerator<Period, Scaling>>;

using squarecentimeters = Area<double, std::centi>;
using squaredecimeters  = Area<double, std::deci>;
using squaremeters      = Area<double>;
using squareinches      = Area<double, std::ratio<254LL, 10'000LL>>;
using squarefeets       = Area<double, std::ratio<3'048LL, 10'000LL>>;
using Acres             = Area<double, std::ratio<3'048LL, 10'000LL>, std::ratio<43'560>>;

// unit prefixes
template <>
auto prefix<squaremeters, char>() -> const char* const { return "m^2";}
template <>
auto prefix<squaremeters, wchar_t>() -> const wchar_t* const { return L"m^2";}

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
// ---------------------------------------------------
template <typename Rep, typename TimePeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>, typename Scaling = std::ratio<1>>
using Velocity = Unit<Rep, unit::VelocityUnitGenerator<TimePeriod, LengthPeriod, Scaling>>;

using metersPerSecond   = Velocity<double>;
using kilometersPerHour = Velocity<double, std::ratio<3'600LL>, std::kilo>;

// unit prefixes
template <>
auto prefix<metersPerSecond, char>() -> const char* const { return "m/s";}
template <>
auto prefix<metersPerSecond, wchar_t>() -> const wchar_t* const { return L"m/s";}
template <>
auto prefix<kilometersPerHour, char>() -> const char* const { return "km/h";}
template <>
auto prefix<kilometersPerHour, wchar_t>() -> const wchar_t* const { return L"km/h";}

// literal operators
namespace literals {
/** @brief Literal operator for kilometers per hour. */
constexpr auto operator"" _kmh(long double kmh) noexcept { return kilometersPerHour(kmh);}
}


// ---------------------------------------------------
// convenient alias templates for Energy units
// ---------------------------------------------------
template <typename Rep, typename TimePeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>,
          typename MassPeriod = std::ratio<1>, typename Scaling = std::ratio<1>>
using Energy = Unit<Rep, unit::EnergyUnitGenerator<TimePeriod, LengthPeriod, MassPeriod, Scaling>>;

using joule       = Energy<double>;
using calorie     = Energy<double, std::ratio<1>, std::ratio<1>, std::ratio<1>, std::ratio<41'868ULL, 10'000ULL>>;
using kilocalorie = Energy<double, std::ratio<1>, std::ratio<1>, std::ratio<1>, std::ratio<41'868ULL, 10ULL>>;


// literal operators
namespace literals {
/** @brief Literal operator for kilometers per hour. */
constexpr auto operator"" _J(long double j) noexcept { return joule(j);}
constexpr auto operator"" _cal(long double c) noexcept { return calorie(c);}
constexpr auto operator"" _kcal(long double c) noexcept { return kilocalorie(c);}
}


// ---------------------------------------------------
// convenient alias templates for Power units
// ---------------------------------------------------
template <typename Rep, typename TimePeriod = std::ratio<1>, typename LengthPeriod = std::ratio<1>,
          typename MassPeriod = std::ratio<1>, typename Scaling = std::ratio<1>>
using Power = Unit<Rep, unit::PowerUnitGenerator<TimePeriod, LengthPeriod, MassPeriod, Scaling>>;

using watt = Power<double>;

// literal operators
namespace literals {
/** @brief Literal operator for kilometers per hour. */
constexpr auto operator"" _W(long double w) noexcept { return watt(w);}
}


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITS_H_5766234103479994529869111222549189613122 
