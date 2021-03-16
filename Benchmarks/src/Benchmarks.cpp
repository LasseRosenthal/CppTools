/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    benchmark.cpp. 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    28.10.2020 
 */ 
 
// includes
#include <MultiIndexVector/VectorMatrixAlgebra.h>
#include <benchmark/benchmark.h>
#include <Utils/StringUtils.h>
#include <Utils/miscellaneous.h>
#include <Utils/StringAlgorithms.h>

#include <random>
#include <iostream>
#include <memory>

#include <immintrin.h>

#include "linalgBenchmarks.h"


using namespace std::string_literals;
using namespace multiindex;

constexpr auto ordering = storageOrdering::rowMajor;



 

class Person {
public:
  //Person(std::string const& n)
  //  : name{n}
  //{}

  Person(std::string n)
    : name{std::move(n)}
  {}

private:

  std::string name;
};

#if 0
template <typename FloatT>
void fillWithRandomNumbers(FloatT* const a1, FloatT* const a2, std::size_t size)
{
  std::mt19937 gen;
  gen.seed(1);
  constexpr FloatT                       minFloat = -100.0;
  constexpr FloatT                       maxFloat = 100.0;
  std::uniform_real_distribution<FloatT> dis(minFloat, maxFloat);

  for(std::size_t i = 0ULL; i < size; ++i)
  {
    a1[i] = dis(gen);
    a2[i] = dis(gen);
  }
}

static constexpr std::size_t arraysSize = 8ULL;

static void BM_dotProductSimple(benchmark::State& state)
{
  using type = double;
  type a1[arraysSize];
  type a2[arraysSize];

  fillWithRandomNumbers(a1, a2, arraysSize);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(multiindex::simpleDotProduct(a1, a2, arraysSize));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_dotProductSimple);

inline double horizontalAdd(__m256d v4)
{
  //  d1 | d2 | d3 | d4  -->  d1+d2 | d3+d4 = v2
  const __m128d v2 = _mm_add_pd(_mm256_castpd256_pd128(v4), _mm256_extractf128_pd(v4, 1));
  // _mm_shuffle_pd(v2, v2, 1) : d1+d2 | d3+d4 -> d3+d4 | d1+d2
  // v1 = d1+d2 | d3+d4 + d3+d4 | d1+d2 = d1+d2+d3+d4 | d3+d4+d1+d2
  const __m128d v1 = _mm_add_pd(v2, _mm_shuffle_pd(v2, v2, 1));
  return _mm_cvtsd_f64(v1);
}

double dotProdSIMD(double const* p1, double const* p2, std::size_t size)
{
  const auto end = p1 + size;

  __m256d resultVector = _mm256_set1_pd(0.0);
  while(p1 < end)
  {
    const __m256d leftLane = _mm256_loadu_pd(p1);
    const __m256d rightLane = _mm256_loadu_pd(p2);
    resultVector = _mm256_fmadd_pd(leftLane, rightLane, resultVector);

    p1 += 4;
    p2 += 4;
  }

  return horizontalAdd(resultVector);
}

static void BM_dotProdSIMD(benchmark::State& state)
{
  using type = double;
  type a1[arraysSize];
  type a2[arraysSize];

  fillWithRandomNumbers(a1, a2, arraysSize);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(dotProdSIMD(a1, a2, arraysSize));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_dotProdSIMD);
#endif


#if 0
static void BM_stringSplit1(benchmark::State& state)
{
  auto s = "split this string with lots of text into substrings"s;

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(stringUtils::split(s));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_stringSplit1);


static void BM_stringSplit2(benchmark::State& state)
{
  auto s = "split this string with lots of text into substrings"s;

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(stringUtils::split2(s));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_stringSplit2);


static void BM_stringSplit3(benchmark::State& state)
{
  auto s = "split this string with lots of text into substrings"s;

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(stringUtils::split3(s, ' '));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_stringSplit3);

static void BM_stringSplit4(benchmark::State& state)
{
  auto s = "split this string with lots of text into substrings"s;

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(stringUtils::split4(s, ' '));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_stringSplit4);

static void BM_stringSplit5(benchmark::State& state)
{
  auto s = "split this string with lots of text into substrings"s;

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(stringUtils::split5(s, ' '));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_stringSplit5);

#if 0
static void BM_MatrixMult(benchmark::State& state) {
  SquareStackMatrix<double, ordering, 2ULL> mat1{12.0, 4.0, 3.0, 9.0};
  SquareStackMatrix<double, ordering, 2ULL> mat2{134.13, 253.135, 12.0009, 1.0};
  for(auto _ : state)
  {
    benchmark::DoNotOptimize(mat1 * mat2);
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_MatrixMult);


template <std::size_t Alignment>
constexpr std::size_t align_up(std::size_t n) noexcept
{
  return (n + (Alignment - 1)) & ~(Alignment - 1);
}

double horizontalAdd(__m256d v4)
{
  //  d1 | d2 | d3 | d4  -->  d1+d2 | d3+d4 = v2
  const __m128d v2 = _mm_add_pd(_mm256_castpd256_pd128(v4), _mm256_extractf128_pd(v4, 1));
  // _mm_shuffle_pd(v2, v2, 1) : d1+d2 | d3+d4 -> d3+d4 | d1+d2
  // v1 = d1+d2 | d3+d4 + d3+d4 | d1+d2 = d1+d2+d3+d4 | d3+d4+d1+d2
  const __m128d v1 = _mm_add_pd(v2, _mm_shuffle_pd(v2, v2, 1));
  return _mm_cvtsd_f64(v1);
}
double dotProd(double const* p1, double const* p2, std::size_t size)
{
  const auto end = p1 + size;

  __m256d resultVector = _mm256_set1_pd(0.0);
  while(p1 < end)
  {
    const __m256d leftLane = _mm256_loadu_pd(p1);
    const __m256d rightLane = _mm256_loadu_pd(p2);
    resultVector = _mm256_fmadd_pd(leftLane, rightLane, resultVector);

    p1 += 4;
    p2 += 4;
  }

  return horizontalAdd(resultVector);
}

float sse_dpps(const float* a, const float* b, size_t n) {

  const size_t chunks = n / 4;

  float result = 0;
  for (size_t i = 0; i < n; i += 4)
  {
    __m128 A = _mm_loadu_ps(&a[i]);
    __m128 B = _mm_loadu_ps(&b[i]);
    __m128 AB = _mm_dp_ps(A, B, 0xf1);

    result += _mm_cvtss_f32(AB);
  }

  for (size_t i = chunks * 4; i < n; i++)
  {
    result += a[i] * b[i];
  }

  return result;
}

template <typename T>
T dotProdSimple(T const* p1, T const* p2, std::size_t const size)
{
  const auto end = p1 + size;
  T result{};
  while(p1 != end)
  {
    result += (*p1++) * (*p2++);
  }
  return result;
}

static void BM_dotProdSimple(benchmark::State& state)
{
  using type = double;
  const std::size_t size = 10000;

  auto a1 = std::make_unique<type[]>(size);
  auto a2 = std::make_unique<type[]>(size);
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<type> dis(0.0, 1000.0);
  for(std::size_t i = 0ULL; i < size; ++i)
  {
    a1[i] = dis(gen);
    a2[i] = dis(gen);
  }

  auto* p1 = a1.get();
  auto* p2 = a2.get();
  for(auto _ : state)
  {
    benchmark::DoNotOptimize(dotProdSimple(p1, p2, size));
  }

  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_dotProdSimple);
#endif

static void BM_getMapKeys(benchmark::State& state)
{
  const int mapSize = 31;
  std::map<int, int> m;
  const int first = 152;
  for(int i = first; i < first + mapSize; ++i)
  {
    m[i] = 156;
  }

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(cpptools::getMapKeys(m));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_getMapKeys);


static void BM_wstringToString(benchmark::State& state)
{
  const auto str = L"hallo dies ist ein langer string mit viel schönem Text"s;

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(stringAlgorithms::wstringToString(str));
  }
  state.SetItemsProcessed(100000);
}

BENCHMARK(BM_wstringToString);
#endif



BENCHMARK_MAIN();
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
