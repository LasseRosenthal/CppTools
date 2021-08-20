/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StopWatch.h 
 * @brief   Definition of a simple stop watch providing start and stop methods.
 * 
 * @author  Lasse Rosenthal 
 * @date    19.08.2020 
 */ 
 
#ifndef STOPWATCH_H_28749230959748295961992513353107243059318283 
#define STOPWATCH_H_28749230959748295961992513353107243059318283 
 

// includes
#include <Benchmark/DurationTypes.h>

#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

#if defined (_WIN32) || defined (_WIN64)
  #include "WinHighResClock.h"
#endif


namespace benchmark {


/** 
 * @class  StopWatch 
 * @brief  StopWatch is a simple benchmark class based on a clock that provides time stamps.
 * @remark the clock has to fulfill the requirements of a clock as defined in the standard.
 * @see    see https://en.cppreference.com/w/cpp/named_req/Clock for the requirements of a clock.
 */
#if defined (_WIN32) || defined (_WIN64)
  template <typename ClockT = WinHighResClock>
#else
  template <typename ClockT = std::chrono::high_resolution_clock>
#endif
class StopWatch {

public:

  // ---------------------------------------------------
  // public types
  using clockType  = ClockT;
  using duration   = benchmark::seconds;
  using rep        = typename duration::rep;
  using period     = typename duration::period;
  using time_point = typename clockType::time_point;

  /// Enumeration to specify the current state of the stop watch.
  enum class state : char {
    reset,   ///< indicates that the watch has not been started yet
    running, ///< the watch is currrently running
    stopped, ///< the watch has been stopped
  };

  // ---------------------------------------------------
  // c'tor & d'tor
  explicit StopWatch (std::wostream& = std::wcout, std::wstring = {});
  StopWatch          (StopWatch const&) = delete;
  StopWatch          (StopWatch&&) = delete;
  auto operator=     (StopWatch const&) -> StopWatch& = delete;
  auto operator=     (StopWatch&&) -> StopWatch& = delete;
  ~StopWatch         () = default;

  // ---------------------------------------------------
  // public methods
  void start          ();
  void stop           ();
  void reset          () noexcept;
  template <typename Duration = duration>
  auto getDuration    () const -> Duration;
  auto now            () const -> time_point;
  auto getState       () const noexcept -> state;
  auto loggingMessage () const -> std::wstring const&;
  template <typename Duration = duration>
  void log            (int precision = 3);

private:

  // ---------------------------------------------------
  // private data
  time_point     startTimePoint;
  time_point     stopTimePoint;
  state          currState {state::reset};
  std::wstring   logMessage {L"StopWatch"};
  std::wostream& logStream {std::wcout};
};


/**
 * @brief Constructor.
 * @param loggingStrm the ostream object used for logging.
 * @param logMsg activity message for the current measurement.
 */
template <typename ClockT>
inline StopWatch<ClockT>::StopWatch(std::wostream& logStrm, std::wstring logMsg)
  : logStream  {logStrm}
  , logMessage {std::move(logMsg)}
{}

/**
 * @brief Starts a measurement.
 * @param logMsg  activity message for the current measurement.
 */
template <typename ClockT>
inline void StopWatch<ClockT>::start()
{
  currState      = state::running;
  startTimePoint = ClockT::now();
}

/**
 * @brief stops the current measurement.
 */
template <typename ClockT>
inline void StopWatch<ClockT>::stop()
{
  stopTimePoint = ClockT::now();
  currState     = state::stopped;
}

/**
 * @brief Resets the state of the timer.
 */
template <typename ClockT>
void StopWatch<ClockT>::reset() noexcept
{
  startTimePoint = time_point{};
  stopTimePoint  = time_point{};
  currState      = state::reset;
}

/**
 * @brief If the stop watch is still running the elapsed time since start is returned.
 *        If the stop watch has been stopped, the elapsed between start and stop is returned.
 */
template <typename ClockT>
template <typename Duration>
auto StopWatch<ClockT>::getDuration() const -> Duration
{
  if(state::stopped == currState || state::reset == currState)
  {
    return std::chrono::duration_cast<Duration>(stopTimePoint - startTimePoint);
  }

  const auto timePointNow = ClockT::now();
  return std::chrono::duration_cast<Duration>(timePointNow - startTimePoint);
}

/**
 * @brief returns the current time as a time_point.
 */
template <typename ClockT>
inline auto StopWatch<ClockT>::now() const -> time_point
{
  return ClockT::now();
}

/**
 * @brief   Simple getter. Returns the current /link #state /endlink of the clock
 * @returns currState.
 */
template <typename ClockT>
inline auto StopWatch<ClockT>::getState() const noexcept -> state
{
  return currState;
}

/**
 * @brief   Simple getter. Returns the activity message for the current measurement.
 * @returns logMessage.
 */
template <typename ClockT>
inline auto StopWatch<ClockT>::loggingMessage() const -> std::wstring const&
{
  return logMessage;
}

/**
 * @brief Writes the results of the current measurement into the logging stream.
 */
template <typename ClockT>
template <typename Duration>
void StopWatch<ClockT>::log(int precision)
{
  const auto oldPrecision = logStream.precision();
  const auto currentFlags = logStream.flags();

  logStream << L"benchmark : " << getDuration<Duration>().count() << DurationString<Duration, wchar_t>
            << L" [" << logMessage << L"]\n";

  logStream.setf(currentFlags);
  logStream.precision(oldPrecision);
}


}   // namespace benchmark


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STOPWATCH_H_28749230959748295961992513353107243059318283 
