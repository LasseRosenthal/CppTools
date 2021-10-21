/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Watchdog.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.05.2021 
 */ 
 
#ifndef WATCHDOG_H_1994945226430239401882041523032117785313 
#define WATCHDOG_H_1994945226430239401882041523032117785313 
 
 
// includes
#include "RAIIThread.h"

#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <mutex>


namespace cctools {


/** 
 * @class Watchdog
 * @brief Watchdog implements a timer that calls recurrently calls
 *        a specified function after a given time out has expired.
 */
class Watchdog {

public:

  // ---------------------------------------------------
  // public types
  using callback_type = std::function<void(void)>;
  using duration_type = std::chrono::milliseconds;

  // ---------------------------------------------------
  // 
  template <typename Rep, typename Period>
  Watchdog(callback_type c, std::chrono::duration<Rep, Period> t,
           bool startNow);

  // ---------------------------------------------------
  // public api
  void registerCallback (callback_type c);
  void start            ();
  void stop             ();

private:

  // ---------------------------------------------------
  // private methods
  void loop ();

  // ---------------------------------------------------
  // private data
  cctools::JoinThread     watchThread;
  callback_type           callback;
  duration_type           timeOut;
  mutable std::mutex      mut;
  std::condition_variable buzzer;
  bool                    timeOutExpired {false};
  bool                    isActive {false};

};


template <typename Rep, typename Period>
Watchdog::Watchdog(callback_type c, std::chrono::duration<Rep, Period> t,
                   bool startNow)
  : callback {std::move(c)}
  , timeOut  {t}
{
  if(startNow)
  {
    start();
  }
}

inline void Watchdog::registerCallback(callback_type c)
{
  callback = std::move(c);
}

/** 
 * @brief starts the 
 */
inline void Watchdog::start()
{
  if(!callback)
  {
    throw std::runtime_error("no callback registered");
  }

  isActive = true;
  watchThread = cctools::JoinThread([this] { this->loop(); });
}

/**
 * @brief stops the watchdog loop.
 */
inline void Watchdog::stop()
{
  isActive = false;
  buzzer.notify_one();
}

inline void Watchdog::loop()
{
  while(isActive)
  {
    std::unique_lock<std::mutex> lock(mut);
  }
}


}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // WATCHDOG_H_1994945226430239401882041523032117785313 
