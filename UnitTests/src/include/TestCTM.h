/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestCTM.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    29.09.2020 
 */ 
 
#ifndef TESTCTM_H_2278416328555301461995117149265571425728524 
#define TESTCTM_H_2278416328555301461995117149265571425728524 
 
 
// includes
#include <TransformationMatrix/CTM.h>

using namespace cpptools;


TEST(CTM, DefaultConstructor)
{
  CTM<double> ctm;
  CTM<double>::matrix expectedRot{1.0, 0.0, 0.0, 1.0};
  CTM<double>::vector expectedTrans{0.0, 0.0};
  EXPECT_EQ(ctm.transformationMatrix(), expectedRot);
  EXPECT_EQ(ctm.translation(), expectedTrans);
}

TEST(CTM, ParametrizedConstructor)
{
  CTM<double>::matrix expectedRot{1.0, 1.0, 0.5, 1.0};
  CTM<double>::vector expectedTrans{0.6, 0.7};
  CTM<double> ctm{{1.0, 1.0, 0.5, 1.0}, {0.6, 0.7}};
  EXPECT_EQ(ctm.transformationMatrix(), expectedRot);
  EXPECT_EQ(ctm.translation(), expectedTrans);
}

TEST(CTM, setTranslationComponents)
{
  CTM<double> ctm;
  double const tx = 1.234;
  double const ty = 23.97;
  ctm.setTranslation({tx, ty});
  CTM<double>::vector expectedTrans{tx, ty};
  EXPECT_EQ(ctm.translation(), expectedTrans);
}

//TEST(CTM, setRotationAnglePi)
//{
//  constexpr double pi = 3.14159265358979323846;
//  constexpr double pi2 = 1.57079632679489661923;
//  CTM<double> ctm;
//  ctm.setRotation(pi);
//  CTM<double> expected{{-1.0, 0.0, 0.0, -1.0}, {0.0, 0.0}};
//  EXPECT_EQ(ctm, expected);
//}

//TEST(CTM, setRotationAnglePiHalf)
//{
//  constexpr double pi = 3.14159265358979323846;
//  constexpr double pi2 = 1.57079632679489661923;
//  CTM<double> ctm;
//  ctm.setRotation(pi2);
//  CTM<double> expected{{0.0, -1.0, 1.0, 0.0}, {0.0, 0.0}};
//  EXPECT_EQ(ctm, expected);
//}

TEST(CTM, setTranslationVector)
{
  CTM<double> ctm;
  double const tx = 1.234;
  double const ty = 23.97;
  CTM<double>::vector newTrans{tx, ty};
  ctm.setTranslation(newTrans);
  CTM<double>::vector expectedTrans{tx, ty};
  EXPECT_EQ(ctm.translation(), expectedTrans);
}

TEST(CTM, applyTranslationWithoutRotation)
{
  CTM<double> ctm;
  double const tx = 1.234;
  double const ty = 23.97;
  CTM<double>::vector newTrans{tx, ty};
  ctm.setTranslation(newTrans);
  CTM<double>::vector x{0.0,0.0};

  const auto xTrans = x * ctm;
  EXPECT_EQ(xTrans, newTrans);
}

TEST(CTM, applyTranslationWithRotation)
{
  CTM<double> ctm{{0.0, -1.0, 1.0, 0.0}, {1.0, 1.0}};
  CTM<double>::vector x{2.52, 5.25};
  CTM<double>::vector expected{-4.25, 3.52};
  const auto xTrans = x * ctm;
  EXPECT_EQ(xTrans, expected);
}

TEST(CTM, CombineTwoCTMsOneOnlyRotationOneOnlyTranslation)
{
  CTM<double> ctm1{{0.0, -1.0, 1.0, 0.0}};
  CTM<double> ctm2{{1.0, 0.0, 0.0, 1.0}, {1.0, 1.0}};
  const auto ctm3 = ctm1 * ctm2;

  CTM<double>::vector x{2.52, 5.25};
  CTM<double>::vector expected{-4.25, 3.52};
  const auto xTrans = x * ctm3;
  EXPECT_EQ(xTrans, expected);
}

TEST(CTM, CombineTwoCTMs)
{
  CTM<double> ctm1{{1.0, -1.0, 1.0, 2.0}, {2.5, 1.25}};
  CTM<double> ctm2{{2.0, 1.0, 0.5, 4.0}, {1.2, 5.0}};
  const auto ctm3 = ctm1 * ctm2;
  CTM<double> ctmExpected{{3.0, 0.0, 4.5, 7.5}, {7.45, 11.25}};
  EXPECT_EQ(ctm3, ctmExpected);
}

TEST(CTM, CheckOrthogonalityOfDefaultCTM)
{
  CTM<double> ctm;
  EXPECT_TRUE(ctm.isOrthogonal());
}

TEST(CTM, CheckOrthogonality)
{
  CTM<double> ctm{{2.0, 3.0, -3.0, 2.0}, {2.5, 1.25}};
  EXPECT_TRUE(ctm.isOrthogonal());
}

TEST(CTM, Scale)
{
  CTM<double> ctm{{1.0, 0.0, 0.0, 1.0}, {2.5, 1.25}};
  ctm.scale(1.5, 2.5);
  CTM<double>::vector x{1.0, 1.0};
  const auto xTrans = x * ctm;
  CTM<double>::vector expected{1.5 + 2.5, 2.5 + 1.25};
  EXPECT_EQ(xTrans, expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTCTM_H_2278416328555301461995117149265571425728524 
