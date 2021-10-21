/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   RAIIThread.h
 * @brief  RAIIThread is a thin wrapper around the std::thread class.
 *         It can be configured to automatically perform a join
 *         or detach upon destruction.
 *         
 * @author Lasse Rosenthal
 * @date   04.07.2019
 */

#ifndef RAIITHREAD_H_8374684312398116422447125200397791528002
#define RAIITHREAD_H_8374684312398116422447125200397791528002


// includes
#include <functional>
#include <thread>


namespace cctools {


/**
 * @brief this enum is used to determine the type of cleanup
 *        during the destructor of the RAIIThread
 */
enum class threadcleanup : char { 
  join,
  detach
};

namespace detail {

template <threadcleanup CleanUp, typename Thread>
struct ThreadCleanerT;

template <typename Thread>
struct ThreadCleanerT<threadcleanup::join, Thread> {
  static constexpr void(Thread::*cleanUpAction)() = &Thread::join;
};

template <typename Thread>
struct ThreadCleanerT<threadcleanup::detach, Thread> {
  static constexpr void(Thread::*cleanUpAction)() = &Thread::detach;
};

template <threadcleanup CleanUp, typename Thread>
constexpr auto ThreadCleaner = ThreadCleanerT<CleanUp, Thread>::cleanUpAction;

}   // namespace detail


/**
 * @class RAIIThread
 * @brief
 */
template <threadcleanup CleanUp, typename Thread = std::thread>
class RAIIThread {

public:

  // ---------------------------------------------------
  // types
  using thread_type        = Thread;
  using id                 = typename thread_type::id;
  using native_handle_type = typename thread_type::native_handle_type;

  // ---------------------------------------------------
  // constructor & destructor
  RAIIThread          () = default;
  RAIIThread          (thread_type&& thread);
  RAIIThread          (RAIIThread const&) = delete;
  RAIIThread          (RAIIThread&&) = default;
  auto operator=      (RAIIThread const&) -> RAIIThread& = delete;
  auto operator=      (RAIIThread&&) -> RAIIThread& = default;
  template <typename Function, typename... Args>
  explicit RAIIThread (Function&& f, Args&&... args);
  ~RAIIThread         ();

  // ---------------------------------------------------
  // methods
  [[nodiscard]] auto joinable () const noexcept -> bool;
  auto getId                  () const noexcept -> id;
  auto nativeHandle           () -> native_handle_type;
  void swap                   (RAIIThread& other) noexcept;

private:

  // ---------------------------------------------------
  // the thread
  thread_type raiiThread;
};

/**
 * @brief Convenient alias definitions for the two thread types
 */
using JoinThread   = RAIIThread<threadcleanup::join, std::thread>;
using DetachThread = RAIIThread<threadcleanup::detach, std::thread>;


/**
 * @brief Moves the provided thread into the RAIIThread
 */
template <threadcleanup CleanUp, typename Thread>
inline RAIIThread<CleanUp, Thread>::RAIIThread(thread_type&& thread)
  : raiiThread {std::move(thread)}
{}

/**
 * @brief Creates a new thread and associates it with the execution
 *        of the provided function.
 */
template <threadcleanup CleanUp, typename Thread>
template <typename Function, typename... Args>
inline RAIIThread<CleanUp, Thread>::RAIIThread(Function&& f, Args&&... args)
  : raiiThread {std::forward<Function>(f), std::forward<Args>(args)...}
{}

/**
 * @brief Destructor. invokes join or detach.
 */
template <threadcleanup CleanUp, typename Thread>
inline RAIIThread<CleanUp, Thread>::~RAIIThread()
{
  if(raiiThread.joinable())
  {
    std::invoke(detail::ThreadCleaner<CleanUp, Thread>, raiiThread);
  }
}

/**
 * @brief Exchanges the underlying handles of two thread objects.
 */
template <threadcleanup CleanUp, typename Thread>
inline void RAIIThread<CleanUp, Thread>::swap(RAIIThread& other) noexcept
{
  raiiThread.swap(other.raiiThread);
}

/**
 * @brief Checks if the std::thread object identifies an active thread of execution.
 */
template <threadcleanup CleanUp, typename Thread>
inline [[nodiscard]] auto RAIIThread<CleanUp, Thread>::joinable() const noexcept -> bool
{
  return raiiThread.joinable();
}

/**
 * @brief Returns a value of thread_type::id identifying the thread associated with *this.
 */
template <threadcleanup CleanUp, typename Thread>
inline auto RAIIThread<CleanUp, Thread>::getId() const noexcept -> id
{
  return raiiThread.get_id();
}

/**
 * @brief Returns the implementation defined underlying thread handle.
 */
template <threadcleanup CleanUp, typename Thread>
inline auto RAIIThread<CleanUp, Thread>::nativeHandle() -> native_handle_type
{
  return raiiThread.native_handle();
}


}   // namespace cctools


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif   // RAIITHREAD_H_8374684312398116422447125200397791528002
