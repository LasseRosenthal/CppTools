/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FrameworkStack.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    16.09.2021 
 */ 
 
#ifndef FRAMEWORKSTACK_H_1711620253255481608721023716982852603230005 
#define FRAMEWORKSTACK_H_1711620253255481608721023716982852603230005 
 
 
// includes
#include <Utils/Framework.h>

#include <stack>


namespace cpptools {
 
 
/** 
 * @class FrameworkStack
 * @brief 
 */
class FrameworkStack {

public:

  // ---------------------------------------------------
  // public types
  using value_type     = Framework::ptr;
  using container_type = std::stack<value_type>;
  using size_type      = container_type::size_type;

  // ---------------------------------------------------
  // special member functions
  FrameworkStack  () = default;
  FrameworkStack  (FrameworkStack const&) = delete;
  FrameworkStack  (FrameworkStack&&) = default;
  auto operator=  (FrameworkStack const&) -> FrameworkStack& = delete;
  auto operator=  (FrameworkStack&&) -> FrameworkStack& = default;
  ~FrameworkStack ();

  // ---------------------------------------------------
  // public methods
  [[nodiscard]] auto size  () const -> size_type;
  [[nodiscard]] auto empty () const noexcept -> bool;
  template <typename FrameworkT, typename... Args>
  [[nodiscard]] auto push  (Args&&... args) -> bool;
  void clear               ();
  [[nodiscard]] auto good  () const noexcept -> bool;

private:

  // ---------------------------------------------------
  // private data
  container_type frameworks;
  bool success{true};
};

/** 
 * @brief Returns the number of frameworks in the stack.
 */
inline [[nodiscard]] auto FrameworkStack::size() const -> size_type
{
  return frameworks.size();
}

/** 
 * @brief Checks whether the framework stack is empty.
 */
inline [[nodiscard]] auto FrameworkStack::empty() const noexcept -> bool
{
  return frameworks.empty();
}

/** 
 * @brief  Constructs a new framework of a given type and inserts it into the stack.
 * @tparam FrameworkT the type of the framework.
 * @param  Args a parameter pack that is passed to the constructor of the framework.
 * @return returns true if the framework has successfully been initialized and inserted into the stack.
 *         If initialization has failed, the framework object is not inserted into the stack
 *         and false is returned.
 */
template <typename FrameworkT, typename... Args>
[[nodiscard]] auto FrameworkStack::push(Args&&... args) -> bool
{
  auto framework = Framework::create<FrameworkT>(std::forward<Args>(args)...);
  if(framework->isInitialized())
  {
    frameworks.push(std::move(framework));
    return true;
  }

  success = false;
  return false;
}

/** 
 * @brief Returns true if all frameworks have been successfully initialized.
 */
inline [[nodiscard]] auto FrameworkStack::good() const noexcept -> bool
{
  return success;
}

/** 
 * @brief Destroys the frameworks in the reverse order of their creation.
 */
inline void FrameworkStack::clear()
{
  while(!frameworks.empty())
  {
    frameworks.pop();
  }
}

/** 
 * @brief Destructor. Destroys the frameworks in the reverse order of their creation.
 */
inline FrameworkStack::~FrameworkStack()
{
  clear();
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FRAMEWORKSTACK_H_1711620253255481608721023716982852603230005 
