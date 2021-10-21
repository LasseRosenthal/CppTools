/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   ConcurrencyToolsConfig.h
 *         
 * @author Lasse Rosenthal
 * @date   04.07.2019
 */

#ifndef CONCURRENCYTOOLSCONFIG_H_9270386815230541196173967288701432910039
#define CONCURRENCYTOOLSCONFIG_H_9270386815230541196173967288701432910039


// disable warning due to macro redefinition
#pragma warning(disable : 4005)

#ifdef _MSC_VER

#if _MSC_VER <= 1900

#include <mutex>

namespace std {
using shared_mutex = std::mutex;

template <typename MutexType>
using shared_lock = std::lock_guard<MutexType>;

}   // namespace std

#elif _MSC_VER <= 1920

#include <shared_mutex>
namespace std {
using shared_mutex = std::shared_timed_mutex;
}   // namespace std

#define SHAREDMUTEXAVAILABLE

#else

#define SHAREDMUTEXAVAILABLE

#endif

#endif


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif   // CONCURRENCYTOOLSCONFIG_H_9270386815230541196173967288701432910039
