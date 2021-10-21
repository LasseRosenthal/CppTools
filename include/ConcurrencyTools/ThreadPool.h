/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   ThreadPool.h
 *         
 * @author Lasse Rosenthal
 * @date   22.01.2020
 */

#ifndef THREADPOOL_H_142401729555859226299031251320350853928695 
#define THREADPOOL_H_142401729555859226299031251320350853928695 
 
 
// includes
#include "FunctionWrapper.h"
#include "RAIIThread.h"
#include "ThreadsafeQueue.h"

#include <future>
#include <limits>
#include <type_traits>
#include <vector>


namespace cctools {


/**
 * @brief  Policy for threadpools
 */
enum class threadPoolPolicy : char {
  waitable,
  nonwaitable
};

/**
 * @brief Policy for the queueing policy
 */
enum class queuePolicy : char {
  fifo,
  prioritized
};
 

namespace detail {

template <queuePolicy>
struct PoolQueueT;

template <>
struct PoolQueueT<queuePolicy::fifo> {
  template <typename T>
  struct QueueType {
    using type = Queue<T>;
  };
};

template <>
struct PoolQueueT<queuePolicy::prioritized> {
  template <typename T>
  struct QueueType {
    using type = PriorityQueue<T>;
  };
};

template <queuePolicy p, typename T>
using PoolQueue = typename PoolQueueT<p>::template QueueType<T>::type;


}   // namespace detail


//#define USE_YIELD_IN_LOOP 

/**
 * @class ThreadPool
 * @brief ThreadPool is a threadpool
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy = queuePolicy::prioritized>
class ThreadPool {

#ifdef USE_YIELD_IN_LOOP
  static constexpr std::chrono::microseconds loopTimeOut {50};
#endif

  // ---------------------------------------------------
  // private constants & types
  static constexpr bool isWaitable = Policy == threadPoolPolicy::waitable;

  template <typename Fun>
  using waitableResultType = std::enable_if_t<isWaitable, std::future<std::invoke_result_t<Fun>>>;

public:

  // ---------------------------------------------------
  // public types & constants
  using size_type = std::size_t;
  using callable  = FunctionWrapper;
  
  static constexpr threadPoolPolicy policy = Policy;

  // ---------------------------------------------------
  // constructors & dtor
  ThreadPool     (size_type numThreads, size_type capacity = (std::numeric_limits<size_type>::max)());
  ThreadPool     () = delete;
  ThreadPool     (ThreadPool const&) = delete;
  ThreadPool     (ThreadPool&&) = delete;
  auto operator= (ThreadPool const&) -> ThreadPool& = delete;
  auto operator= (ThreadPool&&) -> ThreadPool& = delete;
  ~ThreadPool    ();

  // ---------------------------------------------------
  // api
  auto size       () const noexcept -> size_type;
  template <typename Fun>
  auto submit     (Fun&& fun, int priority = 0) -> waitableResultType<Fun>;
  template <typename Fun, typename = std::enable_if_t<!isWaitable>>
  void submit     (Fun&& fun, int priority = 0);
  void deactivate ();

private:

  // ---------------------------------------------------
  // an alias for the queue type
  using queue = detail::PoolQueue<QueuePolicy, callable>;

  // ---------------------------------------------------
  // private data
  bool                    isActive {true};
  const size_type         numberOfThreads;
  const size_type         queueCapacity {(std::numeric_limits<size_type>::max)()};
  queue                   scheduledTasks;
  std::vector<JoinThread> workingThreads;

  // ---------------------------------------------------
  void workThread ();
};


/**
 * @brief Convenient alias for waitable thread pools.
 */
template <queuePolicy p>
using WaitableThreadPool = ThreadPool<threadPoolPolicy::waitable, p>;

/**
 * @brief Convenient alias for non-waitable thread pools.
 */
template <queuePolicy p>
using NonWaitableThreadPool = ThreadPool<threadPoolPolicy::nonwaitable, p>;


/**
 * @brief Constructor. Starts the working threads.
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
ThreadPool<Policy, QueuePolicy>::ThreadPool(size_type numThreads, size_type capacity)
  : numberOfThreads {numThreads}
  , queueCapacity   {capacity}
  , scheduledTasks  {queueCapacity}
{
  try
  {
    workingThreads.reserve(numberOfThreads);
    for(std::size_t i = 0ULL; i < numberOfThreads; ++i)
    {
      workingThreads.emplace_back(&ThreadPool::workThread, this);
    }
  }
  catch(...)
  {
    isActive = false;
    throw;
  }
}

/**
 * @brief The actual working method
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
void ThreadPool<Policy, QueuePolicy>::workThread()
{
  while(isActive)
  {
    callable task;
#ifdef USE_YIELD_IN_LOOP
    if(scheduledTasks.tryPop(task))
    {
      task();
    }
    else
    {
      std::this_thread::yield();
      std::this_thread::sleep_for(loopTimeOut);
    }
#else
    scheduledTasks.waitAndPop(task);
    if(isActive)
    {
      task();
    }
#endif
  }
}

/**
 * @brief Deactivates the thread pool to ensure that the working
 *        threads don't continue to process possibly remaining tasks.
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
inline ThreadPool<Policy, QueuePolicy>::~ThreadPool()
{
  deactivate();
  //  isActive = false;
//#if !USE_YIELD_IN_LOOP
//  for(size_type i{}; i < numberOfThreads; ++i)
//  {
//    // this is a hack to ensure that every thread gets one last task to process.
//    submit([]{});
//  }
//#endif
}

/**
 * @brief Returns the number threads in the pool.
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
inline auto ThreadPool<Policy, QueuePolicy>::size() const noexcept -> size_type
{
  return numberOfThreads;
}

/**
 * @brief Schedules the passed function for processing and returns
 *        a future associated with a shared state holding the
 *        result of the method.
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
template <typename Fun>
auto ThreadPool<Policy, QueuePolicy>::submit(Fun&& fun, int priority) -> waitableResultType<Fun>
{
  std::packaged_task<std::invoke_result_t<Fun>()> task(std::forward<Fun>(fun));
  auto result = task.get_future();
  scheduledTasks.push(callable(std::move(task), priority));

  return result;
}

/**
 * @brief Schedules the passed function for processing.
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
template <typename Fun, typename>
inline void ThreadPool<Policy, QueuePolicy>::submit(Fun&& fun, int priority)
{
  scheduledTasks.push(callable(std::forward<Fun>(fun), priority));
}

/**
 * @brief Deactivates the thread pool.
 */
template <threadPoolPolicy Policy, queuePolicy QueuePolicy>
inline void ThreadPool<Policy, QueuePolicy>::deactivate()
{
  if(isActive)
  {
    isActive = false;
    scheduledTasks.stopQueue();
  }
}


}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // THREADPOOL_H_142401729555859226299031251320350853928695 
