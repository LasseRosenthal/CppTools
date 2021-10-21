/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    SingletonBase.h 
 * @brief   A generic implementation of the Singleton pattern.
 * 
 * @author  Lasse Rosenthal 
 * @date    30.09.2021 
 */ 
 
#ifndef SINGLETONBASE_H_16401504032000276352256431592248982743723475 
#define SINGLETONBASE_H_16401504032000276352256431592248982743723475 
 
 
// includes
#include <functional>
#include <tuple>
#include <utility>
 

namespace cpptools {


/// Enumeration to specify the construction policy of the singleton
enum class singletonMode {
  lazy,  ///< the singleton object is created upon the first call of instance.
  eager  ///< the singleton object is created once the initialize method is called.
};


/** 
 * @class  SingletonBase
 * @brief  SingletonBase provides a generic implementation of the Singleton pattern
 *         based on the curiously recurring template pattern.
 * @tparam T the singleton type
 * @tparam Mode a value of type /link #singletonMode /endlink that specifies if the
 *         the singleton object is created upon the first call of instance or before.
 * @tparam EnableWithoutInit if set to true, the call of initialize is not mandatory
 *         but possible.
 * @remark In order to enable SingletonBase to create the singleton object it has to
 *         be declared as a friend class of the Singleton type.
 * @see    https://en.wikipedia.org/wiki/Singleton_pattern
 * @see    https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */
template <class T, singletonMode Mode, bool EnableWithoutInit = false>
class SingletonBase {

  // ---------------------------------------------------
  // types
  using singletonType = T;
  using reference     = singletonType&;
  using creator       = std::function<reference()>;

  // ---------------------------------------------------
  // constants
  static constexpr auto mode = Mode;
  static constexpr bool enableWithoutInit = EnableWithoutInit;

public:

  // ---------------------------------------------------
  // access function
  static auto instance   () -> reference;
  template <typename... Params>
  static void initialize (Params&&... params);

protected:

  // ---------------------------------------------------
  // default constructor and destructor only accessible
  // for the singleton class.
  SingletonBase  () = default;
  ~SingletonBase () = default;

private:

  // ---------------------------------------------------
  // deleted copy construction and assignment
  SingletonBase  (SingletonBase const&) = delete;
  SingletonBase  (SingletonBase&&) = delete;
  auto operator= (SingletonBase const&) -> SingletonBase& = delete;
  auto operator= (SingletonBase&&) -> SingletonBase& = delete;

  // ---------------------------------------------------
  // private helpers
  template <typename Tuple>
  static decltype(auto) createInstance (Tuple&& paramsTuple);
  template <typename Tuple, std::size_t... Is>
  static auto createInstance           (Tuple&& paramsTuple, std::index_sequence<Is...>) -> reference;

  // ---------------------------------------------------
  // static creation object
  inline static creator theCreator;
};


/**
 * @brief convenient alias for lazy singletons.
 */
template <typename T, bool EnableWithoutInit = false>
using LazySingleton = SingletonBase<T, singletonMode::lazy, EnableWithoutInit>;

/**
 * @brief convenient alias for eager singletons.
 */
template <typename T, bool EnableWithoutInit = false>
using EagerSingleton = SingletonBase<T, singletonMode::eager, EnableWithoutInit>;


/**
 * @brief  Returns a reference to the singleton object.
 * @remark if /link #singletonMode /endlink is lazy, the singleton object
 *         is created upon the first call of instance.
 */
template <class T, singletonMode Mode, bool EnableWithoutInit>
inline auto SingletonBase<T, Mode, EnableWithoutInit>::instance() -> reference
{
  if constexpr(enableWithoutInit)
  {
    static auto& instanceRef = theCreator ? theCreator() : createInstance(std::tuple<>{});
    return instanceRef;  
  }
  else
  {
    static auto& instanceRef = theCreator();
    return instanceRef;
  }
}

/**
 * @brief Creates a factory object that is used to create the instance of the singleton.
 * @param params a parameter pack that is forwarded to the constructor of the singleton object.
 */
template <typename T, singletonMode Mode, bool EnableWithoutInit>
template <typename... Params>
static void SingletonBase<T, Mode, EnableWithoutInit>::initialize(Params&&... params)
{
  if(!theCreator)
  {
    theCreator = [paramsTuple = std::make_tuple(std::forward<Params>(params)...)]() -> reference {
      static auto& theInstance = createInstance(std::move(paramsTuple));
      return theInstance;
    };

    if constexpr(mode == singletonMode::eager)
    {
      theCreator();
    }
  }
}

/**
 * @brief creates an instance of the singleton object forwarding the elements
 *        of the tuple as the arguments to the constructor.
 */
template <typename T, singletonMode Mode, bool EnableWithoutInit>
template <typename Tuple>
inline decltype(auto) SingletonBase<T, Mode, EnableWithoutInit>::createInstance(Tuple&& paramsTuple)
{
  return createInstance(std::forward<Tuple>(paramsTuple),
                        std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <typename T, singletonMode Mode, bool EnableWithoutInit>
template <typename Tuple, std::size_t... Is>
inline auto SingletonBase<T, Mode, EnableWithoutInit>::createInstance(Tuple&& paramsTuple, std::index_sequence<Is...>) -> reference
{
  static singletonType theInstance(std::get<Is>(std::forward<Tuple>(paramsTuple))...);
  return theInstance;
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // SINGLETONBASE_H_16401504032000276352256431592248982743723475 
