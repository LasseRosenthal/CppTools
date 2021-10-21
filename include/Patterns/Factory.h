/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Factory.h 
 * @brief   Factory provides a generic implementation of the factory pattern.
 * 
 * @author  Lasse Rosenthal 
 * @date    09.08.2021 
 */ 
 
#ifndef FACTORY_H_3015858361218725988101731802518123192327585 
#define FACTORY_H_3015858361218725988101731802518123192327585 
 
 
// includes
#include <Meta/Utility.h>

#include <memory>
#include <unordered_map>
 

namespace patterns {


/**
 * @class Factory
 * @brief
 */
template <typename AbstractType, typename Identifier,
          typename Builder = std::unique_ptr<AbstractType>(*)()>
class Factory {

public:

  // ---------------------------------------------------
  // public types
  using product_type = std::unique_ptr<AbstractType>;
  using id_type      = Identifier;

  // ---------------------------------------------------
  // public api
  template <typename... Args>
  auto create(Identifier id, Args&&... args) const -> product_type;

  static auto registerBuilder(Identifier id, Builder builder) -> bool;

private:

  // ---------------------------------------------------
  inline static std::unordered_map<Identifier, Builder> creators;
};


/**
 * @brief registers a factory method to create a product for a given id
 */
template <typename AbstractType, typename Identifier, typename Builder>
auto Factory<AbstractType, Identifier, Builder>::registerBuilder(Identifier id, Builder builder) -> bool
{
  if(auto creator = creators.find(id); creator == creators.end())
  {
    creators[id] = std::move(builder);
    return true;
  }

  return false;
}

/**
 * @brief invokes the factory method for the given id
 */
template <typename AbstractType, typename Identifier, typename Builder>
template <typename... Args>
auto Factory<AbstractType, Identifier, Builder>::create(Identifier id, Args&&... args) const -> product_type
{
  if(auto creator = creators.find(id); creator != creators.end())
  {
    return creator->second(std::forward<Args>(args)...);
  }
}


}   // namespace patterns


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FACTORY_H_3015858361218725988101731802518123192327585 
