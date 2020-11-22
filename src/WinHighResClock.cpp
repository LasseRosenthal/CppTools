/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    WinHighResClock.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    18.08.2020 
 */ 
 
// includes 
#include <Benchmark/WinHighResClock.h>


namespace benchmark {


/** 
 * @struct ReferenceTimePoint.
 * @brief  ReferenceTimePoint wraps a FILETIME value and a LARGE_INTEGER counter.
 *         The counter is synchronized to the system time.
 */
class WinHighResClock::ReferenceTimePoint {

  // ---------------------------------------------------
  // grant friendship to the parent
  friend WinHighResClock;

  // ---------------------------------------------------
  // ctor & dtor
  ReferenceTimePoint  ();
  ReferenceTimePoint  (ReferenceTimePoint const&) = delete;
  ReferenceTimePoint  (ReferenceTimePoint&&) = delete;
  auto operator=      (ReferenceTimePoint const&) -> ReferenceTimePoint& = delete;
  auto operator=      (ReferenceTimePoint&&) -> ReferenceTimePoint& = delete;
  ~ReferenceTimePoint () = default;

  // ---------------------------------------------------
  // data
  FILETIME      fileTime;
  LARGE_INTEGER counter;
};

/**
 * @brief Default constructor. Synchronizes the FILETIME value and the counter retrieved by ::QueryPerformanceCounter.
 *        Using these two values as a reference, it is possible to calculate the current system time with a high resolution
 * @see   http://web.archive.org/web/20121018065403/http://msdn.microsoft.com/en-us/magazine/cc163996.aspx
 */
WinHighResClock::ReferenceTimePoint::ReferenceTimePoint()
{
  // Spin waiting for a change in system time.
  // Get the matching performance counter value for that time.
  FILETIME ft0 = {0, 0};
  ::GetSystemTimeAsFileTime(&ft0);
  do
  {
    ::GetSystemTimeAsFileTime(&fileTime);
    ::QueryPerformanceCounter(&counter);
  } while(ft0.dwHighDateTime == fileTime.dwHighDateTime &&
          ft0.dwLowDateTime == fileTime.dwLowDateTime);
}


// definition of static members
WinHighResClock::ReferenceTimePoint WinHighResClock::referenceTimePoint;
static LARGE_INTEGER WinPerfCtrFrequency{};
static const bool WINfrequencyAvailable           = ::QueryPerformanceFrequency(&WinPerfCtrFrequency) != 0;
static const ULONGLONG fileTimeIntervalsPerSecond = filetimeDuration::period::den;
static const double FileTimeTicksPerQPCTicks      = static_cast<double>(fileTimeIntervalsPerSecond) / WinPerfCtrFrequency.QuadPart;


/**
 * @brief  returns a high resolution FILETIME value representing the current system time.
 * @return a FILETIME value which is a 64-bit value representing the number of 100-nanosecond intervals
 *         since January 1, 1601 (UTC).
 * @see    https://msdn.microsoft.com/en-us/library/windows/desktop/ms724284(v=vs.85).aspx
 */
auto WinHighResClock::getSynchronizedFileTime() -> FILETIME
{
  // get the current value of the performance counter
  // and calculate the number of elapsed ticks since synchronization
  LARGE_INTEGER perfCtr;
  ::QueryPerformanceCounter(&perfCtr);

  LARGE_INTEGER ticksElapsed;
  ticksElapsed.QuadPart = perfCtr.QuadPart - referenceTimePoint.counter.QuadPart;

  // translate the number of elapsed ticks to 100-nanoseconds intervals (FILETIME resolution)
  // and add to reference FILETIME to get current FILETIME.
  ULARGE_INTEGER fileTimeTicks;
  fileTimeTicks.QuadPart = static_cast<ULONGLONG>(ticksElapsed.QuadPart * FileTimeTicksPerQPCTicks + 0.5);

  ULARGE_INTEGER fileTimeRef;
  fileTimeRef.HighPart = referenceTimePoint.fileTime.dwHighDateTime;
  fileTimeRef.LowPart = referenceTimePoint.fileTime.dwLowDateTime;
  fileTimeRef.QuadPart += fileTimeTicks.QuadPart;

  return {fileTimeRef.LowPart, fileTimeRef.HighPart};
}


}   // namespace benchmark


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
