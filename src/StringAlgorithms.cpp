/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringAlgorithms.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    16.12.2020 
 */ 
 
// includes 
#include <Utils/StringAlgorithms.h>

#include <cstring>
#include <new>


namespace stringAlgorithms {


// string -> char*
auto toMBstring(std::string_view s) -> char*
{
  const auto strLength = s.length();
  const auto bufferSize = strLength + 1ULL;
  auto* const dest = new(std::nothrow) char[bufferSize];

  if(dest != nullptr)
  {
    if(0ULL == strLength)
    {
      *dest = '\0';
    }
    else
    {
      strncpy_s(dest, bufferSize, s.data(), strLength);
      dest[strLength] = '\0';
    }
  }

  return dest;
}

// wstring -> char*
auto toMBstring(std::wstring_view s) -> char*
{
  const auto strLength = s.length();
  const auto bufferSize = strLength + 1ULL;
  auto* const dest = new(std::nothrow) char[bufferSize];

  if(dest != nullptr)
  {
    if(0ULL == strLength)
    {
      *dest = '\0';
    }
    else
    {
      std::size_t retval;
      wcstombs_s(&retval, dest, bufferSize, s.data(), strLength);
      dest[strLength] = '\0';
    }
  }

  return dest;
}

// string -> wchar_t*
auto toWCstring(std::string_view s) -> wchar_t*
{
  const auto strLength  = s.length();
  const auto bufferSize = strLength + 1ULL;
  auto* const dest = new(std::nothrow) wchar_t[bufferSize];

  if(dest != nullptr)
  {
    if(0ULL == strLength)
    {
      *dest = L'\0';
    }
    else
    {
      std::size_t retval;
      mbstowcs_s(&retval, dest, bufferSize, s.data(), strLength);
      dest[strLength] = L'\0';
    }
  }

  return dest;
}

// wstring -> wchar_t*
auto toWCstring(std::wstring_view s) -> wchar_t*
{
  const auto strLength = s.length();
  const auto bufferSize = strLength + 1ULL;
  auto* const dest = new(std::nothrow) wchar_t[bufferSize];

  if(dest != nullptr)
  {
    if(0ULL == strLength)
    {
      *dest = L'\0';
    }
    else
    {
      wcscpy_s(dest, bufferSize, s.data());
      dest[strLength] = L'\0';
    }
  }

  return dest;
}

auto beginsWith(std::string_view s1, std::string_view s2) -> bool
{
  const auto length1 = s1.length();
  const auto length2 = s2.length();
  if(length2 > length1)
  {
    return false;
  }
  return s1.compare(0ULL, length2, s2) == 0;
}

auto beginsWith(std::wstring_view s1, std::wstring_view s2) -> bool
{
  const auto length1 = s1.length();
  const auto length2 = s2.length();
  if(length2 > length1)
  {
    return false;
  }
  return s1.compare(0ULL, length2, s2) == 0;
}

auto endsWith(std::string_view s1, std::string_view s2) -> bool
{
  const auto length1 = s1.length();
  const auto length2 = s2.length();
  if(length2 > length1)
  {
    return false;
  }
  return s1.compare(length1 - length2, length2, s2) == 0;
}

auto endsWith(std::wstring_view s1, std::wstring_view s2) -> bool
{
  const auto length1 = s1.length();
  const auto length2 = s2.length();
  if(length2 > length1)
  {
    return false;
  }
  return s1.compare(length1 - length2, length2, s2) == 0;
}


}   // namespace stringAlgorithms


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
