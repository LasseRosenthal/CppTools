/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    LambdaOverload.h 
 * @brief   Definition of the LambdaOverload structure.
 * 
 * @author  Lasse Rosenthal 
 * @date    01.07.2020 
 */ 
 
#ifndef LAMBDAOVERLOAD_H_1492521466316522032614699141911374385230017 
#define LAMBDAOVERLOAD_H_1492521466316522032614699141911374385230017 
 
 
namespace cpptools {

 
/** 
 * @class LambdaOverload
 * @brief LambdaOverload can be used to create a set of lambda overloads.
 */
template <typename... Callables>
struct LambdaOverload : Callables...
{
  using Callables::operator()...;
};


// deduction guide
template <typename... Callables>
LambdaOverload(Callables...) -> LambdaOverload<Callables...>;


}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // LAMBDAOVERLOAD_H_1492521466316522032614699141911374385230017 
