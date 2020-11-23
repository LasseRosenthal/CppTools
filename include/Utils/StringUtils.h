/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringUtils.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.11.2020 
 */ 
 
#ifndef STRINGUTILS_H_293897259301651040819803304511412113441 
#define STRINGUTILS_H_293897259301651040819803304511412113441 
 
 
// includes
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
//#include <boost/algorithm/string.hpp>
#include <C:\Users\lasse\Documents\Programme\boost_1_74_0\boost\algorithm\string.hpp>
//#include <D:/Programs/boost_1_74_0/boost/algorithm/string.hpp>

namespace stringUtils {


template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto split(std::basic_string<CharT, CharTraits> const& s) -> std::vector<std::basic_string<CharT, CharTraits>>
{
//  using StringT = std::basic_string<CharT, CharTraits>;
  std::basic_istringstream<CharT, CharTraits> istr(s);
  return std::vector<std::basic_string<CharT, CharTraits>>{std::istream_iterator<std::basic_string<CharT, CharTraits>, CharT, CharTraits>{istr},
                                                           std::istream_iterator<std::basic_string<CharT, CharTraits>, CharT, CharTraits>{}};
}

inline auto split2(std::string const& s) -> std::vector<std::string>
{
  std::istringstream str(s);
  return std::vector<std::string>{std::istream_iterator<std::string>{str},
                                  std::istream_iterator<std::string>{}};
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

template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto split4(std::basic_string<CharT, CharTraits> const& s, CharT delimiter) -> std::vector<std::basic_string<CharT, CharTraits>>
{
  using StringT = std::basic_string<CharT, CharTraits>;
  StringT token;
  std::basic_istringstream<CharT, CharTraits> istr(s);
  std::vector<StringT> tokens;
  tokens.reserve(std::count(s.begin(), s.end(), delimiter));
  while(std::getline(istr, token, delimiter))
  {
    tokens.push_back(std::move(token));
  }
  return tokens;
}

template <typename CharT, typename CharTraits = std::char_traits<CharT>>
inline auto split5(std::basic_string<CharT, CharTraits> const& s, CharT delimiter) -> std::vector<std::basic_string<CharT, CharTraits>>
{
  using StringT = std::basic_string<CharT, CharTraits>;
  std::vector<StringT> tokens;
  tokens.reserve(std::count(s.begin(), s.end(), delimiter));
  boost::split(tokens, s, [delimiter](auto c){return c == delimiter;});
  return tokens;
}




}   // namespace stringUtils


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGUTILS_H_293897259301651040819803304511412113441 
