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

#include <string_view>
 

namespace unit {


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

// unit prefixes
template <>
constexpr std::string_view prefix<femtoseconds, char> = "fs";
template <>
constexpr std::wstring_view prefix<femtoseconds, wchar_t> = L"fs";
template <>
constexpr std::string_view prefix<picoseconds, char> = "ps";
template <>
constexpr std::wstring_view prefix<picoseconds, wchar_t> = L"ps";
template <>
constexpr std::string_view prefix<nanoseconds, char> = "ns";
template <>
constexpr std::wstring_view prefix<nanoseconds, wchar_t> = L"ns";
template <>
constexpr std::string_view prefix<microseconds, char> = "mus";
template <>
constexpr std::wstring_view prefix<microseconds, wchar_t> = L"mus";
template <>
constexpr std::string_view prefix<milliseconds, char> = "ms";
template <>
constexpr std::wstring_view prefix<milliseconds, wchar_t> = L"ms";
template <>
constexpr std::string_view prefix<seconds, char> = "s";
template <>
constexpr std::wstring_view prefix<seconds, wchar_t> = L"s";
template <>
constexpr std::string_view prefix<minutes, char> = "min";
template <>
constexpr std::wstring_view prefix<minutes, wchar_t> = L"min";
template <>
constexpr std::string_view prefix<hours, char> = "h";
template <>
constexpr std::wstring_view prefix<hours, wchar_t> = L"h";
template <>
constexpr std::string_view prefix<days, char> = "d";
template <>
constexpr std::wstring_view prefix<days, wchar_t> = L"d";
template <>
constexpr std::string_view prefix<weeks, char> = "weeks";
template <>
constexpr std::wstring_view prefix<weeks, wchar_t> = L"weeks";
template <>
constexpr std::string_view prefix<months, char> = "mon";
template <>
constexpr std::wstring_view prefix<months, wchar_t> = L"mon";
template <>
constexpr std::string_view prefix<years, char> = "a";
template <>
constexpr std::wstring_view prefix<years, wchar_t> = L"a";

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
// convenient alias templates for angle units
template <typename Rep, typename Period = std::ratio<1>, typename Scaling = std::ratio<1>>
using Angle = Unit<double, unit::AngleUnitGenerator<Period, Scaling>>;

using radians = Angle<double>;
using degrees = Angle<double, std::ratio<10000000000000000LL, 572957795130823208LL>>;

// unit prefixes
template <>
constexpr std::string_view prefix<radians, char> = "rad";
template <>
constexpr std::wstring_view prefix<radians, wchar_t> = L"rad";
template <>
constexpr std::string_view prefix<degrees, char> = "deg";
template <>
constexpr std::wstring_view prefix<degrees, wchar_t> = L"deg";

// literal operators
namespace literals {
/** @brief Literal operator for angles. */
constexpr auto operator"" _rad(long double rad) noexcept { return radians(rad);}
constexpr auto operator"" _deg(long double deg) noexcept { return degrees(deg);}
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

// unit prefixes
template <>
constexpr std::string_view prefix<hertz, char> = "Hz";
template <>
constexpr std::wstring_view prefix<hertz, wchar_t> = L"Hz";
template <>
constexpr std::string_view prefix<kilohertz, char> = "kHz";
template <>
constexpr std::wstring_view prefix<kilohertz, wchar_t> = L"kHz";
template <>
constexpr std::string_view prefix<megahertz, char> = "MHz";
template <>
constexpr std::wstring_view prefix<megahertz, wchar_t> = L"MHz";
template <>
constexpr std::string_view prefix<gigahertz, char> = "GHz";
template <>
constexpr std::wstring_view prefix<gigahertz, wchar_t> = L"GHz";
template <>
constexpr std::string_view prefix<terahertz, char> = "THz";
template <>
constexpr std::wstring_view prefix<terahertz, wchar_t> = L"THz";
template <>
constexpr std::string_view prefix<petahertz, char> = "PHz";
template <>
constexpr std::wstring_view prefix<petahertz, wchar_t> = L"PHz";
template <>
constexpr std::string_view prefix<exahertz, char> = "EHz";
template <>
constexpr std::wstring_view prefix<exahertz, wchar_t> = L"EHz";

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
constexpr std::string_view prefix<squaremeters, char> = "m²";
template <>
constexpr std::wstring_view prefix<squaremeters, wchar_t> = L"m²";

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
using kilometersPerHour = Velocity<double, std::ratio<3'600LL>, std::kilo>;

// unit prefixes
template <>
constexpr std::string_view prefix<metersPerSecond, char> = "m/s";
template <>
constexpr std::wstring_view prefix<metersPerSecond, wchar_t> = L"m/s";
template <>
constexpr std::string_view prefix<kilometersPerHour, char> = "km/h";
template <>
constexpr std::wstring_view prefix<kilometersPerHour, wchar_t> = L"km/h";

// literal operators
namespace literals {
/** @brief Literal operator for kilometers per hour. */
constexpr auto operator"" _kmh(long double kmh) noexcept { return kilometersPerHour(kmh);}
}


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITS_H_5766234103479994529869111222549189613122 
