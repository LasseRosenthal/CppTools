/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    XercesString.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.02.2021 
 */ 
 
// includes 
#include <XercesUtils/XercesString.h>


namespace xcu {


/**
 * @brief Appends the content of str to the current string.
 */
auto XercesString::append(XercesString const& src) -> XercesString&
{
  const auto newLength = strLength + src.strLength;
  auto* newBuffer = reinterpret_cast<value_type*>(xercesc::XMLPlatformUtils::fgMemoryManager->allocate(newLength + 1ULL));
  xercesc::XMLString::copyString(newBuffer, strData.get());
  xercesc::XMLString::catString(newBuffer, src.strData.get());

  strData.reset(newBuffer);
  strLength = newLength;

  return *this;
}


}   // namespace xcu


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
