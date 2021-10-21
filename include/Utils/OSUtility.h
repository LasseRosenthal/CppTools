/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    OSUtility.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    05.05.2021 
 */ 
 
#ifndef OSUTILITY_H_2053040712930715703158471177322661371731586 
#define OSUTILITY_H_2053040712930715703158471177322661371731586 
 
 
// includes
#include <stdexcept>
#include <string>


namespace osutility {


/** 
 * @brief Exception type indicating that an environment variable was not found.
 */
class environmentvariableNotFound : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/** 
 * @brief Retrieves the content of the specified environment variable.
 *        If the environment variable does not exist, an exception
 *        of type std::runtime_error is thrown.
 * @throw environmentvariableNotFound, std::runtime_error
 */
auto getEnvironmentVariable(std::wstring const& name) -> std::wstring;

/** 
 * @brief Sets the contents of the specified environment variable for the current process.
 */
auto setEnvironmentVariable(std::wstring const& name, std::wstring const& value) -> bool;

/** 
 * @brief Checks if an environment variable with the given name exists.
 */
auto existsEnvironmentVariable(std::wstring const& name) -> bool;


}   // namespace osutility


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // OSUTILITY_H_2053040712930715703158471177322661371731586 
