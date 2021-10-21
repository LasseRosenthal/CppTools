/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   FunctionWrapper.h
 * @brief  FunctionWrapper is a type-erase class for callable objects.
 *         for an explanation of type-erasure classes see
 *         https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure
 *         
 * @author Lasse Rosenthal
 * @date   21.01.2020
 */

#ifndef FUNCTIONWRAPPER_H_29803210782190253302196619340169112028611706 
#define FUNCTIONWRAPPER_H_29803210782190253302196619340169112028611706 
 
 
// includes 
#include <memory>


namespace cctools {
 
 
/**
 * @class FunctionWrapper
 * @brief
 */
class FunctionWrapper {

  // ---------------------------------------------------
  // type erasure base type
  struct CallableBase {

    virtual ~CallableBase () = default;
    virtual void call () = 0;
  };

  // ---------------------------------------------------
  // the polymorphic type erasure type
  template <typename Fun>
  struct Callable : CallableBase {
    Callable  (Fun&&); 
    void call () override;
    Fun fun; 
  };

public:

  // ---------------------------------------------------
  // special member functions
  FunctionWrapper () = default;
  FunctionWrapper (const FunctionWrapper&) = delete;
  FunctionWrapper (FunctionWrapper&&) = default;
  auto operator=  (const FunctionWrapper&) -> FunctionWrapper& = delete;
  auto operator=  (FunctionWrapper&&) -> FunctionWrapper& = default;

  template <typename Fun>
  FunctionWrapper (Fun&&, int priority = 0);

  // ---------------------------------------------------
  // public api
  void operator ()();

  friend auto operator< (FunctionWrapper const& f1, FunctionWrapper const& f2) -> bool;

private:

  // --------------------------------------------------- 
  // a pointer to the callable
  std::unique_ptr<CallableBase> callable;
  int                           priority {0};
};


/**
 * @brief Constructor. Moves the provided function into the Callable.
 */
template <typename Fun>
inline FunctionWrapper::Callable<Fun>::Callable(Fun&& f) :
  fun {std::move(f)}
{}

/**
 * @brief Constructor. Creates the internal callable object from the given function.
 */
template <typename Fun>
inline FunctionWrapper::FunctionWrapper(Fun&& fun, int priority)
  : callable {std::make_unique<Callable<Fun>>(std::forward<Fun>(fun))}
  , priority {priority}
{}

/**
 * @brief Calls the stored function.
 */
template <typename Fun>
inline void FunctionWrapper::Callable<Fun>::call()
{
  fun();
}

/**
 * @brief Function call operator. Calls the stored callable object
 */
inline void FunctionWrapper::operator()()
{
  callable->call();
}

/**
 * @brief less operator.
 */
inline auto operator<(FunctionWrapper const& f1, FunctionWrapper const& f2) -> bool
{
  return f1.priority < f2.priority;
}


}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FUNCTIONWRAPPER_H_29803210782190253302196619340169112028611706 
