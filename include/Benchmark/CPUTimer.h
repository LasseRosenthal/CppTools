/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/** 
 * @file    CPUTimer.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    02.06.2021 
 */

#ifndef CPUTIMER_H_109473235583331084496512773196562769212692
#define CPUTIMER_H_109473235583331084496512773196562769212692


// includes
#include <Benchmark/DurationTypes.h>
#include <Benchmark/WinHighResClock.h>

#include <Windows.h>


namespace benchmark {


/**
 * @class CPUTimer
 * @brief
 */
class CPUTimer {

public:

  // ---------------------------------------------------
  // public types
  using duration = std::chrono::microseconds;

  struct CPUProcessTimes {
    duration kernelDuration;
    duration userDuration;
    duration totalDuration;
  };

  // ---------------------------------------------------
  // ctor & dtor
  CPUTimer();

  // ---------------------------------------------------
  // public methods
  auto getCPUTimes       () const -> CPUProcessTimes;
  auto getProcessStartTM () const noexcept -> std::tm const*;

private:

  // ---------------------------------------------------
  // member types
  using clockType = std::chrono::system_clock;

  struct CPUProcessFileTimes {
    FILETIME creationTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
  };

  // ---------------------------------------------------
  // data
  std::tm             timeInfoProcessStart;
  CPUProcessFileTimes procTimesAtStart;

  // ---------------------------------------------------
  // auxiliary functions
  auto queryCPUProcessFileTimes (CPUProcessFileTimes&) const -> bool;
};

/**
 * @brief Returns a pointer to a tm structure stroring the time point of the process start.
 */
inline auto CPUTimer::getProcessStartTM() const noexcept -> std::tm const*
{
  return &timeInfoProcessStart;
}

/**
 * @brief Retrieves the process time information from the current process.
 */
inline bool CPUTimer::queryCPUProcessFileTimes(CPUProcessFileTimes& procTimes) const
{
  return GetProcessTimes(GetCurrentProcess(), &procTimes.creationTime, &procTimes.exitTime,
                         &procTimes.kernelTime, &procTimes.userTime) != -1;
}

/**
 * @brief operator- for CPUTimer::CPUProcessTimes.
 */
inline CPUTimer::CPUProcessTimes operator-(CPUTimer::CPUProcessTimes const& left,
                                           CPUTimer::CPUProcessTimes const& right)
{
  CPUTimer::CPUProcessTimes result;
  result.kernelDuration = left.kernelDuration - right.kernelDuration;
  result.userDuration   = left.userDuration - right.userDuration;
  result.totalDuration  = left.totalDuration - right.totalDuration;

  return result;
}


}   // namespace benchmark


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif   // CPUTIMER_H_109473235583331084496512773196562769212692
