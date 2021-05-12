/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    OSUtility.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    05.05.2021 
 */ 
 
// includes
#include <Utils/OSUtility.h>
#include <Utils/StringAlgorithms.h>

#include <Windows.h>

#include <memory>

using namespace std::string_literals;


namespace osutility {


// getEnvironmentVariable
auto getEnvironmentVariable(std::wstring_view name) -> std::wstring
{
  const unsigned long minBufSize{2};
  auto buffer = std::make_unique<wchar_t[]>(minBufSize);

  // first call to obtain the number of characters required to hold the environment variable
  const auto requiredSize = GetEnvironmentVariable(name.data(), buffer.get(), minBufSize);
  if(requiredSize == 0U)
  {
    if(GetLastError() == ERROR_ENVVAR_NOT_FOUND)
    {
      throw environmentvariableNotFound("Environment variable "s + stringAlgorithms::wstringToString(name) + " not found"s);
    }
    else
    {
      throw std::runtime_error("Unknown error during call of getEnvironmentVariable");
    }
  }
  else
  {
    if(requiredSize > minBufSize || !buffer)
    {
      buffer = std::make_unique<wchar_t[]>(requiredSize);
    }

    // second call to read the environment variable
    if(GetEnvironmentVariable(name.data(), buffer.get(), requiredSize) == 0)
    {
      throw std::runtime_error("Unknown error during call of getEnvironmentVariable");
    }
  }

  return std::wstring{buffer.get()};
}


}   // namespace osutility


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
