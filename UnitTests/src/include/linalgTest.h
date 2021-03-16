/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    linalgTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    15.02.2021 
 */ 
 
#ifndef LINALGTEST_H_111315465278521602667182619911888134797023 
#define LINALGTEST_H_111315465278521602667182619911888134797023 
 
 
// includes
#include <Numerics/linalg.h>

#include <random>


template <auto Min, auto Max>
class UniformIntDistribution {

public:
  using type = decltype(Min);

  UniformIntDistribution()
    : dis(Min, Max)
  {
    gen.seed(1);
  }

  auto operator()()
  {
    return dis(gen);
  }

private:
  std::mt19937                        gen;
  std::uniform_int_distribution<type> dis;
};

TEST(Linalg, multArrValSeq)
{
  constexpr int fac = 5;
  constexpr std::size_t size = 5398;
  std::vector<int> v;
  std::vector<int> expected;
  v.reserve(size);
  expected.reserve(size);

  UniformIntDistribution<-100, 100>  dis;

  for(int i = 0ULL; i < size; ++i)
  {
    const auto value = dis();
    v.push_back(value);
    expected.push_back(fac * value);
  }

  linalg::multArrValSeq(v.data(), size, fac);
  EXPECT_EQ(v, expected);
}

TEST(Linalg, multArrValPar)
{
  constexpr int fac = 5;
  constexpr std::size_t size = 20*32;
  std::vector<int> v;
  std::vector<int> expected;
  v.reserve(size);
  expected.reserve(size);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < size; ++i)
  {
    const auto value = dis();
    v.push_back(value);
    expected.push_back(fac * value);
  }

  linalg::multArrValPar(v.data(), size, fac);
  EXPECT_EQ(v, expected);
}

TEST(Linalg, multArrValSIMD)
{
  constexpr int fac = 5;
  constexpr std::size_t size = 20*32;
  std::vector<int> v;
  std::vector<int> expected;
  v.reserve(size);
  expected.reserve(size);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < size; ++i)
  {
    const auto value = dis();
    v.push_back(value);
    expected.push_back(fac * value);
  }

  linalg::multArrValSIMD(v.data(), size, fac);
  EXPECT_EQ(v, expected);
}


 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // LINALGTEST_H_111315465278521602667182619911888134797023 
