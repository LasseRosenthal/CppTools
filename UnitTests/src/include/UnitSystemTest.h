/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    UnitSystemTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.11.2020 
 */ 
 
#ifndef UNITSYSTEMTEST_H_503275584157208865815859517803214594 
#define UNITSYSTEMTEST_H_503275584157208865815859517803214594 
 
 
// includes
#include <Unit/UnitSystemGenerator.h>
#include <Unit/detail/Arithmetic.h>
 
using namespace unit::dimension;
using namespace unit;
 

TEST(UnitSystem, TimeUnitSystemDimension)
{
  using myTimeUnit = TimeUnitGenerator<std::milli>; 
  EXPECT_TRUE((std::is_same_v<myTimeUnit::dimension, unit::dimension::Time>));
}

TEST(UnitSystem, LengthUnitSystemDimension)
{
  using myLengthUnit = LengthUnitGenerator<std::milli>; 
  EXPECT_TRUE((std::is_same_v<myLengthUnit::dimension, unit::dimension::Length>));
}

TEST(UnitSystem, Division)
{
  using myTimeUnit         = TimeUnitGenerator<std::milli>;
  using myLengthUnit       = LengthUnitGenerator<std::milli>;
  using expectedUnitSystem = VelocityUnitGenerator<std::ratio<1>, std::ratio<1>>;
  using generatedSystem    = arithmetic::DivisionType<myLengthUnit, myTimeUnit>;
  EXPECT_TRUE((std::is_same_v<generatedSystem, expectedUnitSystem>));
}

TEST(UnitSystem, Multiplication)
{
  using D1 = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                                MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;
  using D2 = DimensionGenerator<TimeTag<std::ratio<5, 9>>, LengthTag<std::ratio<1>>,
                                MassTag<std::ratio<4, 5>>, SubstanceTag<std::ratio<3, 7>>>;
  using expectedType = DimensionGenerator<TimeTag<std::ratio<8, 63>>,
                                          LengthTag<std::ratio<3>>,
                                          MassTag<std::ratio<17, 15>>,
                                          StorageAmountTag<std::ratio<3, 7>>,
                                          SubstanceTag<std::ratio<3, 7>>>;

  using UnitSystem1        = arithmetic::UnitSystemForDimension<D1>;
  using UnitSystem2        = arithmetic::UnitSystemForDimension<D2>;
  using expectedUnitSystem = arithmetic::UnitSystemForDimension<expectedType>;
  using generatedSystem    = arithmetic::MultiplicationType<UnitSystem1, UnitSystem2>;
  EXPECT_TRUE((std::is_same_v<generatedSystem, expectedUnitSystem>));
}

TEST(UnitSystem, InversionType)
{
  using D1 = DimensionGenerator<TimeTag<std::ratio<-3, 7>>, LengthTag<std::ratio<2>>,
                                MassTag<std::ratio<1, 3>>, StorageAmountTag<std::ratio<3, 7>>>;
  using scaling = std::ratio<2, 3>;

  using D3 = DimensionGenerator<TimeTag<std::ratio<3, 7>>, LengthTag<std::ratio<-2>>,
                                MassTag<std::ratio<-1, 3>>, StorageAmountTag<std::ratio<-3, 7>>>;

  using UnitSystem1        = arithmetic::UnitSystemForDimension<D1, scaling>;
  using expectedUnitSystem = arithmetic::UnitSystemForDimension<D3, std::ratio<3, 2>>;
  using generatedSystem    = arithmetic::InversionType<UnitSystem1>;
  EXPECT_TRUE((std::is_same_v<generatedSystem, expectedUnitSystem>));
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // UNITSYSTEMTEST_H_503275584157208865815859517803214594 
