/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    CompileTimeArithmeticTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    12.11.2020 
 */ 
 
#ifndef COMPILETIMEARITHMETICTEST_H_29563158653231157417907326092810622803283 
#define COMPILETIMEARITHMETICTEST_H_29563158653231157417907326092810622803283 
 
 
// includes
#include <Meta/CompileTimeArithmetic.h>
 
 
using namespace meta;


TEST(CompileTimeArithmetic, PowerPositiveExponent)
{
  constexpr double val = Power<4>(3.25);
  EXPECT_EQ(val, 111.56640625);
}

TEST(CompileTimeArithmetic, PowerNegativeExponent)
{
  constexpr double val = Power<-4>(3.25);
  EXPECT_EQ(val, 1.0/111.56640625);
}

TEST(CompileTimeArithmetic, ExponentZero)
{
  constexpr double val = Power<0>(3.25);
  EXPECT_EQ(val, 1.0);
}

TEST(CompileTimeArithmetic, NthRoot)
{
  constexpr double val = NthRoot<5>(114.3137652768);
  EXPECT_EQ(val, 2.58);
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // COMPILETIMEARITHMETICTEST_H_29563158653231157417907326092810622803283 
