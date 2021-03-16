/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    linalg.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    13.02.2021 
 */ 
 
#ifndef LINALG_H_23973458112281915813388279723403114712958 
#define LINALG_H_23973458112281915813388279723403114712958 
 
 
// includes
#include <algorithm>
#include <cstddef>
#include <execution>
#include <future>
#include <vector>

#if __has_include(<immintrin.h>)
#  include <immintrin.h>
#  define HAS_SIMD
#endif


namespace linalg {


inline auto const hardwareThreads = static_cast<std::size_t>(std::thread::hardware_concurrency());

/// Enumeration to discriminate between execution policies.
enum class executionPolicy : char {
  seq, ///< tag for sequential execution
  par  ///< tag for parallel execution
};


/**
 * @brief  Multiplies the entries of a given array with a scalar.
 * @tparam T the type of the array.
 * @param  arr a pointer the first element of the array.
 * @param  size the size of the array.
 * @param  fac the factor the array is multiplied with.
 * @remark sequential implementation
 */
template <typename T>
inline void multArrValSeq(T* arr, std::size_t const size, T const& fac)
{
  for(const auto end = arr + size; arr != end; ++arr)
  {
    *arr *= fac;
  }
}


inline void multArrValSIMD(int* arr, std::size_t const size, int fac)
{
  const __m256i f = _mm256_set1_epi32(fac);
  constexpr std::size_t blockSize = 8ULL;
  const auto numBlocks = size / blockSize;
  for(std::size_t i = 0ULL; i < numBlocks; ++i, arr += blockSize)
  {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(arr), _mm256_mullo_epi32(_mm256_lddqu_si256(reinterpret_cast<__m256i const*>(arr)), f));
  }

  for(std::size_t i = 0ULL; i < size % blockSize; ++i, ++arr)
  {
    *arr *= fac;
  }
}


/**
 * @brief  Performs elementwise multiplication of a given array with a given scalar.
 * @tparam T the type of the array.
 * @param  arr a pointer the first element of the array.
 * @param  size the size of the array.
 * @param  fac the factor the array is multiplied with.
 * @remark parallel implementation
 */
template <typename T>
inline void multArrValPar(T* arr, std::size_t const size, T const& fac)
{
  static constexpr std::size_t minBlockSize{1024ULL};
  if(const auto numThreads = hardwareThreads < size / minBlockSize ? hardwareThreads : size / minBlockSize; numThreads > 1ULL)
  {
    const auto blockSize = size / numThreads;
    auto numExtraThreads = numThreads - 1ULL;
    std::vector<std::future<void>> futures;
    futures.reserve(numExtraThreads);
    for(;numExtraThreads > 0ULL; --numExtraThreads, arr += blockSize)
    {
      futures.emplace_back(std::async(std::launch::async, [=]() { multArrValSeq(arr, blockSize, fac); }));
    }

    multArrValSeq(arr, blockSize + size % numThreads, fac);
    for(auto const& f : futures)
    {
      f.wait();
    }
  }
  else
  {
    for(const auto end = arr + size; arr != end; ++arr)
    {
      *arr *= fac;
    }
  }
}

template <typename T>
inline void multArrValPar2(T* arr, std::size_t const size, T const& fac)
{
  std::for_each(std::execution::par, arr, arr + size, [&fac](T& v) { v *= fac; });
}

#ifdef HAS_SIMD

inline void multArrValSIMD2(int* arr, std::size_t const size, int fac)
{
  const __m128i f = _mm_set1_epi32(fac);
  constexpr std::size_t blockSize = 4ULL;
  const auto numBlocks = size / blockSize;
  for(std::size_t i = 0ULL; i < numBlocks; ++i, arr += blockSize)
  {
    _mm_store_si128 (reinterpret_cast<__m128i*>(arr), _mm_mullo_epi32(_mm_loadu_si128(reinterpret_cast<__m128i*>(arr)), f));
  }

  for(std::size_t i = 0ULL; i < size % blockSize; ++i, ++arr)
  {
    *arr *= fac;
  }
}



inline void multArrValSIMD(double* arr, std::size_t const size, double fac)
{
  const __m256d f = _mm256_set1_pd(fac);
  const auto numBlocks = size / 4;
  for(std::size_t i = 0ULL; i < numBlocks; ++i)
  {
    const __m256d a = _mm256_load_pd(arr);
    const auto v  = _mm256_mul_pd(a, f);
    _mm256_store_pd(arr, v);
    arr += 4;
  }

  for(std::size_t i = 0ULL; i < size % 4; ++i)
  {
    *arr *= fac;
  }
}


#endif


}   // namespace linalg
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // LINALG_H_23973458112281915813388279723403114712958 
