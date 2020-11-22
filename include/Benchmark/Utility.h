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

#include <chrono>
#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
#  include <Windows.h>
#endif


namespace benchmark {

#if defined (_WIN32) || defined (_WIN64)

// ntToUnixEpoch specifies the number of seconds between January 1, 1601 (NT epoch) and January 1, 1970 (Unix epoch):
constexpr std::chrono::duration<std::int64_t> ntToUnixEpoch{INT64_C(-11'644'473'600)};


/** 
 * @brief  Converts a FILETIME structure to the time point of a given clock.
 * @remark the clock has to be based on the UNIX epoch.
 * @see    https://github.com/HowardHinnant/date/wiki/Examples-and-Recipes#fILETIME
 */
template <typename ClockType = std::chrono::system_clock>
auto FILETIMEToTimePoint (FILETIME fileTime) -> typename ClockType::time_point
{
  using timePoint = typename ClockType::time_point;
  const filetimeDuration asDuration{static_cast<std::int64_t>(
    (static_cast<std::uint64_t>(fileTime.dwHighDateTime) << 32) | fileTime.dwLowDateTime)};

  const auto withUnixEpoch = asDuration + ntToUnixEpoch;
  return timePoint{std::chrono::duration_cast<typename ClockType::duration>(withUnixEpoch)};
}

#endif


}   // namespace benchmark


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UTILITY_H_2890112451222626043264514810562212310279 
