/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    WinHighResClock.h 
 * @brief   WinHighResClock is a high resolution clock based on the QueryPerformanceCounter method
 *          provided by Windows. It fulfills all requirements of clocks as defined in the standard.
 * @see     see https://en.cppreference.com/w/cpp/named_req/Clock for the requirements of clocks.
 * @author  Lasse Rosenthal 
 * @date    18.08.2020 
 */ 
 
#ifndef WINHIGHRESCLOCK_H_8976200581756616922731314071175402891710184 
#define WINHIGHRESCLOCK_H_8976200581756616922731314071175402891710184 
 
 
// includes
#include <Benchmark/Utility.h>
 
 
namespace benchmark {


/** 
 * @class WinHighResClock 
 * @brief WinHighResClock is a high resolution clock based on the QueryPerformanceCounter method
 *        provided by Windows.
 * @see   see https://en.cppreference.com/w/cpp/named_req/Clock for the requirements of a clock.
 */
class WinHighResClock {

public:

  // ---------------------------------------------------
  // public types
  using clockType  = std::chrono::system_clock;
  using rep        = clockType::rep;
  using period     = clockType::period;
  using duration   = clockType::duration;
  using time_point = clockType::time_point;

  // ---------------------------------------------------
  // member constants
  static const bool is_steady = true;

  // static member functions
  static auto now       () -> time_point;
  static auto to_time_t (time_point const& tp) -> std::time_t;

private:

  // ---------------------------------------------------
  // auxiliary functions
  static auto getSynchronizedFileTime () -> FILETIME;
  
  // ---------------------------------------------------
  // static data
  class ReferenceTimePoint;
  static ReferenceTimePoint referenceTimePoint;
};


/**
 * @brief Returns a time point representing the current point in time.
 */
inline auto WinHighResClock::now() -> time_point
{
  const auto fileTime = getSynchronizedFileTime();
  return FILETIMEToTimePoint<clockType>(fileTime);
}

/**
 * @brief Converts a given time point to a std::time_t type.
 */
inline auto WinHighResClock::to_time_t(time_point const& tp) -> std::time_t
{
  return clockType::to_time_t(tp);
}


}   // namespace benchmark


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // WINHIGHRESCLOCK_H_8976200581756616922731314071175402891710184 
