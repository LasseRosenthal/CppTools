/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Framework.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    09.02.2021 
 */ 
 
#ifndef FRAMEWORK_H_1955686817175266185822379617245450023791 
#define FRAMEWORK_H_1955686817175266185822379617245450023791 
 
 
// includes
#include <memory>
#include <string>
#include <type_traits>
 

namespace cpptools {

 
/** 
 * @class Framework
 * @brief Framework provides 
 */
class Framework {

public:

  // ---------------------------------------------------
  // public types
  using ptr = std::unique_ptr<Framework>;

  // ---------------------------------------------------
  // virtual destructor for inheritance
  virtual ~Framework() = default;

  // ---------------------------------------------------
  // public api
  [[nodiscard]] auto isInitialized () const noexcept -> bool;
  [[nodiscard]] auto errorMsg      () const -> std::string const&;

  // static factory method
  template <typename FrameworkT, typename... Args,
            typename = typename std::enable_if_t<std::is_base_of_v<Framework, FrameworkT>>>
  static auto create(Args&&... args) -> ptr;

protected:

  // ---------------------------------------------------
  // construction
  Framework      () = default;
  Framework      (Framework const&) = delete;
  Framework      (Framework&&) = delete;
  auto operator= (Framework const&) -> Framework& = delete;
  auto operator= (Framework&&) -> Framework& = delete;

  // ---------------------------------------------------
  // data
  bool        success{true};
  std::string errMsg;
};


/**
 * @brief Returns true if the framework has been successfully initialized.
 */
inline [[nodiscard]] auto Framework::isInitialized() const noexcept -> bool
{ 
  return success;
}

/**
 * @brief Returns a const const reference to the error message string.
 */
inline [[nodiscard]] auto Framework::errorMsg() const -> std::string const&
{ 
  return errMsg;
}

/**
 * @brief static creation method. Only enabled for types that inherit from Framework.
 */
template <typename FrameworkT, typename... Args, typename>
inline auto Framework::create(Args&& ... args) -> ptr
{
  return ptr{new FrameworkT(std::forward<Args>(args)...)};
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FRAMEWORK_H_1955686817175266185822379617245450023791 
