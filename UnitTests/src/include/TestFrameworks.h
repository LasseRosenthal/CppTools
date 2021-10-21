/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestFrameworks.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    16.09.2021 
 */ 
 
#ifndef TESTFRAMEWORKS_H_23690226221868205313215537593003081011941 
#define TESTFRAMEWORKS_H_23690226221868205313215537593003081011941 
 
 
// includes
#include <Utils/Framework.h>

#include <string>
#include <vector>
 
 
namespace cpptools {
 

class TestFramework final : Framework {

  // the Framework interface shall have access to the constructor
  friend Framework;

public:

  void terminate() override;

private:

  TestFramework  (bool& b, std::vector<std::string>& msgs, bool throwException = false);
  ~TestFramework ();

  bool& dtorCalled;
  std::vector<std::string>& msgs;
};


/**
 * @brief Constructor. Initializes the framework. If the initialization fails,
 *        an error message is provided.
 */
inline TestFramework::TestFramework(bool& b, std::vector<std::string>& msgs, bool throwException)
  : dtorCalled {b}
  , msgs {msgs}
{
  try
  {
    dtorCalled = false;
    if(throwException)
    {
      throw std::exception("error message");
    }
  }
  catch(...)
  {
    initialized = false;
    errMsg  = "Error during initialization of Debug Trace";
    return;
  }

  initialized = true;
  msgs.push_back("TestFramework1 initialized");
}

inline void TestFramework::terminate()
{
  initialized = false;
  dtorCalled = true;
  msgs.push_back("TestFramework1 terminated");
}

/**
 * @brief Destructor. Terminates the DebugTrace framework.
 */
inline TestFramework::~TestFramework()
{
  if(isInitialized())
  {
    terminate();
  }
}



class TestFramework2 final : Framework {

  // the Framework interface shall have access to the constructor
  friend Framework;

public:

  void terminate() override;

private:

  TestFramework2  (bool& b, std::vector<std::string>& msgs, bool throwException = false);
  ~TestFramework2 ();

  bool& dtorCalled;
  std::vector<std::string>& msgs;
};


/**
 * @brief Constructor. Initializes the framework. If the initialization fails,
 *        an error message is provided.
 */
inline TestFramework2::TestFramework2(bool& b, std::vector<std::string>& msgs, bool throwException)
  : dtorCalled{b}
  , msgs{msgs}
{
  try
  {
    dtorCalled = false;
    if(throwException)
    {
      throw std::exception("error message");
    }
  }
  catch(...)
  {
    initialized = false;
    errMsg  = "Error during initialization of Debug Trace";
    return;
  }

  msgs.push_back("TestFramework2 initialized");
  initialized = true;
}

inline void TestFramework2::terminate()
{
  initialized = false;
  dtorCalled = true;
  msgs.push_back("TestFramework2 terminated");
}

/**
 * @brief Destructor. Terminates the DebugTrace framework.
 */
inline TestFramework2::~TestFramework2()
{
  if(isInitialized())
  {
    terminate();
  }
}


}   // namespace cpptools
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTFRAMEWORKS_H_23690226221868205313215537593003081011941 
