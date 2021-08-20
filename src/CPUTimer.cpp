/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/** 
 * @file    CPUTimer.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    02.06.2021 
 */

// includes
#include <Benchmark/CPUTimer.h>
#include <Benchmark/Utility.h>

#include <ctime>
#include <sstream>
#include <stdexcept>


namespace benchmark {


/**
 * @brief Retrieves the process time information from the current process.
 */
CPUTimer::CPUTimer()
{
  if(queryCPUProcessFileTimes(procTimesAtStart))
  {
    const auto tpCreation = FILETIMEToTimePoint<clockType>(procTimesAtStart.creationTime);
    const std::time_t ttCreation = clockType::to_time_t(tpCreation);

    localtime_s(&timeInfoProcessStart, &ttCreation);
  }
}

/**
 * @brief
 */
auto CPUTimer::getCPUTimes() const -> CPUProcessTimes
{
  CPUProcessFileTimes procFileTimes;
  if(queryCPUProcessFileTimes(procFileTimes))
  {
    CPUProcessTimes procTimes;
    procTimes.kernelDuration = std::chrono::duration_cast<duration>(filetimeToDuration(procFileTimes.kernelTime));
    procTimes.userDuration   = std::chrono::duration_cast<duration>(filetimeToDuration(procFileTimes.userTime));
    procTimes.totalDuration  = procTimes.kernelDuration + procTimes.userDuration;

    return procTimes;
  }

  const auto lastError = GetLastError();

  std::ostringstream errMsg;
  errMsg << "GetProcessTimes failed [error code : " << lastError << "]";
  throw std::runtime_error(errMsg.str());
}


}   // namespace benchmark


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //
