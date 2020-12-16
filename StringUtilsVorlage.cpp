// *****************************************************************************
//      project :
//         file :  stringUtils.cpp
//
//  Copyright (c) 2016 Heidelberger Druckmaschinen AG
//  Reproduction or disclosure of this file or its contents without the prior
//  written consent of Heidelberger Druckmaschinen AG is prohibited.
// -----------------------------------------------------------------------------
//  description :
// *****************************************************************************
//
//  environment :  omake
//
// =============================================================================
//  date       | version |  author           |  description
// ------------+---------+-------------------+----------------------------------
//
//  02.05.2016     0.1       L. Rosenthal       created
// *****************************************************************************

// ---------------------------------------------------
// includes
// ---------------------------------------------------
#if 0
#include "stringUtils.h"


// namespace stringUtils
namespace stringUtils {

// ---------------------------------------------------------------------------------- //
// conversion functions
// ---------------------------------------------------------------------------------- //


// wchar -> string
template <>
std::string cstrToString(const wchar_t* src)
{
  const size_t len = std::wcslen(src) + 1u;
  if(len == 1u)
  {
    return std::string();
  }
  char* dest = new char[len];
  std::wcstombs(dest, src, len);
  std::string s(dest);
  delete[] dest;
  return s;
}

// char -> wstring
template <>
std::wstring cstrToString(const char* src)
{
  const size_t len = std::strlen(src) + 1u;
  if(len == 1u)
  {
    return std::wstring();
  }
  wchar_t* dest = new wchar_t[len];
  std::mbstowcs(dest, src, len);
  std::wstring ret(dest);
  delete[] dest;
  return ret;
}

// string -> char*
char* toMBstring(const std::string& s)
{
  const auto len = strLength(s) + 1u;
  char*      dest = new(std::nothrow) char[len];

  if(dest)
  {
    if(len == 1u)
    {
      *dest = '\0';
    }
    else
    {
      std::strncpy(dest, s.c_str(), len);
    }
  }
  return dest;
}

// wstring -> char*
char* toMBstring(const std::wstring& s)
{
  const auto len = strLength(s) + 1u;
  char*      dest = new(std::nothrow) char[len];

  if(dest)
  {
    if(len == 1u)
    {
      *dest = '\0';
    }
    else
    {
      std::wcstombs(dest, s.c_str(), len);
    }
  }
  return dest;
}

// string -> wchar_t*
wchar_t* toWCstring(const std::string& s)
{
  const auto len  = strLength(s) + 1u;
  wchar_t*   dest = new wchar_t[len];
  if(len == 1u)
  {
    *dest = L'\0';
  }
  else
  {
    std::mbstowcs(dest, s.c_str(), len);
  }
  return dest;
}

// wstring -> wchar_t*
wchar_t* toWCstring(const std::wstring& s)
{
  const auto len  = strLength(s) + 1u;
  wchar_t*   dest = new wchar_t[len];
  if(len == 1u)
  {
    *dest = L'\0';
  }
  else
  {
    std::wcscpy(dest, s.c_str());
  }
  return dest;
}


//template <>
//inline char* toMBstring (const std::string& s)
//{
//  const size_t len = strLength (s) + 1u;
//  if (len == 1u)
//  {
//    return nullptr;
//  }
//  char* dest = new char[len];
//  std::strcpy (dest, s.c_str() );
//  return dest;
//}
//
//// wstring -> char*
//template <>
//inline char* toMBstring (const std::wstring& s)
//{
//  const size_t len = strLength (s) + 1u;
//  if (len == 1u)
//  {
//    return nullptr;
//  }
//  char* dest = new char[len];
//  std::wcstombs (dest, s.c_str(), len);
//  return dest;
//}

}   // namespace stringUtils
#endif

// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //
