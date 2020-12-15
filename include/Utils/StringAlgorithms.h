/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringAlgorithms.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.12.2020 
 */ 
 
#ifndef STRINGALGORITHMS_H_8056167151199719446262236186154101186924390 
#define STRINGALGORITHMS_H_8056167151199719446262236186154101186924390 
 
 
// includes
#include <Utils/miscellaneous.h>
#include <boost/algorithm/string.hpp>

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace stringAlgorithms {


/** 
 * @brief casts a wstring to an integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, int>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stoi(s);
}

/** 
 * @brief casts a string to an integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, int>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stoi(s);
}

/** 
 * @brief casts a wstring to a long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, long>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stol(s);
}

/** 
 * @brief casts a string to a long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, long>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stol(s);
}

/** 
 * @brief casts a wstring to an unsigned long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, unsigned long>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stoul(s);
}

/** 
 * @brief casts a string to an unsigned long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, unsigned long>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stoul(s);
}

/** 
 * @brief casts a wstring to a long long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, long long>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stoll(s);
}

/** 
 * @brief casts a string to a long long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, long long>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stoll(s);
}

/** 
 * @brief casts a wstring to an unsigned long long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, unsigned long long>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stoull(s);
}

/** 
 * @brief casts a string to an unsigned long long integer
 */
template <typename T, std::enable_if_t<std::is_same_v<T, unsigned long long>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stoull(s);
}

/** 
 * @brief casts a wstring to a float
 */
template <typename T, std::enable_if_t<std::is_same_v<T, float>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stof(s);
}

/** 
 * @brief casts a string to a float
 */
template <typename T, std::enable_if_t<std::is_same_v<T, float>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stof(s);
}

/** 
 * @brief casts a wstring to a double
 */
template <typename T, std::enable_if_t<std::is_same_v<T, double>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stod(s);
}

/** 
 * @brief casts a string to a double
 */
template <typename T, std::enable_if_t<std::is_same_v<T, double>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stod(s);
}

/** 
 * @brief casts a wstring to a long double
 */
template <typename T, std::enable_if_t<std::is_same_v<T, long double>, void**> = nullptr>
inline auto lexicalCast(std::wstring const& s) -> T
{
  return std::stold(s);
}

/** 
 * @brief casts a string to a long double
 */
template <typename T, std::enable_if_t<std::is_same_v<T, long double>, void**> = nullptr>
inline auto lexicalCast(std::string const& s) -> T
{
  return std::stold(s);
}

/** 
 * @brief casts an arithmetic value to a string
 */
template <typename CharT, typename T, typename CharTraits = std::char_traits<CharT>,
          std::enable_if_t<std::is_arithmetic_v<T>, void**> = nullptr> 
inline auto lexicalCast(T value) -> std::basic_string<CharT, CharTraits>
{
  std::basic_stringstream<CharT, CharTraits> sstr;
  sstr << std::fixed << std::setprecision(cpptools::numberOfDecimalPlaces(value)) << value;
  return sstr.str();
}

/** 
 * @brief casts a boolean value to a string
 */
template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto lexicalCast(bool b) -> std::basic_string<CharT, CharTraits>
{
  std::basic_stringstream<CharT, CharTraits> sstr;
  sstr << std::boolalpha << b;
  return sstr.str();
}

/** 
 * @brief casts a string representing a boolean value to a boolean.
 */
template <typename T, std::enable_if_t<std::is_same_v<T, bool>, void**> = nullptr>
auto lexicalCast(std::string const& valString) -> T
{
  return valString == "true" ? true : false;
}

/** 
 * @brief casts a wstring representing a boolean value to a boolean.
 */
template <typename T, std::enable_if_t<std::is_same_v<T, bool>, void**> = nullptr>
auto lexicalCast(std::wstring const& valString) -> T
{
  return valString == L"true" ? true : false;
}

/** 
 * @brief "casts" a wstring to a wstring. Can be useful for generic algorithms.
 */
template <typename T, std::enable_if_t<std::is_same_v<T, std::wstring>, void**> = nullptr>
auto lexicalCast(std::wstring const& valString) -> T
{
  return valString;
}

/** 
 * @brief "casts" a string to a string. Can be useful for generic algorithms.
 */
template <typename T, std::enable_if_t<std::is_same_v<T, std::string>, void**> = nullptr>
auto lexicalCast(std::string const& valString) -> T
{
  return valString;
}


/** 
 * @brief 
 */
template <typename CharT, typename Predicate, typename CharTraits = std::char_traits<CharT>>
inline auto split(std::basic_string<CharT, CharTraits> const& s, Predicate&& pred) -> std::vector<std::basic_string<CharT, CharTraits>>
{
  if(s.empty()) return {};

  using StringT = std::basic_string<CharT, CharTraits>;

  const auto numDelim = std::count_if(s.begin(), s.end(), std::forward<Predicate>(pred));
  std::vector<StringT> tokens;
  tokens.reserve(numDelim + 1);

  const auto end = s.end();
  auto pos1 = std::find_if_not(s.begin(), end, std::forward<Predicate>(pred));
  auto pos2 = std::find_if(pos1, end, std::forward<Predicate>(pred));
  while(pos2 != end)
  {
    tokens.emplace_back(pos1, pos2);

    pos1 = std::find_if_not(pos2, end, std::forward<Predicate>(pred));
    pos2 = std::find_if(pos1, end, std::forward<Predicate>(pred));
  }

  if(pos1 != end)
  {
    tokens.emplace_back(pos1, pos2);
  }

  return tokens;
}


template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto split3(std::basic_string<CharT, CharTraits> const& s, CharT delimiter) -> std::vector<std::basic_string<CharT, CharTraits>>
{
  using StringT = std::basic_string<CharT, CharTraits>;
  StringT token;

  std::vector<StringT> tokens;
  tokens.reserve(std::count(s.begin(), s.end(), delimiter));

  typename StringT::size_type pos1 = 0ULL;
  typename StringT::size_type pos2 = s.find(delimiter, 0ULL);
  while(pos2 != StringT::npos)
  {
    tokens.push_back(s.substr(pos1, pos2 - pos1));
    pos1 = pos2 + 1ULL;
    pos2 = s.find(delimiter, pos1);
  }
  tokens.push_back(s.substr(pos1, pos2 - pos1));

  return tokens;
}


}   // namespace stringAlgorithms


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGALGORITHMS_H_8056167151199719446262236186154101186924390 
