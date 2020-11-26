/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Unit.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    26.11.2020 
 */ 
 
#ifndef UNIT_H_179732885328477293061676127622711153282600 
#define UNIT_H_179732885328477293061676127622711153282600 
 
 
// includes
#include <Unit/UnitSystemGenerator.h>
#include <Unit/detail/UnitSystemConversion.h>
 
 
namespace unit {


/** 
 * @class Unit
 * @brief 
 */
template <typename T, typename UnitSystemT>
class Unit {

public:

  // ---------------------------------------------------
  // public types
  using value_type = T;
  using unitSystem = UnitSystemT;
  using dimension  = typename unitSystem::dimension;

  // ---------------------------------------------------
  // constructor, destructor & assignment
  constexpr Unit() = default;
  constexpr Unit(value_type v) noexcept;

  constexpr auto value() const noexcept -> value_type;

private:

  // private type alias for SFINAE
  template <typename UnitT>
  using RequiresSameDimension = RequiresSameDimension<unitSystem, typename UnitT::unitSystem>;

  // ---------------------------------------------------
  // private data
  value_type val{};
};


/** 
 * @brief CommonType is alias for the common value_type of two Units.
 */
template <typename UnitT1, typename Unit2>
using CommonType = std::common_type_t<typename UnitT1::value_type, typename Unit2::value_type>;

/** 
 * @brief UnitCast converts a Unit to a different Unit with same dimension.
 */
template <typename UnitTo, typename UnitFrom>
auto constexpr UnitCast(UnitFrom const& unitFrom) noexcept -> std::enable_if_t<UnitFrom::dimension::isRational, UnitTo>
{
  using factor = typename ConversionFactorT<typename UnitTo::unitSystem, typename UnitFrom::unitSystem>::factor;
  return UnitTo{unitFrom.value() * meta::AsDecimal<factor, CommonType<UnitTo, UnitFrom>>};

//  return UnitTo{(unitFrom.value() * factor::num) / factor::den};
}

template <typename UnitTo, typename UnitFrom>
auto constexpr UnitCast(UnitFrom const& unitFrom) noexcept -> std::enable_if_t<!UnitFrom::dimension::isRational, UnitTo>
{
  return UnitTo{unitFrom.value() * ConversionFactorT<UnitTo, UnitFrom>::value};
}



/** 
 * @brief parametrized constructor. Initializes the stored value.
 */
template <typename T, typename UnitSystemT>
constexpr Unit<T, UnitSystemT>::Unit(value_type v) noexcept
  : val{v}
{}


/** 
 * @brief Returns the current value.
 */
template <typename T, typename UnitSystemT>
constexpr auto Unit<T, UnitSystemT>::value() const noexcept -> value_type
{
  return val;
}


}   // namespace unit


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNIT_H_179732885328477293061676127622711153282600 
