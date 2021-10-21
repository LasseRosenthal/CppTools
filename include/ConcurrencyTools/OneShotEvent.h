/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   OneShotEvent.h
 * @brief  OneSHotEvent is a class that provides a simple interface
 *         for one shot event communication.
 *         
 * @author Lasse Rosenthal
 * @date   21.01.2020
 */

#ifndef ONESHOTEVENT_H_8121258231009119235877937997211830717459 
#define ONESHOTEVENT_H_8121258231009119235877937997211830717459 
 
 
// includes 
#include <chrono>
#include <future>
#include <mutex>
#include <type_traits>


namespace cctools {


/// sharing policy for one shot events.
enum class eventPolicy : char {
  shared,    ///< multiple threads are allowed to wait for the event
  nonShared  ///< event may only be accessed by one thread
};


/**
 * @brief Class OneShotEvent provides a simple facility
 *        for one-shot event communication.
 */
template <typename T, eventPolicy EventPolicy = eventPolicy::nonShared>
class OneShotEvent {

public:

  // ---------------------------------------------------
  // public data
  using data_type   = T;
  using future_type = std::conditional_t<EventPolicy == eventPolicy::nonShared,
                                         std::future<data_type>, std::shared_future<data_type>>;

  // ---------------------------------------------------
  // construction
  OneShotEvent   ();
  OneShotEvent   (OneShotEvent const&) = delete;
  OneShotEvent   (OneShotEvent&&) = default;
  auto operator= (OneShotEvent const&) -> OneShotEvent& = delete;
  auto operator= (OneShotEvent&&) -> OneShotEvent&;

  // ---------------------------------------------------
  // public api
  void notify       (data_type&& value);
  void notify       (data_type const& value);
  auto get          () -> data_type;
  void wait         () const;
  auto setException (std::exception_ptr exPtr) -> bool;

private:

  // ---------------------------------------------------
  // private data
  std::promise<data_type> prom;
  future_type             fut;
};


/**
 * Convenient alias for non-shared events.
 */
template <typename T>
using NonSharedOneShotEvent = OneShotEvent<T, eventPolicy::nonShared>;

/**
 * Convenient alias for shared events.
 */
template <typename T>
using SharedOneShotEvent = OneShotEvent<T, eventPolicy::shared>;


/**
 * @brief constructor. Retrieves the future from the promise.
 */
template <typename T, eventPolicy EventPolicy>
inline OneShotEvent <T, EventPolicy>::OneShotEvent()
  : fut {prom.get_future()}
{}

/**
 * @brief Move assignment.
 */
template <typename T, eventPolicy EventPolicy>
auto OneShotEvent<T, EventPolicy>::operator=(OneShotEvent&& src) -> OneShotEvent&
{
  if(this != &src)
  {
    prom = std::move(src.prom);
    fut = std::move(src.fut);
  }
  return *this;
}

/**
 * @brief Stores (moves) the value into the shared state.
 */
template <typename T, eventPolicy EventPolicy>
inline void OneShotEvent<T, EventPolicy>::notify(data_type&& value)
{
  prom.set_value(std::move(value));
}

/**
 * @brief  Stores (copies) the value into the shared state.
 */
template <typename T, eventPolicy EventPolicy>
inline void OneShotEvent<T, EventPolicy>::notify(data_type const& value)
{
  prom.set_value(value);
}

/**
 * @brief Returns the value associated with the shared state.
 */
template <typename T, eventPolicy EventPolicy>
inline auto OneShotEvent<T, EventPolicy>::get() -> data_type
{
  if(!fut.valid())
  {
    throw std::runtime_error("future is not associated with a shared state.");
  }
  return fut.get();
}

/**
 * @brief Blocks the current thread until the result becomes available.
 */
template <typename T, eventPolicy EventPolicy>
inline void OneShotEvent<T, EventPolicy>::wait() const
{
  if(!fut.valid())
  {
    throw std::runtime_error("future is not associated with a shared state.");
  }
  return fut.wait();
}

/**
 * @brief Stores the exception into the shared state.
 */
template <typename T, eventPolicy EventPolicy>
inline bool OneShotEvent<T, EventPolicy>::setException(std::exception_ptr exPtr)
{
  try
  {
    prom.set_exception(exPtr);
    return true;
  }
  catch(...) // set_exception may throw too
  {
    return false;
  }
}


/**
 * @brief Full template specialization of class OneShotEvent for void
 */
template <eventPolicy EventPolicy>
class OneShotEvent<void, EventPolicy> {

public:

  // ---------------------------------------------------
  // public data
  using data_type = void;
  using future_type = std::conditional_t<EventPolicy == eventPolicy::nonShared,
                                         std::future<void>,
                                         std::shared_future<void>>;

  // ---------------------------------------------------
  // construction
  OneShotEvent   ();
  OneShotEvent   (OneShotEvent const&) = delete;
  OneShotEvent   (OneShotEvent&&) = default;
  auto operator= (OneShotEvent const&) -> OneShotEvent& = delete;
  auto operator= (OneShotEvent&&) -> OneShotEvent&;

  // ---------------------------------------------------
  // public api
  void notify       ();
  void get          ();
  void wait         () const;
  auto setException (std::exception_ptr exPtr) -> bool;

private:

  // ---------------------------------------------------
  // private data
  std::promise<data_type> prom;
  future_type             fut;
};


/**
 * @brief Constructor. Retrieves the future from the promise.
 */
template <eventPolicy EventPolicy>
inline OneShotEvent<void, EventPolicy>::OneShotEvent()
  : fut {prom.get_future()}
{}

/**
 * @brief Move assignment.
 */
template <eventPolicy EventPolicy>
inline auto OneShotEvent<void, EventPolicy>::operator=(OneShotEvent&& src) -> OneShotEvent&
{
  if(this != &src)
  {
    prom = std::move(src.prom);
    fut = std::move(src.fut);
  }
  return *this;
}

/**
 * @brief Makes the shared state ready.
 */
template <eventPolicy EventPolicy>
inline void OneShotEvent<void, EventPolicy>::notify()
{
  prom.set_value();
}

/**
 * @brief Waits until the future has a valid result.
 */
template <eventPolicy EventPolicy>
inline void OneShotEvent<void, EventPolicy>::get()
{
  fut.get();
}

/**
 * @brief Blocks until the shared state is ready.
 */
template <eventPolicy EventPolicy>
inline void OneShotEvent<void, EventPolicy>::wait() const
{
  fut.wait();
}

/**
 * @brief Stores the exception into the shared state.
 */
template <eventPolicy EventPolicy>
inline bool OneShotEvent<void, EventPolicy>::setException(std::exception_ptr exPtr)
{
  try
  {
    prom.set_exception(exPtr);
    return true;
  }
  catch(...) // set_exception may throw too
  {
    return false;
  }
}


}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ONESHOTEVENT_H_8121258231009119235877937997211830717459 
