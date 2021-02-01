/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    XercesInitializer.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    20.01.2021 
 */ 
 
#ifndef XERCESINITIALIZER_H_6194257861170624032100303186281092258126562 
#define XERCESINITIALIZER_H_6194257861170624032100303186281092258126562 
 
 
// includes
#include <xercesc/util/PlatformUtils.hpp>

namespace xcu {


/** 
 * @class XercesInitializer
 * @brief 
 */
class XercesInitializer {

public:

  // ---------------------------------------------------
  // construction
  XercesInitializer  ();
  XercesInitializer  (XercesInitializer const&) = delete;
  XercesInitializer  (XercesInitializer&&) = delete;
  auto operator=     (XercesInitializer const&) -> XercesInitializer& = delete;
  auto operator=     (XercesInitializer&&) -> XercesInitializer& = delete;
  ~XercesInitializer ();

  // ---------------------------------------------------
  // public api
  void terminate();

private:

  bool isInitialized{false};
};


/**
 * @brief Constructor. Initializes the xerces framework.
 */
XercesInitializer::XercesInitializer()
{
  try
  {
    xercesc::XMLPlatformUtils::Initialize();
    isInitialized = true;
  }
  catch(xercesc::XMLException const&)
  {
    // Do your failure processing here
  }
}

XercesInitializer::~XercesInitializer()
{
  terminate();
}

/**
 * @brief Terminates the xerces framework if it has been initialized before.
 */
inline void XercesInitializer::terminate()
{
  if(isInitialized)
  {
    try
    {
      xercesc::XMLPlatformUtils::Terminate();
      isInitialized = false;
    }
    catch (xercesc::XMLException const&)
    {

    }
  }
}


}   // namespace xcu


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // XERCESINITIALIZER_H_6194257861170624032100303186281092258126562 
