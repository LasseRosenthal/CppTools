/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    linalgBenchmarks.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    15.02.2021 
 */ 
 
#ifndef LINALGBENCHMARKS_H_637776252182487762043631864156551330416091 
#define LINALGBENCHMARKS_H_637776252182487762043631864156551330416091 
 
 
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

inline constexpr std::size_t arraySize = 10240 * 2048 + 0;

static void BM_multArrValSeq(benchmark::State& state)
{
  constexpr int fac = 5;
  std::vector<int> v;
  v.reserve(arraySize);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < arraySize; ++i)
  {
    const auto value = dis();
    v.push_back(value);
  }

  for(auto _ : state)
  {
    linalg::multArrValSeq(v.data(), arraySize, fac);
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_multArrValSeq);

static void BM_multArrValPar(benchmark::State& state)
{
  constexpr int fac = 5;
  std::vector<int> v;
  v.reserve(arraySize);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < arraySize; ++i)
  {
    const auto value = dis();
    v.push_back(value);
  }

  for(auto _ : state)
  {
    linalg::multArrValPar(v.data(), arraySize, fac);
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_multArrValPar);


static void BM_multArrValPar2(benchmark::State& state)
{
  constexpr int fac = 5;
  std::vector<int> v;
  v.reserve(arraySize);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < arraySize; ++i)
  {
    const auto value = dis();
    v.push_back(value);
  }

  for(auto _ : state)
  {
    linalg::multArrValPar2(v.data(), arraySize, fac);
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_multArrValPar2);


static void BM_multArrValSIMD(benchmark::State& state)
{
  constexpr int fac = 5;
  std::vector<int> v;
  v.reserve(arraySize);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < arraySize; ++i)
  {
    const auto value = dis();
    v.push_back(value);
  }

  for(auto _ : state)
  {
    linalg::multArrValSIMD(v.data(), arraySize, fac);
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_multArrValSIMD);

static void BM_multArrValSIMD2(benchmark::State& state)
{
  constexpr int fac = 5;
  std::vector<int> v;
  v.reserve(arraySize);

  UniformIntDistribution<-100, 100>  dis;
  for(int i = 0ULL; i < arraySize; ++i)
  {
    const auto value = dis();
    v.push_back(value);
  }

  for(auto _ : state)
  {
    linalg::multArrValSIMD2(v.data(), arraySize, fac);
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_multArrValSIMD2);
 

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // LINALGBENCHMARKS_H_637776252182487762043631864156551330416091 
