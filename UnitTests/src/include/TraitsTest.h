/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TraitsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    04.06.2021 
 */ 
 
#ifndef TRAITSTEST_H_15806229612496416372266523012024722756312233 
#define TRAITSTEST_H_15806229612496416372266523012024722756312233 
 
 
// includes
#include <Meta/Traits.h>

#include <string>
 

class Callable {
public:
  int operator()(int, double)
  {
    return 2;
  }
};

class NonCallable {};
 
TEST(Traits, IsCallable)
{
  EXPECT_TRUE((meta::IsCallable<Callable>));
  EXPECT_FALSE((meta::IsCallable<NonCallable>));
  EXPECT_FALSE((meta::IsCallable<double>));
  EXPECT_FALSE((meta::IsCallable<std::string>));
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TRAITSTEST_H_15806229612496416372266523012024722756312233 
