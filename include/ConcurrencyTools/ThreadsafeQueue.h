/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   ThreadsafeQueue.h
 * @brief  implementation of a thread safe queue
 *         based on Williams - C++ Concurrency in Action
 *         
 * @author Lasse Rosenthal
 * @date   05.07.2019
 */

#ifndef THREADSAFEQUEUE_H_14637657521676284412325020248211542122228070
#define THREADSAFEQUEUE_H_14637657521676284412325020248211542122228070


// includes
#include <ConcurrencyTools/detail/FunctionTraits.h>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iterator>
#include <limits>
#include <list>
#include <optional>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace cctools {


/// insertion policy for queue classes.
enum class insertionPolicy : char {
  prioritized,  ///< insertion policy for riority queues
  fifo          ///< unprioritized insertion, first in first out
};


namespace detail {


/**
 * @struct QueueAccessor
 * @brief  QueueAccessor is a policy class that allows for acces of the elements of a
 *         given container according to a specified insertion policy.
 */
template <typename Container, insertionPolicy InsertionPolicy, typename...>
class QueueAccessor;

/**
 * @brief Partial specialization of QueueAccessor for prioritized queues.
 */
template <typename Container, typename Compare>
class QueueAccessor<Container, insertionPolicy::prioritized, Compare> {

public:

  // ---------------------------------------------------
  // types
  using value_type      = typename Container::value_type;
  using const_reference = typename Container::const_reference;
  using reference       = typename Container::reference;
  using iterator        = typename Container::reverse_iterator;
  using const_iterator  = typename Container::const_reverse_iterator;

  // ---------------------------------------------------
  // ctor & dtor
  QueueAccessor  (Container& cont);
  QueueAccessor  () = delete;
  QueueAccessor  (QueueAccessor const&) = delete;
  QueueAccessor  (QueueAccessor&&) = delete;
  auto operator= (QueueAccessor const&) -> QueueAccessor& = delete;
  auto operator= (QueueAccessor&&) -> QueueAccessor& = delete;
  ~QueueAccessor () = default;

  // ---------------------------------------------------
  // access methods
  void push   (value_type const& value);
  void push   (value_type&& value);
  void pop    (value_type& value);
  auto front  () const -> const_reference;
  auto front  () -> reference;
  void erase  (iterator elem);
  auto cbegin () const -> const_iterator;
  auto cend   () const -> const_iterator;
  auto begin  () -> iterator;
  auto end    () -> iterator;
  auto begin  () const -> const_iterator;
  auto end    () const -> const_iterator;

private :

  auto insertPosition(value_type const& value) -> typename Container::const_iterator;

  // ---------------------------------------------------
  // member
  Compare    compare;
  Container& container;
};

template <typename Container, typename Compare>
inline QueueAccessor<Container, insertionPolicy::prioritized, Compare>::QueueAccessor(Container& cont)
  : container{cont}
{}

template <typename Container, typename Compare>
inline void QueueAccessor<Container, insertionPolicy::prioritized, Compare>::push(value_type const& value)
{
  container.insert(insertPosition(value), value);
}

template <typename Container, typename Compare>
inline void QueueAccessor<Container, insertionPolicy::prioritized, Compare>::push(value_type&& value)
{
  const auto pos = insertPosition(value);
  container.insert(pos, std::move(value));
}

template <typename Container, typename Compare>
inline void QueueAccessor<Container, insertionPolicy::prioritized, Compare>::pop(value_type& value)
{
  value = std::move(front());
  container.pop_back();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::front() const -> const_reference
{
  return container.back();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::front() -> reference
{
  return container.back();
}

template <typename Container, typename Compare>
inline void QueueAccessor<Container, insertionPolicy::prioritized, Compare>::erase(iterator elem)
{
  container.erase(std::prev(elem.base()));
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::cbegin() const -> const_iterator
{
  return container.crbegin();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::cend() const -> const_iterator
{
  return container.crend();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::begin() -> iterator
{
  return container.rbegin();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::end() -> iterator
{
  return container.rend();
}



template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::begin() const -> const_iterator
{
  return container.crbegin();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::end() const -> const_iterator
{
  return container.crend();
}

template <typename Container, typename Compare>
inline auto QueueAccessor<Container, insertionPolicy::prioritized, Compare>::insertPosition(value_type const& value)
  -> typename Container::const_iterator
{
  // lower_bound returns an iterator to the the first element for which
  // the predicate (the passed value is the second argument to the predicate) yields false
  // see http://en.cppreference.com/w/cpp/algorithm/lower_bound
  // and http://www.cplusplus.com/reference/algorithm/lower_bound/
  return std::lower_bound(container.begin(), container.end(), value, compare);
}


/**
 * @brief Partial specialization of QueueAccessor for fifo queues.
 */
template <typename Container>
class QueueAccessor<Container, insertionPolicy::fifo, void> {

public:

  // ---------------------------------------------------
  // types
  using value_type      = typename Container::value_type;
  using const_reference = typename Container::const_reference;
  using reference       = typename Container::reference;
  using iterator        = typename Container::reverse_iterator;
  using const_iterator  = typename Container::const_reverse_iterator;
  
  // ---------------------------------------------------
  // ctor & dtor
  QueueAccessor  (Container& cont);
  QueueAccessor  () = delete;
  QueueAccessor  (QueueAccessor const&) = delete;
  QueueAccessor  (QueueAccessor&&) = delete;
  auto operator= (QueueAccessor const&) -> QueueAccessor& = delete;
  auto operator= (QueueAccessor&&) -> QueueAccessor& = delete;
  ~QueueAccessor () = default;

  // ---------------------------------------------------
  // access methods
  void push    (value_type const& value);
  void push    (value_type&& value);
  template <typename... Args>
  void emplace (Args&&... args);
  void pop     (value_type& value);
  auto front   () const -> const_reference;
  auto front   () -> reference;
  void erase   (iterator elem);
  auto cbegin  () const -> const_iterator;
  auto cend    () const -> const_iterator;
  auto begin   () -> iterator;
  auto end     () -> iterator;
  auto begin   () const -> const_iterator;
  auto end     () const -> const_iterator;

private:

  // ---------------------------------------------------
  // member
  Container& container;
};

template <typename Container>
inline QueueAccessor<Container, insertionPolicy::fifo, void>::QueueAccessor(Container& cont)
  : container{cont}
{}

template <typename Container>
inline void QueueAccessor<Container, insertionPolicy::fifo, void>::push(value_type const& value)
{
  container.insert(container.cbegin(), value);
}

template <typename Container>
inline void QueueAccessor<Container, insertionPolicy::fifo, void>::push(value_type&& value)
{
  container.insert(container.cbegin(), std::move(value));
}

template <typename Container>
template <typename... Args>
inline void QueueAccessor<Container, insertionPolicy::fifo, void>::emplace(Args&&... args)
{
  container.emplace(container.cbegin(), std::forward<Args>(args)...);
}

template <typename Container>
inline void QueueAccessor<Container, insertionPolicy::fifo, void>::pop(value_type& value)
{
  value = std::move(front());
  container.pop_back();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::front() const -> const_reference
{
  return container.back();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::front() -> reference
{
  return container.back();
}

template <typename Container>
inline void QueueAccessor<Container, insertionPolicy::fifo, void>::erase(iterator elem)
{
  container.erase(std::prev(elem.base()));
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::cbegin() const -> const_iterator
{
  return container.crbegin();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::cend() const -> const_iterator
{
  return container.crend();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::begin() -> iterator
{
  return container.rbegin();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::end() -> iterator
{
  return container.rend();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::begin() const -> const_iterator
{
  return container.crbegin();
}

template <typename Container>
inline auto QueueAccessor<Container, insertionPolicy::fifo, void>::end() const -> const_iterator
{
  return container.crend();
}


}   // namespace detail


/**
 * @class ThreadsafeQueueT
 * @brief ThreadsafeQueueT is a queue that allows threadsafe access to its elements
 *        according to specified insertion policy.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare = void>
class ThreadsafeQueueT {

  // ---------------------------------------------------
  // internal types
  using mutex         = std::mutex;
  using lock          = std::lock_guard<mutex>;
  using uniqueLock    = std::unique_lock<mutex>;
  using queueAccessor = detail::QueueAccessor<Container<T>, InsertionPolicy, Compare>; 

  // ---------------------------------------------------
  // internal SFINAE alias definitions
  template <insertionPolicy p>
  using requiresFIFO = std::enable_if_t<p == insertionPolicy::fifo>;

  template <typename V>
  using visitorResult = typename detail::FunctionTraits<V>::result_type;

  template <typename S>
  using requiresNonVoid = std::enable_if_t<!std::is_void_v<S>>;

  template <typename V>
  using requiresVoidVisitor = std::enable_if_t<std::is_void_v<visitorResult<V>>>;

public:

  // ---------------------------------------------------
  // public types
  using container_type  = Container<T>;
  using value_type      = typename container_type::value_type;
  using size_type       = typename container_type::size_type;
  using reference       = typename container_type::reference;
  using const_reference = typename container_type::const_reference;

  // ---------------------------------------------------
  // constructors & assignments
  ThreadsafeQueueT  (size_type capacity = (std::numeric_limits<size_type>::max)());
  ThreadsafeQueueT  (ThreadsafeQueueT const& src);
  ThreadsafeQueueT  (ThreadsafeQueueT&& src);
  auto operator=    (ThreadsafeQueueT const& src) -> ThreadsafeQueueT&;
  auto operator=    (ThreadsafeQueueT&& src) -> ThreadsafeQueueT&;
  ~ThreadsafeQueueT ();

  // ---------------------------------------------------
  // methods
  auto empty                () const -> bool;
  auto size                 () const -> size_type;
  auto capacity             () const noexcept -> size_type;
  void push                 (value_type const& value);
  void push                 (value_type&& value);
  auto tryPush              (value_type const& value) -> bool;
  auto tryPush              (value_type&& value) -> bool;
  template <typename Rep, typename Period>
  auto tryPushFor           (value_type const& value,
                             std::chrono::duration<Rep, Period> const& timeOut) -> bool;
  template <typename Rep, typename Period>
  auto tryPushFor           (value_type&& value, 
                             std::chrono::duration<Rep, Period> const& timeOut) -> bool;
  void waitAndPop           (value_type& value);
  auto tryPop               (value_type& value) -> bool;
  template <typename Rep, typename Period>
  auto tryPopFor            (value_type& value,
                             std::chrono::duration<Rep, Period> const& timeOut) -> bool;
  template <typename... Args, typename = requiresFIFO<InsertionPolicy>>
  void emplace              (Args&&... args);
  void swap                 (ThreadsafeQueueT& other) noexcept;
  void stopQueue            ();
  template <typename Predicate>
  auto contains             (Predicate&& predicate) const -> bool;
  template <typename Predicate>
  auto hasTopProperty       (Predicate&& predicate) const -> bool;
  template <typename Predicate>
  auto removeIf             (Predicate&& predicate) -> size_type;
  template <typename Predicate>
  auto extractIf            (value_type& value, Predicate&& predicate) -> bool;
  template <typename Predicate>
  auto extractIf            (Predicate&& predicate) -> std::list<value_type>;
  template <typename F>
  auto forEach              (F&& f) const -> F;
  template <typename V, typename = requiresVoidVisitor<V>>
  void visitTop             (V&& visitor);
  template <typename V, typename Res = visitorResult<V>, typename = requiresNonVoid<Res>>
  auto visitTop             (V&& visitor, Res const& defaultValue = Res{}) -> Res;
  template <typename V, typename Predicate, typename = requiresVoidVisitor<V>>
  auto extractVisitReinsert (V&& visitor, Predicate&& predicate) -> size_type;
  template <typename V, typename Predicate, typename Res = visitorResult<V>, typename = requiresNonVoid<Res>>
  auto extractVisitReinsert (V&& visitor, Predicate&& predicate) -> std::optional<Res>;

private:

  // ---------------------------------------------------
  // members
  container_type          data;
  size_type               dataCapacity{(std::numeric_limits<size_type>::max)()};
  queueAccessor           accessor;
  mutable bool            isActive{true};
  mutable mutex           dataMutex;
  std::condition_variable dataCondVar;
  std::condition_variable capacityCondVar;

  // ---------------------------------------------------
  // auxiliary methods
  auto sizeIsBelowCapacity () const -> bool;
  auto hasData             () const -> bool;
  template <typename Predicate>
  auto extractIfImpl       (Predicate&& predicate) -> std::list<value_type>;
};


/**
 * @brief Alias template for prioritized queues.
 */
template <typename T, typename Compare = std::less<T>, template <typename...> typename Container = std::list>
using PriorityQueue = ThreadsafeQueueT<T, Container, insertionPolicy::prioritized, Compare>;

/**
 * @brief Alias template for fifo queues.
 */
template <typename T, template <typename...> typename Container = std::list>
using Queue = ThreadsafeQueueT<T, Container, insertionPolicy::fifo>;

/**
 * @brief constructor. Sets the data capacity.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::ThreadsafeQueueT(size_type capacity)
  : dataCapacity {capacity}
  , accessor     {data}
{}

/**
 * @brief Copy constructor.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::ThreadsafeQueueT(ThreadsafeQueueT const& src)
  : dataCapacity {src.dataCapacity}
  , accessor     {data}
{
  // ------- begin critical section ------- //
  lock lk(src.dataMutex);
  data = src.data;
}

/**
 * @brief Move constructor.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::ThreadsafeQueueT(ThreadsafeQueueT&& src)
  : dataCapacity {src.dataCapacity}
  , accessor     {data}
{
  // ------- begin critical section ------- //
  lock lk(src.dataMutex);
  data = std::move(src.data);
}

/**
 * @brief Copy assignment.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::operator=(ThreadsafeQueueT const& src) -> ThreadsafeQueueT&
{
  ThreadsafeQueueT copy(src);
  swap(copy);
  return *this;
}

/**
 * @brief Move assignment.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::operator=(ThreadsafeQueueT&& src) -> ThreadsafeQueueT&
{
  // ------- begin critical section ------- //
  lock lkThis(dataMutex);
  lock lkOther(src.dataMutex);
  data = std::move(src.data);

  return *this;
}

/**
 * @brief Destructor. Deactivates the queue.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::~ThreadsafeQueueT()
{
  if(isActive)
  {
    stopQueue();
  }
}

/**
 * @brief checks whether the underlying container is empty.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::empty() const -> bool
{
  // ------- begin critical section ------- //
  lock lk(dataMutex);
  return data.empty();
}

/**
 * @brief returns the number of elements.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::size() const -> size_type
{
  // ------- begin critical section ------- //
  lock lk(dataMutex);
  return data.size();
}

/**
 * @brief returns the capacity of the queue.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::capacity() const noexcept -> size_type
{
  return dataCapacity;
}

/**
 * @brief  Attempts to copy another element into the queue.
 * @remark Blocks the calling thread until the size of the queue allows
 *         for insertion or the queue is stopped.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::push(value_type const& value)
{
  // ------- begin critical section ------- //
  uniqueLock lk(dataMutex);
  capacityCondVar.wait(lk, [this] { return sizeIsBelowCapacity() || !isActive; });

  if(isActive)
  {
    accessor.push(value);
    dataCondVar.notify_one();
  }
}

/**
 * @brief  Attempts to move another element into the queue.
 * @remark Blocks the calling thread until the size of the queue allows
 *         for insertion or the queue is stopped.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::push(value_type&& value)
{
  // ------- begin critical section ------- //
  uniqueLock lk(dataMutex);
  capacityCondVar.wait(lk, [this] { return sizeIsBelowCapacity() || !isActive; });

  if(isActive)
  {
    accessor.push(std::move(value));
    dataCondVar.notify_one();
  }
}

/**
 * @brief Attempts to copy another element into the queue. If the size of the container
 *        doesn't exceed the container's capacity, the value is copied into the queue and
 *        the return value is true. Otherwise, it is false.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::tryPush(value_type const& value) -> bool
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    if(!sizeIsBelowCapacity())
    {
      return false;
    }

    accessor.push(value);
    dataCondVar.notify_one();
  }
  // -------- end critical section -------- //

  return true;
}

/**
 * @brief Attempts to move another element into the queue. If the size of the container
 *        doesn't exceed the container's capacity, the new element is moved into the queue and
 *        the return value is true. Otherwise, it is false.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::tryPush(value_type&& value) -> bool
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    if(!sizeIsBelowCapacity())
    {
      return false;
    }

    accessor.push(std::move(value));
    dataCondVar.notify_one();
  }
  // -------- end critical section -------- //

  return true;
}

/**
 * @brief Attempts to copy a new element into the queue. Blocks until the spcified time out
 *        has elapsed or the lock is aquired. If the lock is successfully aquired and
 *        the queue is active, the element is inserted into the queue and the return value is true.
 *        Otherwise, false is returned.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Rep, typename Period>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::tryPushFor(value_type const& value,
                                                                          std::chrono::duration<Rep, Period> const& timeOut) -> bool
{
  // ------- begin critical section ------- //
  {
    uniqueLock lk(dataMutex);
    if(capacityCondVar.wait_for(lk, timeOut, [this] { return sizeIsBelowCapacity() || !isActive; }))
    {
      if(isActive)
      {
        accessor.push(value);
        dataCondVar.notify_one();
        return true;
      }
    }
  }
  // -------- end critical section -------- //

  return false;
}

/**
 * @brief Attempts to move a new element into the queue. Blocks until the spcified time out
 *        has elapsed or the lock is aquired. If the lock is successfully aquired and
 *        the queue is active, the element is moved into the queue and the return value is true.
 *        Otherwise, false is returned.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Rep, typename Period>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::tryPushFor(value_type&& value,
                                                                          std::chrono::duration<Rep, Period> const& timeOut) -> bool
{
  // ------- begin critical section ------- //
  {
    uniqueLock lk(dataMutex);
    if(capacityCondVar.wait_for(lk, timeOut, [this] { return sizeIsBelowCapacity() || !isActive; }))
    {
      if(isActive)
      {
        accessor.push(std::move(value));
        dataCondVar.notify_one();
        return true;
      }
    }
  }
  // -------- end critical section -------- //

  return false;
}

/**
 * @brief constructs a new element in-place.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename... Args, typename>
void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::emplace(Args&&... args)
{
  // ------- begin critical section ------- //
  uniqueLock lk(dataMutex);
  capacityCondVar.wait(lk, [this] { return sizeIsBelowCapacity() || !isActive;});

  if(isActive)
  {
    accessor.emplace(std::forward<Args>(args)...);
    dataCondVar.notify_one();
  }
}

/**
 * @brief blocks the calling thread until the queue is not empty
 *        and moves the element at the front of the queue into the passed value.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::waitAndPop(value_type& value)
{
  // ------- begin critical section ------- //
  uniqueLock lk(dataMutex);
  dataCondVar.wait(lk, [this] { return hasData() || !isActive; });

  if(isActive)
  {
    accessor.pop(value);
    capacityCondVar.notify_all();
  }
}

/**
 * @brief if the the queue is not empty, the first element at the front of the queue
 *        is moved into the passed value and the method returns true.
 *        If the queue is empty, the method returns false immediately
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::tryPop(value_type& value) -> bool
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    if(data.empty())
    {
      return false;
    }
    accessor.pop(value);
    capacityCondVar.notify_all();
  }
  // -------- end critical section -------- //

  return true;
}

/**
 * @brief Attempts to pop an element from the queue. If the specified time out
 *        elapses and the queue is not empty, the first element of the queue is moved
 *        into the given value and true is returned. Otherwise, this method return returns false.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Rep, typename Period>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::tryPopFor(value_type& value,
                                                                         std::chrono::duration<Rep, Period> const& timeOut) -> bool
{
  // ------- begin critical section ------- //
  {
    uniqueLock lk(dataMutex);
    if(dataCondVar.wait_for(lk, timeOut, [this] { return hasData() || !isActive; }))
    {
      if(isActive)
      {
        accessor.pop(value);
        capacityCondVar.notify_all();
        return true;
      }
    }
  }
  // -------- end critical section -------- //

  return false;
}

/**
 * @brief Exchanges the contents of the container with those of other.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::swap(ThreadsafeQueueT& other) noexcept
{
  // ------- begin critical section ------- //
  lock lkThis(dataMutex);
  lock lkOther(other.dataMutex);
  data.swap(other.data);
  std::swap(dataCapacity, other.dataCapacity);
}

/**
 * @brief stops all threads that might block on pop or push methods.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::stopQueue()
{
  isActive = false;
  dataCondVar.notify_all();
  capacityCondVar.notify_all();
}

/**
 * @brief checks whether the queue contains an element which fulfills a given propery.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Predicate>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::contains(Predicate&& predicate) const -> bool
{
  // ------- begin critical section ------- //
  lock lk(dataMutex);
  return std::find_if(accessor.cbegin(), accessor.cend(), predicate) != accessor.cend();
}

/**
 * @brief checks whether the front element of the queue (which is the first element to be removed by a call to pop)
 *        fulfills a given propery.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Predicate>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::hasTopProperty(Predicate&& predicate) const -> bool
{
  // ------- begin critical section ------- //
  lock lk(dataMutex);
  return std::forward<Predicate>(predicate)(accessor.front());
}

/**
 * @brief  Removes all element that satisfy a specified criteria.
 * @return The number of elements that have been removed.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Predicate>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::removeIf(Predicate&& predicate) -> size_type
{
  size_type numRemovedElements{};

  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    const auto sizeBefore = data.size();
    data.remove_if(std::forward<Predicate>(predicate)), data.end();

    numRemovedElements = sizeBefore - data.size();
    if(numRemovedElements > 0ULL)
    {
      capacityCondVar.notify_all();
    }
  }
  // -------- end critical section -------- //

  return numRemovedElements;
}

/**
 * @brief   Searches for the first element in the queue that satifies a given criteria and moves it into
 *          the given value.
 * @returns true, if an element has been found. False otherwise.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Predicate>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::extractIf(value_type& value, Predicate&& predicate) -> bool
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);

    if(auto elem = std::find_if(accessor.begin(), accessor.end(), predicate);
       elem != accessor.end())
    {
      value = std::move(*elem);
      accessor.erase(elem);
      capacityCondVar.notify_all();
      return true;
    }
  }
  // -------- end critical section -------- //

  return false;
}

/**
 * @brief Searches all elements satisfying a given criteria, removes the from the queue and moves them
 *        into a list.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Predicate>
inline auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::extractIf(Predicate&& predicate) -> std::list<value_type>
{
  // ------- begin critical section ------- //
  lock lk(dataMutex);
  return extractIfImpl(std::forward<Predicate>(predicate));
}

template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename Predicate>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::extractIfImpl(Predicate&& predicate) -> std::list<value_type>
{
  std::list<value_type> extractedElements;

  //const auto firstRemoved = std::remove_if(data.begin(), data.end(), predicate);
  //std::move(firstRemoved, data.end(), std::back_inserter(extractedElements));
  //data.erase(firstRemoved, data.end());

  //auto elem = std::find_if(accessor.begin(), accessor.end(), predicate);
  //while(elem != accessor.end())
  //{
  //  extractedElements.push_back(std::move(*elem));
  //  const auto toErase = elem;
  //  std::advance(elem, 1ULL);
  //  accessor.erase(toErase);
  //  elem = std::find_if(elem, accessor.end(), predicate);
  //}

  auto elem = std::find_if(data.begin(), data.end(), predicate);
  while(elem != data.end())
  {
    extractedElements.push_front(std::move(*elem));
    const auto toErase = elem;
    std::advance(elem, 1ULL);
    data.erase(toErase);
    elem = std::find_if(elem, data.end(), predicate);
  }

  if(!extractedElements.empty())
  {
    capacityCondVar.notify_all();
  }

  return extractedElements;
}

/**
 * @brief   Applies a given function object to every element in the queue.
 * @returns the passed function object.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename F>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::forEach(F&& f) const -> F
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    for(const auto& value : accessor)
    {
      std::forward<F>(f)(value);
    }
  }
  // -------- end critical section -------- //

  return std::forward<F>(f);
}

/**
 * @brief invokes a given method on the first element in the queue.
 *        If the queue ist empty a std::out_of_range exception is thrown.
 * @throw std::out_of_range.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename V, typename>
void ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::visitTop(V&& visitor)
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    if(!data.empty())
    {
      std::forward<V>(visitor)(accessor.front());
      return;
    }
  }
  // -------- end critical section -------- //

  throw std::out_of_range("cannot visit top element in empty queue");
}

/**
 * @brief invokes a given method on the first element in the queue and returns the result of the call.
 *        If the queue is the provided default value is returned.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename V, typename Res, typename>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::visitTop(V&& visitor, Res const& defaultValue) -> Res
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);
    if(!data.empty())
    {
      return std::forward<V>(visitor)(accessor.front());
    }
  }
  // -------- end critical section -------- //

  return defaultValue;
}

/**
 * @brief extracts all elements satisfying a given criteria, applies the visitor to all these elements
 *        and reinserts them afterwards.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename V, typename Predicate, typename>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::extractVisitReinsert(V&& visitor, Predicate&& predicate) -> size_type
{
  size_type numHosts{};
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);

    auto elementsToVisit = extractIfImpl(std::forward<Predicate>(predicate));
    numHosts = elementsToVisit.size();
    for(auto& elem : elementsToVisit)
    {
      std::forward<V>(visitor)(elem);
      accessor.push(std::move(elem));
    }
  }
  // -------- end critical section -------- //

  return numHosts;
}

/**
 * @brief  extracts the first element satisfying a given criteria, applies the visitor to this element
 *         and reinserts it afterwards.
 * @return a std::optional holding either the return value of the visitor if an element was found
 *         or a std::nullopt. 
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
template <typename V, typename Predicate, typename Res, typename>
auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::extractVisitReinsert(V&& visitor, Predicate&& predicate) 
  -> std::optional<Res>
{
  // ------- begin critical section ------- //
  {
    lock lk(dataMutex);

    if(auto elementToVisit = std::find_if(accessor.begin(), accessor.end(), std::forward<Predicate>(predicate));
       elementToVisit != accessor.end())
    {
      value_type copy{std::move(*elementToVisit)};
      accessor.erase(elementToVisit);
      const auto retVal = std::forward<V>(visitor)(copy);
      accessor.push(std::move(copy));

      return retVal;
    }

    //if(auto elementToVisit = std::find_if(data.rbegin(), data.rend(), std::forward<Predicate>(predicate));
    //   elementToVisit != data.rend())
    //{
    //  value_type copy{std::move(*elementToVisit)};      
    //  data.erase(std::prev(elementToVisit.base()));
    //  const auto retVal = std::forward<V>(visitor)(copy);
    //  accessor.push(std::move(copy));

    //  return retVal;
    //}
  }
  // -------- end critical section -------- //

  return std::nullopt;
}

/**
 * @brief checks whether the size of the container is below the capacity.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::sizeIsBelowCapacity() const -> bool
{
  return data.size() < dataCapacity;
}

/**
 * @brief checks whether the container has at least one element.
 */
template <typename T, template <typename...> typename Container,
          insertionPolicy InsertionPolicy, typename Compare>
inline auto ThreadsafeQueueT<T, Container, InsertionPolicy, Compare>::hasData() const -> bool
{
  return !data.empty();
}


}  // namespace cctools


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif   // THREADSAFEQUEUE_H_14637657521676284412325020248211542122228070
