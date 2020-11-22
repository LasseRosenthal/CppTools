/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    DurationTypes.h 
 * @brief   a collection of floating type based definitions of common used durations.
 * 
 * @author  Lasse Rosenthal
 * @date    18.08.2020 
 */ 
 
#ifndef DURATIONTYPES_H_331924140113621812518610291861684095438323 
#define DURATIONTYPES_H_331924140113621812518610291861684095438323 
 
 
// includes 
#include <chrono>
#include <cstdint>


namespace benchmark {


// floating point based standard durations
using RepType = long double;

using weeks        = std::chrono::duration<RepType, std::ratio<604800>>;
using days         = std::chrono::duration<RepType, std::ratio<86400>>;
using hours        = std::chrono::duration<RepType, std::ratio<3600>>;
using minutes      = std::chrono::duration<RepType, std::ratio<60>>;
using seconds      = std::chrono::duration<RepType, std::ratio<1>>;
using milliSeconds = std::chrono::duration<RepType, std::milli>;
using microSeconds = std::chrono::duration<RepType, std::micro>;
using nanoSeconds  = std::chrono::duration<RepType, std::nano>;
using picoSeconds  = std::chrono::duration<RepType, std::pico>;
using femtoSeconds = std::chrono::duration<RepType, std::femto>;

#if defined (_WIN32) || defined (_WIN64)

// FILETIME has a resolution of 100-nanoseconds
// https://msdn.microsoft.com/de-de/library/windows/desktop/ms724284(v=vs.85).aspx
using filetimeDuration = std::chrono::duration<std::int64_t, std::ratio<1, 10'000'000>>;

#endif


namespace literals {


/**
 * @brief Literal operator for femtoseconds.
 * @param v the count.
 */
constexpr femtoSeconds operator"" _fs(long double v)
{
  return femtoSeconds(v);
}

constexpr femtoSeconds operator"" _fs(unsigned long long v)
{
  return femtoSeconds(v);
}

/**
 * @brief Literal operator for picoseconds.
 * @param v the count.
 */
constexpr picoSeconds operator"" _ps(long double v)
{
  return picoSeconds(v);
}

constexpr nanoSeconds operator"" _ps(unsigned long long v)
{
  return picoSeconds(v);
}

/**
 * @brief Literal operator for nanoseconds.
 * @param v the count.
 */
constexpr nanoSeconds operator"" _ns(long double v)
{
  return nanoSeconds(v);
}

constexpr nanoSeconds operator"" _ns(unsigned long long v)
{
  return nanoSeconds(v);
}

/**
 * @brief Literal operator for microseconds.
 * @param v the count.
 */
constexpr microSeconds operator"" _us(long double v)
{
  return microSeconds(v);
}

constexpr microSeconds operator"" _us(unsigned long long v)
{
  return microSeconds(v);
}

/**
 * @brief Literal operator for milliseconds.
 * @param v the count.
 */
constexpr milliSeconds operator"" _ms(long double v)
{
  return milliSeconds(v);
}

constexpr milliSeconds operator"" _ms(unsigned long long v)
{
  return milliSeconds(v);
}

/**
 * @brief Literal operator for seconds.
 * @param v the count.
 */
constexpr seconds operator"" _s(long double v)
{
  return seconds(v);
}

constexpr seconds operator"" _s(unsigned long long v)
{
  return seconds(v);
}

/**
 * @brief Literal operator for minutes.
 * @param v the count.
 */
constexpr minutes operator"" _min(long double v)
{
  return minutes(v);
}

constexpr minutes operator"" _min(unsigned long long v)
{
  return minutes(v);
}

/**
 * @brief Literal operator for hours.
 * @param v the count.
 */
constexpr hours operator"" _h(long double v)
{
  return hours(v);
}

constexpr hours operator"" _h(unsigned long long v)
{
  return hours(v);
}


}   // namespace literals


/** 
 * @struct DurationString.
 * @brief  DurationString provides a string representation of the unit
 *         of a given duration
 */
template <typename Duration, typename CharT>
struct DurationString {
  static constexpr const CharT* value = DurationStringImpl<Duration::period, CharT>::value;
};


template <typename Ratio, typename CharT>
struct DurationStringImpl;

template <>
struct DurationStringImpl<std::ratio<604'800>, wchar_t> {
  static constexpr const wchar_t* value = L"week";
};

template <>
struct DurationStringImpl<std::ratio<604'800>, char> {
  static constexpr const char* value = "week";
};

template <>
struct DurationStringImpl<std::ratio<86'400>, wchar_t> {
  static constexpr const wchar_t* value = L"d";
};

template <>
struct DurationStringImpl<std::ratio<86'400>, char> {
  static constexpr const char* value = "d";
};

template <>
struct DurationStringImpl<std::ratio<3600>, wchar_t> {
  static constexpr const wchar_t* value = L"h";
};

template <>
struct DurationStringImpl<std::ratio<3600>, char> {
  static constexpr const char* value = "h";
};

template <>
struct DurationStringImpl<std::ratio<60>, wchar_t> {
  static constexpr const wchar_t* value = L"min";
};

template <>
struct DurationStringImpl<std::ratio<60>, char> {
  static constexpr const char* value = "min";
};

template <>
struct DurationStringImpl<std::ratio<1>, wchar_t> {
  static constexpr const wchar_t* value = L"s";
};

template <>
struct DurationStringImpl<std::ratio<1>, char> {
  static constexpr const char* value = "s";
};

template <>
struct DurationStringImpl<std::milli, wchar_t> {
  static constexpr const wchar_t* value = L"ms";
};

template <>
struct DurationStringImpl<std::milli, char> {
  static constexpr const char* value = "ms";
};

template <>
struct DurationStringImpl<std::micro, wchar_t> {
  static constexpr const wchar_t* value = L"mus";
};

template <>
struct DurationStringImpl<std::micro, char> {
  static constexpr const char* value = "mus";
};

template <>
struct DurationStringImpl<std::ratio<1, 10'000'000>, wchar_t> {
  static constexpr const wchar_t* value = L"NT-duration";
};

template <>
struct DurationStringImpl<std::ratio<1, 10'000'000>, char> {
  static constexpr const char* value = "NT-duration";
};

template <>
struct DurationStringImpl<std::nano, wchar_t> {
  static constexpr const wchar_t* value = L"ns";
};

template <>
struct DurationStringImpl<std::nano, char> {
  static constexpr const char* value = "ns";
};

template <>
struct DurationStringImpl<std::pico, wchar_t> {
  static constexpr const wchar_t* value = L"ps";
};

template <>
struct DurationStringImpl<std::pico, char> {
  static constexpr const char* value = "ps";
};

template <>
struct DurationStringImpl<std::femto, wchar_t> {
  static constexpr const wchar_t* value = L"fs";
};

template <>
struct DurationStringImpl<std::femto, char> {
  static constexpr const char* value = "fs";
};


/** 
 * @brief Convenience method that returns the unit representation
 *        for a given duration.
 */
template <typename Duration, typename CharT = wchar_t>
constexpr auto unitString(Duration)
{
  return DurationString<Duration, CharT>::value;
}


}   // namespace benchmark


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DURATIONTYPES_H_331924140113621812518610291861684095438323 
