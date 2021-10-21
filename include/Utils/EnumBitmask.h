/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    EnumBitmask.h 
 * @brief   Definition of bitmask operators for enumeration types.
 * 
 * @author  Lasse Rosenthal 
 * @date    16.03.2021 
 */ 
 
#ifndef ENUMBITMASK_H_3083028332627027720231103162323939410112818 
#define ENUMBITMASK_H_3083028332627027720231103162323939410112818 
 
 
// includes
#include <type_traits>
 
 
namespace cpptools {


/** 
 * @struct EnableBitmaskOperations 
 * @brief  EnableBitmaskOperations is a traits that determines if a given enumeration type
 *         can be used as a bitmask type.
 * @remark To enable bitwise operators for a given enumeration type, a specialisation
 *         of EnableBitmaskOperationsT has to be provided for the enumeration type.
 * @see    https://en.cppreference.com/w/cpp/named_req/BitmaskType
 */
template <typename Enum, typename = void>
struct EnableBitmaskOperationsT;

template <typename Enum>
struct EnableBitmaskOperationsT<Enum, std::enable_if_t<std::is_enum_v<Enum>>>
 : std::false_type {};

template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
constexpr bool EnableBitmaskOperations = EnableBitmaskOperationsT<Enum>::value;


}   // namespace cpptools


/** 
 * @brief bitwise and for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
[[nodiscard]] constexpr auto operator&(Enum const e1, Enum const e2) noexcept -> Enum
{
  return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(e1) & static_cast<std::underlying_type_t<Enum>>(e2));
}

/** 
 * @brief bitwise or for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
[[nodiscard]] constexpr auto operator|(Enum const e1, Enum const e2) noexcept -> Enum
{
  return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(e1) | static_cast<std::underlying_type_t<Enum>>(e2));
}

/** 
 * @brief bitwise xor for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
[[nodiscard]] constexpr auto operator^(Enum const e1, Enum const e2) noexcept -> Enum
{
  return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(e1) ^ static_cast<std::underlying_type_t<Enum>>(e2));
}

/** 
 * @brief bitwise negation for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
[[nodiscard]] constexpr auto operator~(Enum const e) noexcept -> Enum
{
  return static_cast<Enum>(~static_cast<std::underlying_type_t<Enum>>(e));
}

/** 
 * @brief bitwise and for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
constexpr auto operator&=(Enum& e1, Enum const e2) noexcept -> Enum
{
  e1 = e1 & e2;
  return e1;
}

/** 
 * @brief bitwise or for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
constexpr auto operator|=(Enum& e1, Enum const e2) noexcept -> Enum
{
  e1 = e1 | e2;
  return e1;
}

/** 
 * @brief bitwise xor for enumeration types.
 */
template <typename Enum, typename = std::enable_if_t<cpptools::EnableBitmaskOperations<Enum>>>
constexpr auto operator^=(Enum& e1, Enum const e2) noexcept -> Enum
{
  e1 = e1 ^ e2;
  return e1;
}


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ENUMBITMASK_H_3083028332627027720231103162323939410112818 
