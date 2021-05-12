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
  const auto len  = s.length() + 1ULL;
  char*      dest = new(std::nothrow) char[len];

  if(dest != nullptr)
  {
    if(1ULL == len)
    {
      *dest = '\0';
    }
    else
    {
      strncpy_s(dest, len, s.data(), len);
    }
  }
  return dest;
}

// wstring -> char*
auto toMBstring(std::wstring_view s) -> char*
{
  const auto len  = s.length() + 1ULL;
  char*      dest = new(std::nothrow) char[len];
  if(dest != nullptr)
  {
    if(1ULL == len)
    {
      *dest = '\0';
    }
    else
    {
      std::size_t retval;
      wcstombs_s(&retval, dest, len, s.data(), len);
    }
  }
  return dest;
}

// string -> wchar_t*
auto toWCstring(std::string_view s) -> wchar_t*
{
  const auto len  = s.length() + 1ULL;
  wchar_t*   dest = new(std::nothrow) wchar_t[len];
  if(dest != nullptr)
  {
    if(1ULL == len)
    {
      *dest = L'\0';
    }
    else
    {
      std::size_t retval;
      mbstowcs_s(&retval, dest, len, s.data(), len);
    }
  }
  return dest;
}

// wstring -> wchar_t*
auto toWCstring(std::wstring_view s) -> wchar_t*
{
  const auto len  = s.length() + 1ULL;
  wchar_t*   dest = new(std::nothrow) wchar_t[len];
  if(dest != nullptr)
  {
    if(1ULL == len)
    {
      *dest = L'\0';
    }
    else
    {
      wcscpy_s(dest, len, s.data());
    }
  }
  return dest;
}


}   // namespace stringAlgorithms


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
