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

#include <functional>
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
 * @brief  Compares two null-terminated byte strings lexicographically.
 * @return Negative value if s1 appears before s2 in lexicographical order.
 *         Zero if s1 and s2 compare equal.
 *         Positive value if s1 appears after s2 in lexicographical order.
 */
inline auto strComp(const char* const s1, const char* const s2) -> int
{
  return strcmp(s1, s2);
}

/** 
 * @brief Compares two null-terminated wide strings lexicographically.
 * @return Negative value if s1 appears before s2 in lexicographical order.
 *         Zero if s1 and s2 compare equal.
 *         Positive value if s1 appears after s2 in lexicographical order. 
 */
inline auto strComp(const wchar_t* const s1, const wchar_t* const s2) -> int
{
  return wcscmp(s1, s2);
}

/**
 * @brief   returns the number of characters of the given string s
 * @param   const StrT& s
 * @returns number of characters stored in s
 */
template <typename StrT>
inline auto strLength(StrT const& s) -> std::size_t
{
  return std::char_traits<typename StrT::value_type>::length(s.c_str());
}

/**
 * @brief   returns the number of characters of the given multibyte string s
 * @param   CharT const* c
 * @returns number of characters stored in c excluding terminating zero byte
 */
template <class CharT>
inline auto cstrLength(CharT const* const c) -> std::size_t
{
  return std::char_traits<CharT>::length(c);
}

/**
 * @brief     converts the given string s to multibyte c-string
 * @param[in] const std::(w)string& s
 * @returns   char* pointer containing the characters of s
 */
auto toMBstring(const std::string& s) -> char*;
auto toMBstring(const std::wstring& s) -> char*;

/**
 * @brief     converts the given string s to wide character c-string dest
 * @param[in] const std::(w)string& s
 * @returns   a wide character c-string containing the characters of s
 */
wchar_t* toWCstring(const std::string&);
wchar_t* toWCstring(const std::wstring&);

/**
 * converts the given std::wstring s to a std::string
 * @param   std::wstring const& s
 * @returns a std::string containing the characters of s
 */
inline auto wstringToString(std::wstring const& s) -> std::string
{
  std::unique_ptr<char[]> p{stringAlgorithms::toMBstring(s)};
  return std::string{p.get()};
}

/**
 * @brief   converts the given std::string s to std::wstring dest
 * @param   std::string const& s
 * @returns a std::wstring containing the characters of s
 */
inline auto stringToWstring(const std::string& s) -> std::wstring
{
  std::unique_ptr<wchar_t[]> p{stringAlgorithms::toWCstring(s)};
  return std::wstring{p.get()};
}

/**
 * @brief   checks if if a given string s1 begins with another string s2
 * @returns true, if s1 begins with s2. False otherwise.
 */
template <typename StrT>
inline auto beginsWith(StrT&& s1, StrT&& s2) -> bool
{
  const auto len1 = s1.length();
  const auto len2 = s2.length();
  if(len2 > len1)
  {
    return false;
  }
  return s1.compare(0ULL, len2, s2) == 0;
}

/**
 * @brief   checks if if a given string s1 ends with another string s2
 * @returns true, if s1 ends with s2. False otherwise.
 */
template <typename StrT>
inline auto endsWith(StrT&& s1, StrT&& s2) -> bool
{
  const auto len1 = s1.length();
  const auto len2 = s2.length();
  if(len2 > len1)
  {
    return false;
  }
  return s1.compare(len1 - len2, len2, s2) == 0;
}

/**
 * @brief returns the substring between two delimiters
 */
template <class StrT, typename Predicate>
auto substringBetweenDelimiters(StrT const& s, Predicate&& pred) -> StrT
{
  const auto end = s.end();
  const auto pos1 = std::find_if_not(s.begin(), end, std::forward<Predicate>(pred));
  if(pos1 == end)
  {
    return {};
  }

  const auto pos2 = std::find_if(pos1, end, std::forward<Predicate>(pred));
  if(pos2 == end)
  {
    return {};
  }

  return StrT{pos1, pos2};
}

/** 
 * @brief 
 */
template <typename StrT, typename Predicate>
auto split(StrT const& s, Predicate&& pred) -> std::vector<StrT>
{
  if(s.empty()) return {};

  const auto numDelim = std::count_if(s.begin(), s.end(), std::forward<Predicate>(pred));
  std::vector<StrT> tokens;
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

/**
 * @brief removes all characters from the left side of a given string that fullfill a certain predicat.
 */
template <typename StrT, typename Predicate>
auto cropLeft(StrT&& s, Predicate&& pred) -> StrT
{
  if(!s.empty())
  {
    if(const auto p = std::find_if_not(s.begin(), s.end(), std::forward<Predicate>(pred)); p != s.end())
    {
      s.erase(s.begin(), p);
    }
  }

  return std::forward<StrT>(s);
}

template <typename StrT, typename Predicate>
auto cropLeft(StrT const& s, Predicate&& pred) -> StrT
{
  StrT copy{s};
  return cropLeft(std::move(copy), std::forward<Predicate>(pred));
}

/**
 * @brief removes all characters from the right side of a given string that fullfill a certain predicat.
 */
template <typename StrT, typename Predicate>
auto cropRight(StrT&& s, Predicate&& pred) -> StrT
{
  if(!s.empty())
  {
    if(const auto p = std::find_if_not(s.rbegin(), s.rend(), std::forward<Predicate>(pred)); p != s.rend())
    {
      s.erase(p.base(), s.end());
    }
  }

  return std::forward<StrT>(s);
}

template <typename StrT, typename Predicate>
auto cropRight(StrT const& s, Predicate&& pred) -> StrT
{
  StrT copy{s};
  return cropRight(std::move(copy), std::forward<Predicate>(pred));
}

/**
 * @brief Adds a given character to the left of a given string until its length has reached a given limit.
 */
template <typename StrT>
auto padLeft(StrT&& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  if(const auto len = s.length(); len < totalLength)
  {
    s.reserve(totalLength);
    s.insert(0ULL, totalLength - len, c);
  }
  return std::forward<StrT>(s);
}

template <typename StrT>
auto padLeft(StrT& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  if(const auto len = s.length(); len < totalLength)
  {
    s.reserve(totalLength);
    s.insert(0ULL, totalLength - len, c);
  }
  return s;
}

template <typename StrT>
auto padLeft(StrT const& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  const auto len = s.length();
  if(len >= totalLength)
  {
    return s;
  }

  StrT retVal;
  retVal.reserve(totalLength);
  return retVal.append(totalLength - len, c).append(s);
}

/**
 * @brief Adds a given character to the right of a given string until its length has reached a given limit.
 */
template <typename StrT>
auto padRight(StrT&& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  if(const auto len = s.length(); len < totalLength)
  {
    s.reserve(totalLength);
    s.append(totalLength - len, c);
  }
  return std::forward<StrT>(s);
}

template <typename StrT>
auto padRight(StrT& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  if(const auto len = s.length(); len < totalLength)
  {
    s.reserve(totalLength);
    s.append(totalLength - len, c);
  }
  return s;
}

template <typename StrT>
auto padRight(StrT const& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  const auto len = s.length();
  if(len >= totalLength)
  {
    return s;
  }

  StrT retVal;
  retVal.reserve(totalLength);
  return retVal.append(s).append(totalLength - len, c);
}


template <typename StrT>
auto padBothSides(StrT&& s, typename StrT::value_type c, const std::size_t totalLength) -> StrT
{
  using CharT = typename StrT::value_type;
  const auto sLen = s.length();
  if(sLen >= totalLength)
  {
    return std::forward<StrT>(s);
  }
  else if(s.empty())
  {
    return StrT(totalLength, c);
  }

  s.reserve(totalLength);
  const auto PaddedLengthTotal = totalLength - sLen;
  const auto paddedLenLeft = PaddedLengthTotal / 2ULL;
  const auto paddedLenRight = PaddedLengthTotal % 2ULL == 0ULL ? paddedLenLeft : paddedLenLeft + 1;

  return padRight(padLeft(std::forward<StrT>(s), c, sLen + paddedLenLeft), c, sLen + paddedLenLeft + paddedLenRight);
}


}   // namespace stringAlgorithms


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGALGORITHMS_H_8056167151199719446262236186154101186924390 
