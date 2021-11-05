/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    DimensionTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    17.11.2020 
 */ 
 
#ifndef DIMENSIONTEST_H_3099031734247091080527003392822182307829386 
#define DIMENSIONTEST_H_3099031734247091080527003392822182307829386 
 
 
// includes
#include <Unit/Dimension.h>
 
using namespace unit::dimension; 

TEST(Dimension, ExponentsPredefinedDimensionTime)
{
  using expectedExponents = meta::Typelist<
    std::ratio<1>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>
  >;

  using myDim = unit::dimension::Time;
  EXPECT_TRUE((std::is_same_v<myDim::exponents, expectedExponents>));
}

TEST(Dimension, isRationalPredefinedDimensionTime)
{
  using myDim = unit::dimension::Time;
  EXPECT_TRUE(myDim::isRational);
}

TEST(Dimension, ExponentsPredefinedDimensionPower)
{
  using expectedExponents = meta::Typelist<
    std::ratio<-3>,
    std::ratio<2>,
    std::ratio<1>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>
  >;

  using myDim = unit::dimension::Power;
  EXPECT_TRUE((std::is_same_v<myDim::exponents, expectedExponents>));
}

TEST(Dimension, ExponentsNonRationalDimension)
{
  using expectedExponents = meta::Typelist<
    std::ratio<-3, 7>,
    std::ratio<2>,
    std::ratio<1, 3>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<0>,
    std::ratio<3,7>,
    std::ratio<0>,
    std::ratio<0>
  >;

  using myDim = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>, MassTag<std::ratio<1, 3>>,
                                   StorageAmountTag<std::ratio<3,7>>>;
  EXPECT_TRUE((std::is_same_v<myDim::exponents, expectedExponents>));
}

TEST(Dimension, isRationalNonRationalDimension)
{
  using myDim = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                                   MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;
  EXPECT_FALSE(myDim::isRational);
}

TEST(Dimension, DivisionType)
{
  using D1 = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                                MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;
  using D2 = DimensionGenerator<SubstanceTag<std::ratio<3, 7>>, TimeTag<std::ratio<5, 9>>, LengthTag<std::ratio<1>>,
                                MassTag<std::ratio<4, 5>>>;

  using generatedType = unit::dimension::DivisionType<D1, D2>;
  using expectedType = DimensionGenerator<TimeTag<std::ratio<-62, 63>>,
                                          LengthTag<std::ratio<1>>,
                                          MassTag<std::ratio<-7, 15>>,
                                          StorageAmountTag<std::ratio<3, 7>>,
                                          SubstanceTag<std::ratio<-3, 7>>>;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}

TEST(Dimension, MultiplicationType)
{
  using D1 = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                                MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;
  using D2 = DimensionGenerator<TimeTag<std::ratio<5, 9>>, LengthTag<std::ratio<1>>,
                                MassTag<std::ratio<4, 5>>, SubstanceTag<std::ratio<3, 7>>>;

  using generatedType = unit::dimension::MultiplicationType<D1, D2>;
  using expectedType = DimensionGenerator<TimeTag<std::ratio<8, 63>>,
                                          LengthTag<std::ratio<3>>,
                                          MassTag<std::ratio<17, 15>>,
                                          StorageAmountTag<std::ratio<3, 7>>,
                                          SubstanceTag<std::ratio<3, 7>>>;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}

TEST(Dimension, InversionTypeOneTag)
{
  using D             = DimensionGenerator<TimeTag<std::ratio<1>>>;
  using generatedType = unit::dimension::InversionType<D>;
  using expectedType  = DimensionGenerator<TimeTag<std::ratio<-1>>>;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}

TEST(Dimension, InversionType)
{
  using D = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                               MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;

  using generatedType = unit::dimension::InversionType<D>;
  using expectedType =
    DimensionGenerator<TimeTag<std::ratio<3, 7>>, LengthTag<std::ratio<-2>>,
                       MassTag<std::ratio<-1, 3>>, StorageAmountTag<std::ratio<-3, 7>>>;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}

TEST(Dimension, NthRootTypeCOmplexType)
{
  constexpr std::size_t N = 2ULL;
  using D = DimensionGenerator<TimeTag<std::ratio<3, 7>>, LengthTag<std::ratio<2>>,
                               MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;

  using generatedType = unit::dimension::NthRoot<D, N>;
  using expectedType =
    DimensionGenerator<TimeTag<std::ratio<3, N*7>>, LengthTag<std::ratio<1>>,
                       MassTag<std::ratio<1, N*3>>, StorageAmountTag<std::ratio<3, N*7>>>;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}

TEST(Dimension, NthRootTypeOfSquaredTime)
{
  constexpr std::size_t N = 2ULL;
  using D = DimensionGenerator<TimeTag<std::ratio<2>>>;

  using generatedType = unit::dimension::NthRoot<D, N>;
  using expectedType = unit::dimension::Time;
  EXPECT_TRUE((std::is_same_v<generatedType, expectedType>));
}


TEST(Dimension, HasOnlyZeroExponentsExpectTrue)
{
  using D = DimensionGenerator<TimeTag<std::ratio<0, 3>>, LengthTag<std::ratio<0, 2>>>;
  EXPECT_TRUE(HasOnlyZeroExponents<D>);
}

TEST(Dimension, HasOnlyZeroExponentsExpectFalse)
{
  using D = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                               MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;
  EXPECT_FALSE(HasOnlyZeroExponents<D>);
}



 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DIMENSIONTEST_H_3099031734247091080527003392822182307829386 
