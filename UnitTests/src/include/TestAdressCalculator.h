/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestAdressCalculator.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.08.2020 
 */ 
 
#ifndef TESTADRESSCALCULATOR_H_5313108902736330569125652010677232482918010 
#define TESTADRESSCALCULATOR_H_5313108902736330569125652010677232482918010 
 
 
// includes
#include <MultiIndexVector/AdressCalculator.h>

using namespace multiindex;

 
TEST(MultiIndexVector, AdressCalculatorColumnMajor2D)
{
  constexpr std::size_t D = 2;
  std::size_t Dims[D] = {2138,3211};
  using Indexer = AdressCalculator<D, storageOrdering::columnMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  for(int i = 0; i < Dims[0]; ++i)
  {
    for(int j = 0; j < Dims[1]; ++j)
    {
      calculated = Indexer::transform(Dims, i, j);
      expected = i + Dims[0] * j;

      if(calculated != expected) break;
    }
  }
  
  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}

TEST(MultiIndexVector, AdressCalculatorColumnMajor5D)
{
  constexpr std::size_t D = 5;
  std::size_t Dims[D] = {21, 32, 11, 32, 12};
  using Indexer = AdressCalculator<D, storageOrdering::columnMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  for(int i = 0; i < Dims[0]; ++i)
  {
    for(int j = 0; j < Dims[1]; ++j)
    {
      for(int k = 0; k < Dims[2]; ++k)
      {
        for(int l = 0; l < Dims[3]; ++l)
        {
          for(int m = 0; m < Dims[4]; ++m)
          {
            calculated = Indexer::transform(Dims, i, j, k, l, m);
            expected = i + Dims[0]*j + Dims[0]*Dims[1]*k + Dims[0]*Dims[1]*Dims[2]*l + Dims[0]*Dims[1]*Dims[2]*Dims[3]*m;

            if(calculated != expected) break;
          }
        }
      }
    }
  }

  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}

TEST(MultiIndexVector, AdressCalculatorRowMajor2D)
{
  constexpr std::size_t D = 2;
  std::size_t Dims[D] = {2138,3211};
  using Indexer = AdressCalculator<D, storageOrdering::rowMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  for(int i = 0; i < Dims[0]; ++i)
  {
    for(int j = 0; j < Dims[1]; ++j)
    {
      calculated = Indexer::transform(Dims, i, j);
      expected = Dims[1] * i + j;

      if(calculated != expected) break;
    }
  }

  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}

TEST(MultiIndexVector, AdressCalculatorRowMajor5D)
{
  constexpr std::size_t D = 5;
  std::size_t Dims[D] = {21, 32, 11, 32, 12};
  using Indexer = AdressCalculator<D, storageOrdering::rowMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  for(int i = 0; i < Dims[0]; ++i)
  {
    for(int j = 0; j < Dims[1]; ++j)
    {
      for(int k = 0; k < Dims[2]; ++k)
      {
        for(int l = 0; l < Dims[3]; ++l)
        {
          for(int m = 0; m < Dims[4]; ++m)
          {
            calculated = Indexer::transform(Dims, i, j, k, l, m);
            expected = i*Dims[1]*Dims[2]*Dims[3]*Dims[4] + j*Dims[2]*Dims[3]*Dims[4] + Dims[3]*Dims[4]*k + Dims[4]*l + m;

            if(calculated != expected) break;
          }
        }
      }
    }
  }

  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}
 
TEST(MultiIndexVector, AdressCalculatorSubDivision2DColumnMajor)
{
  constexpr std::size_t D = 2;
  constexpr std::size_t d1 = 2138;
  constexpr std::size_t d2 = 3211;

  std::size_t Dims[D] = {d1, d2};
  using Indexer = AdressCalculator<D, storageOrdering::columnMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  constexpr auto fac = d1;
  for(int i = 0; i < Dims[D-1]; ++i)
  {
    calculated = Indexer::subDivision(Dims, i);
    expected = i * fac;
    if(calculated != expected) break;
  }
  
  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}

TEST(MultiIndexVector, AdressCalculatorSubDivision5DColumnMajor)
{
  constexpr std::size_t D = 5;
  constexpr std::size_t d1 = 21;
  constexpr std::size_t d2 = 31;
  constexpr std::size_t d3 = 12;
  constexpr std::size_t d4 = 22;
  constexpr std::size_t d5 = 29;

  std::size_t Dims[D] = {d1, d2, d3, d4, d5};
  using Indexer = AdressCalculator<D, storageOrdering::columnMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  constexpr auto fac = d1*d2*d3*d4;
  for(int i = 0; i < Dims[D-1]; ++i)
  {
    calculated = Indexer::subDivision(Dims, i);
    expected = i * fac;
    if(calculated != expected) break;
  }
  
  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}

TEST(MultiIndexVector, AdressCalculatorSubDivision2DRowMajor)
{
  constexpr std::size_t D = 2;
  constexpr std::size_t d1 = 2138;
  constexpr std::size_t d2 = 3211;

  constexpr std::size_t Dims[D] = {d1, d2};
  using Indexer = AdressCalculator<D, storageOrdering::rowMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  constexpr auto fac = d2;
  for(int i = 0; i < Dims[0]; ++i)
  {
    calculated = Indexer::subDivision(Dims, i);
    expected = i * fac;
    if(calculated != expected) break;
  }
  
  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}

TEST(MultiIndexVector, AdressCalculatorSubDivision5DRowMajor)
{
  constexpr std::size_t D = 5;
  constexpr std::size_t d1 = 21;
  constexpr std::size_t d2 = 31;
  constexpr std::size_t d3 = 12;
  constexpr std::size_t d4 = 22;
  constexpr std::size_t d5 = 29;

  std::size_t Dims[D] = {d1, d2, d3, d4, d5};
  using Indexer = AdressCalculator<D, storageOrdering::rowMajor>;

  std::size_t calculated{};
  std::size_t expected{};
  constexpr auto fac = d2*d3*d4*d5;
  for(int i = 0; i < Dims[0]; ++i)
  {
    calculated = Indexer::subDivision(Dims, i);
    expected = i * fac;
    if(calculated != expected) break;
  }
  
  EXPECT_EQ(calculated, expected) << "expected : " << expected << " calculated : " << calculated;
}


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTADRESSCALCULATOR_H_5313108902736330569125652010677232482918010 
