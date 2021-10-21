/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   ThreadingModel.h
 * @brief  ThreadingModel defines three different threading policies
 *         that can be used via the Curiously recurring template pattern
 * @see    https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 * @see    https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick
 *         
 * @author Lasse Rosenthal
 * @date   18.07.2019
 */

#ifndef THREADINGMODEL_H_14642273801559829167286891135103601245413359 
#define THREADINGMODEL_H_14642273801559829167286891135103601245413359 
 
 
// includes 
#include "ConcurrencyToolsConfig.h"
#include "TMPUtils.h"

#include <Utils/CRTP.h>

#include <mutex>
#ifdef SHAREDMUTEXAVAILABLE
#  include <shared_mutex>
#endif

#include <type_traits>


namespace cctools {


/// Enumeration to specify locking policies
enum class lockingPolicy : char {
  standard, ///< standard locking
  unique,   ///< unique locking
  shared    ///< shared locking
};

/** 
 * @brief struct GuardType is an alias helper template providing a lock_guard type
 *        based on a given Mutex and a \link #lockingPolicy \endlink.
 */
template <typename Mutex, lockingPolicy p>
using GuardType = std::conditional_t<p == lockingPolicy::standard, std::lock_guard<Mutex>,
                                     std::conditional_t<p == lockingPolicy::unique, std::unique_lock<Mutex>, std::shared_lock<Mutex>>>;


/**
 * @brief ObjectLevelLockable provides object-level locking semantics 
 */
template <typename Host, typename Mutex, typename SizeConstant, typename = void>
class ObjectLevelLockableT : cpptools::CRTP<Host, ObjectLevelLockableT, Mutex, SizeConstant, void> {

protected:

  // ---------------------------------------------------
  // types
  using Lock       = GuardType<Mutex, lockingPolicy::standard>;
  using LockUnique = GuardType<Mutex, lockingPolicy::unique>;

  // ---------------------------------------------------
  // constants
  static constexpr bool hasSharedLocking  = false;
  static constexpr std::size_t numMutexes = SizeConstant::value;

  // ---------------------------------------------------
  // convenience methods for locking
  [[nodiscard]] auto lock       (std::size_t const i = 0ULL) const -> Lock       { return Lock{mutexes[i]};       };
  [[nodiscard]] auto lockUnique (std::size_t const i = 0ULL) const -> LockUnique { return LockUnique{mutexes[i]}; };

  mutable Mutex mutexes[numMutexes];
};

template <typename Host, typename Mutex, typename SizeConstant>
class ObjectLevelLockableT<Host, Mutex, SizeConstant, std::enable_if_t<tmp::IsSharedMutex<Mutex>>>
  : cpptools::CRTP<Host, ObjectLevelLockableT, Mutex, SizeConstant, void> {

protected:

  // ---------------------------------------------------
  // types
  using Lock       = GuardType<Mutex, lockingPolicy::standard>;
  using LockUnique = GuardType<Mutex, lockingPolicy::unique>;
  using LockShared = GuardType<Mutex, lockingPolicy::shared>;

  // ---------------------------------------------------
  // constants
  static constexpr bool hasSharedLocking  = true;
  static constexpr std::size_t numMutexes = SizeConstant::value;

  // ---------------------------------------------------
  // convenience methods for locking
  [[nodiscard]] auto lock       (std::size_t const i = 0ULL) const -> Lock       { return Lock{mutexes[i]};       };
  [[nodiscard]] auto lockUnique (std::size_t const i = 0ULL) const -> LockUnique { return LockUnique{mutexes[i]}; };
  [[nodiscard]] auto lockShared (std::size_t const i = 0ULL) const -> LockShared { return LockShared{mutexes[i]}; };

  mutable Mutex mutexes[numMutexes];
};

template <typename Host, typename Mutex = std::mutex, std::size_t Size = 1ULL>
using ObjectLevelLockable = ObjectLevelLockableT<Host, Mutex, std::integral_constant<std::size_t, Size>>;


/**
 * @brief ClassLevelLockable provides class-level locking semantics via CRTP 
 */
template <typename Host, typename Mutex, typename SizeConstant, typename Enable = void>
class ClassLevelLockableT : cpptools::CRTP<Host, ClassLevelLockableT, Mutex, void> {

protected:

  // ---------------------------------------------------
  // types
  using Lock       = GuardType<Mutex, lockingPolicy::standard>;
  using LockUnique = GuardType<Mutex, lockingPolicy::unique>;

  // ---------------------------------------------------
  // constants
  static constexpr bool hasSharedLocking  = false;
  static constexpr std::size_t numMutexes = SizeConstant::value;

  // ---------------------------------------------------
  // convenience methods for locking
  [[nodiscard]] auto lock       (std::size_t const i = 0ULL) const -> Lock       { return Lock{mutexes[i]};       };
  [[nodiscard]] auto lockUnique (std::size_t const i = 0ULL) const -> LockUnique { return LockUnique{mutexes[i]}; };

  inline static Mutex mutexes[numMutexes];
};

template <typename Host, typename Mutex, typename SizeConstant>
class ClassLevelLockableT<Host, Mutex, SizeConstant, std::enable_if_t<tmp::IsSharedMutex<Mutex>>> 
  : cpptools::CRTP<Host, ClassLevelLockableT, Mutex, void> {

protected:

  // ---------------------------------------------------
  // types
  using Lock       = GuardType<Mutex, lockingPolicy::standard>;
  using LockUnique = GuardType<Mutex, lockingPolicy::unique>;
  using LockShared = GuardType<Mutex, lockingPolicy::shared>;

  // ---------------------------------------------------
  // constants
  static constexpr bool hasSharedLocking  = true;
  static constexpr std::size_t numMutexes = SizeConstant::value;

  // ---------------------------------------------------
  // convenience methods for locking
  [[nodiscard]] auto lock       (std::size_t const i = 0ULL) const -> Lock       { return Lock{mutexes[i]};       };
  [[nodiscard]] auto lockUnique (std::size_t const i = 0ULL) const -> LockUnique { return LockUnique{mutexes[i]}; };
  [[nodiscard]] auto lockShared (std::size_t const i = 0ULL) const -> LockShared { return LockShared{mutexes[i]}; };

  inline static Mutex mutexes[numMutexes];
};


template <typename Host, typename Mutex, std::size_t Size = 1ULL>
using ClassLevelLockable = ClassLevelLockableT<Host, Mutex, std::integral_constant<std::size_t, Size>>;


/** 
 * @brief EmptyLock is an empty policy with no locking at all.        
 */
struct EmptyLock {
  constexpr void unlock() const noexcept {};
};

/**
 * @brief SingleThreaded ensures syntactic consistency and is a noop interface.
 */
template <typename Host>
class SingleThreaded {

protected:

  // ---------------------------------------------------
  // EmptyLock is a no-op
  using Lock       = EmptyLock;
  using LockUnique = EmptyLock;
  using LockShared = EmptyLock;

  // ---------------------------------------------------
  // constants
  static constexpr bool hasSharedLocking = false;

  // ---------------------------------------------------
  // convenience methods for locking
  [[nodiscard]] constexpr auto lock       () const noexcept -> Lock       { return {}; };
  [[nodiscard]] constexpr auto lockUnique () const noexcept -> LockUnique { return {}; };
  [[nodiscard]] constexpr auto lockShared () const noexcept -> LockShared { return {}; };
};


}  // namespace cctools

 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // THREADINGMODEL_H_14642273801559829167286891135103601245413359 
