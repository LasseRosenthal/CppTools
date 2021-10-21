/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   TMPUtils.h
 *         
 * @author Lasse Rosenthal
 * @date   08.07.2019
 */

#ifndef TMPUTILS_H_285943097684962886611355129523320117775144
#define TMPUTILS_H_285943097684962886611355129523320117775144


// includes
#include <type_traits>
#include <utility>


#if defined(_MSC_VER) && _MSC_VER >= 1900
using std::void_t;
#else
template <typename...>
using void_t = void;
#endif

namespace cctools {
namespace tmp {


/**
 * @struct IsSharedMutexT
 * @brief  IsSharedMutexT is a metafunction that detects if a given mutex type
 *         offers a shared locking facililty.
 */
template <typename Mutex, typename = std::void_t<>>
struct IsSharedMutexT : std::false_type
{};

template <typename Mutex>
struct IsSharedMutexT<Mutex, std::void_t<decltype(&Mutex::lock_shared)>> : std::true_type
{};

template <typename Mutex>
constexpr bool IsSharedMutex = IsSharedMutexT<Mutex>::value;


template <typename T>
struct FunctionTraits : FunctionTraits<decltype(&T::operator())> {};


template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(ClassType::*)(Args...) const> {

  using result_type = ReturnType;

  template <std::size_t index>
  struct arg {
    // the i-th argument is equivalent to the i-th tuple element of a tuple
    // composed of those arguments.
    using type = typename std::tuple_element<index, std::tuple<Args...>>::type;
  };
};


}   // namespace tmp
}   // namespace cctools


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif   // TMPUTILS_H_285943097684962886611355129523320117775144
