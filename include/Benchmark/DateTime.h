/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    DateTime.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    07.06.2021 
 */ 
 
#ifndef DATETIME_H_11212200532119823447121126108269432903719346 
#define DATETIME_H_11212200532119823447121126108269432903719346 
 
 
// includes
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>


namespace date {
 

/** 
 * @struct Date 
 * @brief  Date is a structure holding a calendar date
 *         decomposed into its components.
 * @remark Date is a subset of std::tm.
 * @see    https://en.cppreference.com/w/cpp/chrono/c/tm
 */
struct Date {
  using value_type = std::size_t;
  value_type weekDay;  /// days since sunday 
  value_type monthDay; /// day of the month
  value_type yearDay;  /// current day of the year (1 - 366)
  value_type month;    /// month since January
  value_type year;     /// the year
};

/**
 * @struct Time 
 * @brief  Time is a structure holding a time point decomposed into its components.
 * @remark Time is a subset of std::tm adding a field for milliseconds.
 * @see    https://en.cppreference.com/w/cpp/chrono/c/tm
 */
struct Time {
  using value_type = std::size_t;
  value_type milliSeconds; /// milliseconds after the second
  value_type seconds;      /// seconds after the minute
  value_type minutes;      /// minutes aftre the hour
  value_type hours;        /// hours since midnight
};

/** 
 * @struct DateTime 
 * @brief  DateTime is a structure holding a calendar date and time
 *         decomposed into its components.
 * @remark DateTime is an extension of std::tm adding a field for milliseconds.
 * @see    https://en.cppreference.com/w/cpp/chrono/c/tm
 */
struct DateTime {
  Date date;
  Time time;
};

/** 
 * @brief Stream operator for Date.
 */
template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, Date const& date)
-> std::basic_ostream<CharT, CharTraits>&
{
  const auto sep = ostr.widen('-');
  ostr.fill(ostr.widen('0'));
  return ostr << std::setw(4) << date.year << sep
              << std::setw(2) << date.month << sep
              << std::setw(2) << date.monthDay;
};

/** 
 * @brief Stream operator for Time.
 */
template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, Time const& time)
-> std::basic_ostream<CharT, CharTraits>&
{
  const auto sep = ostr.widen(':');
  ostr.fill(ostr.widen('0'));
  return ostr << std::setw(2) << time.hours << sep
              << std::setw(2) << time.minutes << sep
              << std::setw(2) << time.seconds << ostr.widen('.') << time.milliSeconds;
};

/** 
 * @brief Converts a time_point to a std::tm object.
 */
template <typename ClockT>
auto timePointToTM(typename ClockT::time_point timePoint) -> std::tm
{
  const std::time_t timeT = ClockT::to_time_t(timePoint);
  std::tm timeStruc;
  localtime_s(&timeStruc, &timeT);
  return timeStruc;
};

/** 
 * @brief Converts a std::tm object into a Date object.
 */
inline auto tmToDate(std::tm const& timeStruc) noexcept -> Date
{
  Date date;
  date.weekDay  = timeStruc.tm_wday;
  date.monthDay = timeStruc.tm_mday;
  date.yearDay  = timeStruc.tm_yday + 1;
  date.month    = timeStruc.tm_mon + 1;
  date.year     = timeStruc.tm_year + 1900;
  return date;
};

/** 
 * @brief Converts a std::tm object into a Date object.
 */
inline auto tmToTime(std::tm const& timeStruc) noexcept -> Time
{
  Time time;
  time.milliSeconds = 0;
  time.seconds      = timeStruc.tm_sec;
  time.minutes      = timeStruc.tm_min;
  time.hours        = timeStruc.tm_hour;
  return time;
};

/** 
 * @brief Converts a time_point to a Date object.
 */
template <typename ClockT>
inline auto timePointToDate(typename ClockT::time_point timePoint) -> Date
{
  return tmToDate(timePointToTM<ClockT>(timePoint));
};

/** 
* @brief Converts a time_point to a Time object.
*/
template <typename ClockT>
auto timePointToTime(typename ClockT::time_point timePoint) -> Time
{
  const std::tm timeStruc = timePointToTM<ClockT>(timePoint);
  Time time = tmToTime(timeStruc);

  const auto secSinceEpoch    = std::chrono::time_point_cast<std::chrono::seconds>(timePoint);
  const auto milliSecFraction = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - secSinceEpoch);
  time.milliSeconds = milliSecFraction.count();

  return time;
};

/** 
 * @brief Recieves the current time_point from a given clock and converts it
 *        to a DateTime object.
 */
template <typename ClockT = std::chrono::system_clock>
auto getCurrentDateTime() -> DateTime
{
  const auto now = ClockT::now();
  DateTime dateTime;
  dateTime.date = timePointToDate<ClockT>(now);
  dateTime.time = timePointToTime<ClockT>(now);

  return dateTime;
};


}   // namespace date


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DATETIME_H_11212200532119823447121126108269432903719346 
