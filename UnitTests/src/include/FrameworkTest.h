/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FrameworkTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    10.02.2021 
 */ 
 
#ifndef FRAMEWORKTEST_H_2977982424799892077621258419590505129538 
#define FRAMEWORKTEST_H_2977982424799892077621258419590505129538 
 
 
// includes
#include <Utils/Framework.h>

#include <iostream>
#include <string>

using namespace std::string_literals;
 

namespace cpptools {


/** 
 * @class DebugTraceFramework
 * @brief DebugTraceFramework encapsulates the initialization and termination process
 *        for the DebugTrace framework.
 */
class DebugTraceFramework final : Framework {

  // the Framework interface shall have access to the constructor
  friend Framework;

public:
  /**
   * DbgTraceParams stores the configuration parameters for DebugTrace
   */
  struct Params {
    int p{-1};
  };


private:

  DebugTraceFramework(std::string const& componentName, const Params& params = {});
  ~DebugTraceFramework();
};


/**
 * @brief Constructor. Initializes the DebugTrace framework. If the initialization fails,
 *        an error message is provided.
 */
inline DebugTraceFramework::DebugTraceFramework(const std::string&                 componentName,
                                                const DebugTraceFramework::Params& params)
{
  try
  {
    std::cout << "DebugTraceFramework initialize ...\n"
              << "params.p = " << params.p << '\n';
  }
  catch(...)
  {
    success = false;
    errMsg  = "Error during initialization of Debug Trace";
  }
}

/**
 * @brief Destructor. Terminates the DebugTrace framework.
 */
inline DebugTraceFramework::~DebugTraceFramework()
{
  std::cout << "Terminate DebugTrace now ...\n";
}

}   // namespace cpptools


TEST(Framework, constructor)
{
  cpptools::DebugTraceFramework::Params par;
  par.p          = 1345;
  const auto framework = cpptools::Framework::create<cpptools::DebugTraceFramework>("Test"s, par);

  EXPECT_TRUE(framework->isInitialized());
}

 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FRAMEWORKTEST_H_2977982424799892077621258419590505129538 
