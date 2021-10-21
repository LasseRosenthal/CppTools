/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Utility.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    18.08.2020 
 */ 
 
#ifndef UTILITY_H_2890112451222626043264514810562212310279 
#define UTILITY_H_2890112451222626043264514810562212310279 
 
 
// includes
#include <Benchmark/DurationTypes.h>
#include <Meta/Utility.h>

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>
#include <type_traits>

#if defined(_WIN32) || defined(_WIN64)
#  include <Windows.h>
#endif


namespace benchmark {


#if defined (_WIN32) || defined (_WIN64)


/**
 * @brief ntToUnixEpoch specifies the number of seconds between January 1, 1601 (NT epoch) and January 1, 1970 (Unix epoch)
 */
constexpr std::chrono::duration<std::int64_t> ntToUnixEpoch{INT64_C(-11'644'473'600)};

/**
 * @brief Converts a FILETIME structure to a /link #filetimeDuration /endlink.
 */
constexpr auto filetimeToDuration(FILETIME fileTime) -> filetimeDuration
{
  return filetimeDuration{static_cast<std::int64_t>(
    (static_cast<std::uint64_t>(fileTime.dwHighDateTime) << 32) | fileTime.dwLowDateTime)};
}

/**
 * @brief  Converts a FILETIME structure to the time point of a given clock.
 * @remark the clock has to be based on the UNIX epoch.
 * @remark FILETIME is a structure representing the number of 100-nanosecond intervals
 *         since January 1, 1601 (UTC).
 * @see    https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
 * @see    https://github.com/HowardHinnant/date/wiki/Examples-and-Recipes#fILETIME
 */
template <typename ClockType = std::chrono::system_clock>
inline auto FILETIMEToTimePoint(FILETIME fileTime) -> typename ClockType::time_point
{
  using timePoint = typename ClockType::time_point;
  using duration = typename ClockType::duration;

  return timePoint{std::chrono::duration_cast<duration>(filetimeToDuration(fileTime) + ntToUnixEpoch)};
}

#endif


template <typename CharT>
auto convertToTimeString(std::tm const& time) -> std::basic_string<CharT>
{
  std::basic_stringstream<CharT> timeStrm;
  const auto colon = timeStrm.widen(':');
  timeStrm.fill(timeStrm.widen('0'));
  timeStrm.width(2);

  timeStrm << time.tm_hour << colon << time.tm_min << colon << time.tm_sec;

  return timeStrm.str();
}

/** 
 * @brief
 */
template <typename SliceDuration, typename SrcDuration>
auto sliceDuration(SrcDuration& srcDur) -> std::enable_if_t<std::is_floating_point_v<typename SliceDuration::rep>, SliceDuration>
{
  auto slice = std::chrono::duration_cast<SliceDuration>(srcDur);
  slice = SliceDuration(std::floor(slice.count()));
  srcDur -= slice;
  return slice;
}

/** 
 * @brief 
 */
template <typename SliceDuration, typename SrcDuration>
auto sliceDuration(SrcDuration& srcDur) -> std::enable_if_t<std::is_integral_v<typename SliceDuration::rep>, SliceDuration>
{
  const auto slice = std::chrono::duration_cast<SliceDuration>(srcDur);
  srcDur -= slice;
  return slice;
}

/** 
 * @brief 
 */
#if 0
template <typename... SliceDurations, typename SrcDuration>
void decomposeDuration(SrcDuration& d, SliceDurations& ... sliceDurations)
{
  std::tie(sliceDurations...) = benchmark::decomposeDuration<SliceDurations...>(d);
}

template <typename... SliceDurations, typename SrcDuration>
auto decomposeDuration(SrcDuration& d) -> std::tuple<SliceDurations...>
{
  std::tuple<SliceDurations...> t;
  detail::decomposeDurationImpl<0ULL, sizeof...(SliceDurations), SrcDuration, SliceDurations...>::slice(d, t);
  return t;
}


namespace detail {

template <std::size_t Index, std::size_t maxIndex, typename SrcDuration, typename... SliceDurations>
struct decomposeDurationImpl
{
  static void slice(SrcDuration& d, std::tuple<SliceDurations...>& t)
  {
    std::get<Index>(t) = benchmark::sliceDuration<meta::TypeOf<Index, SliceDurations...>>(d);
    decomposeDurationImpl<Index + 1ULL, maxIndex, SrcDuration, SliceDurations...>::slice(d, t);
  }
};

template <std::size_t maxIndex, typename SrcDuration, typename... SliceDurations>
struct decomposeDurationImpl<maxIndex, maxIndex, SrcDuration, SliceDurations...>
{
  static void slice(SrcDuration& d, std::tuple<SliceDurations...>& t) {}
};

}   // namespace detail
#endif


}   // namespace benchmark


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UTILITY_H_2890112451222626043264514810562212310279 
