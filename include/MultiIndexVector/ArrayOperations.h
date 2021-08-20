/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 

/**
 * @file    ArrayOperations.h
 * @brief
 *
 * @author  Lasse Rosenthal
 * @date    22.09.2020
 */

#ifndef ARRAYOPERATIONS_H_16299848620354257801491122634181692861826826
#define ARRAYOPERATIONS_H_16299848620354257801491122634181692861826826


// includes
#include <MultiIndexVector/StorageOrdering.h>

#include <Utils/miscellaneous.h>

#include <algorithm>
#include <cstddef>
#include <execution>
#include <future>
#include <memory>
#include <type_traits>
#include <vector>


namespace multiindex {


/**
 * @brief  Multiplies the entries of a given array with a factor.
 *         naive implementation.
 * @tparam T the type of the array.
 * @param  x a pointer the array.
 * @param  size the size of the arrays.
 * @param  factor the factor the array is multiplied with.
 */
template <typename T>
inline void multiply(T* x, std::size_t const size, T const& factor)
{
  for(const auto end = x + size; x != end; ++x)
  {
    *x *= factor;
  }
}

/**
 * @brief  Multiplies the entries of a given array with a factor of different type.
 *         The factor has to be convertibe to the type of the array. naive implementation.
 * @tparam T the type of the array.
 * @tparam S the type of the factor
 * @param  x a pointer the array.
 * @param  size the size of the arrays.
 * @param  factor the factor the array is multiplied with.
 */
template <typename T, typename S, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
inline void multiply(T* x, std::size_t const size, S const& factor)
{
  for(const auto end = x + size; x != end; ++x)
  {
    *x *= factor;
  }
}

/**
 * @brief  Divides the entries of a given array by a given divisor.
 *         naive implementation.
 * @tparam T the type of the array.
 * @param  x a pointer the array.
 * @param  size the size of the array.
 * @param  divisor the divisor the array is divided by.
 */
template <typename T>
inline void divide(T* x, std::size_t const size, T const& divisor)
{
  for(const auto end = x + size; x != end; ++x)
  {
    *x /= divisor;
  }
}

/**
 * @brief  Divides the entries of a given array by a divisor of different type.
 *         The factor has to be convertibe to the type of the array. naive implementation.
 * @tparam T the type of the array.
 * @tparam S the type of the divisor
 * @param  x a pointer the array.
 * @param  size the size of the array.
 * @param  divisor the divisor the array is divided by.
 */
template <typename T, typename S, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
inline void divide(T* x, std::size_t const size, S const& divisor)
{
  for(const auto end = x + size; x != end; ++x)
  {
    *x /= divisor;
  }
}

/**
 * @brief Applies compound multiplication with a given factor to each element in the given array.
 *        naive implementation.
 */
template <typename T, std::size_t N>
void multiply(T (&values)[N], T const& factor)
{
  std::transform(/*std::execution::par, */ std::begin(values), std::end(values), std::begin(values),
                 [&factor](T const& v) { return v * factor; });
}

/**
 * @brief Applies compound multiplication with a given factor of different type to each element in the given array.
 *        naive implementation.
 */
template <typename T, typename S, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
void multiply(T (&values)[N], S const& factor)
{
  std::transform(/*std::execution::par, */ std::begin(values), std::end(values), std::begin(values),
                 [&factor](T const& v) { return v * factor; });
}

/**
 * @brief Applies compound division by a given factor to each element in the given array.
 *        naive implementation.
 */
template <typename T, std::size_t N>
void divide(T (&values)[N], T const& divisor)
{
  std::transform(/*std::execution::par, */ std::begin(values), std::end(values), std::begin(values),
                 [&divisor](T const& v) { return v / divisor; });
}

/**
 * @brief Applies compound division by a given factor of different type to each element in the given array.
 *        naive implementation.
 */
template <typename T, typename S, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
void divide(T (&values)[N], S const& divisor)
{
  std::transform(/*std::execution::par, */ std::begin(values), std::end(values), std::begin(values),
                 [&divisor](T const& v) { return v / divisor; });
}

/**
 * @brief  Multiplies the entries of a given array with a factor and stores the result
 *         in destination array. Naive implementation.
 * @tparam T the type of the arrays.
 * @param  dest a pointer to the destination array.
 * @param  src a pointer to the source array.
 * @param  size the size of the arrays.
 * @param  factor the factor the array is multiplied with.
 */
template <typename T>
inline void multipleOf(T* dest, T const* src, std::size_t const size, T const& factor)
{
  for(const auto end = src + size; src != end; ++src, ++dest)
  {
    *dest = factor * (*src);
  }
}

/**
 * @brief  adds the entries of array y to the values of array x.
 *         naive implementation.
 * @tparam T the type of the arrays.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <typename T>
inline void addToArray(T* x, T const* y, std::size_t const size)
{
  for(const auto end = x + size; x != end; ++x, ++y)
  {
    *x += *y;
  }
}

/**
 * @brief  adds the entries of array y to the values of array x.
 *         naive implementation.
 * @tparam T the type of the left array.
 * @tparam S the type of the right array.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <typename T, typename S, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
inline void addToArray(T* x, S const* y, std::size_t const size)
{
  for(const auto end = x + size; x != end; ++x, ++y)
  {
    *x += *y;
  }
}

/**
 * @brief  subtracts the entries of array y from the values of array x.
 *         naive implementation.
 * @tparam T the type of the arrays.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <typename T>
inline void subtractFromArray(T* x, T const* y, std::size_t const size)
{
  for(const auto end = x + size; x != end; ++x, ++y)
  {
    *x -= *y;
  }
}

/**
 * @brief  subtracts the entries of array y from the values of array x.
 *         naive implementation.
 * @tparam T the type of the left array.
 * @tparam S the type of the right array.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <typename T, typename S, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
inline void subtractFromArray(T* x, S const* y, std::size_t const size)
{
  for(const auto end = x + size; x != end; ++x, ++y)
  {
    *x -= *y;
  }
}

/**
 * @brief  adds the entries of array called rightValues to the values of an array leftValues.
 *         naive implementation.
 * @tparam T the type of the arrays.
 * @tparam N the size of the arrays.
 * @param  leftValues a reference the left array.
 * @param  rightValues a reference the right array.
 */
template <typename T, std::size_t N>
void addToArray(T (&leftValues)[N], T const (&rightValues)[N])
{
  //std::transform(/*std::execution::par, */std::begin(leftValues), std::end(leftValues), std::begin(rightValues), std::begin(leftValues),
  //               [](T const& x, T const& y){ return x + y;});

  auto x = leftValues;
  auto y = rightValues;
  for(const auto end = x + N; x != end; ++x, ++y)
  {
    *x += *y;
  }
}

/**
 * @brief  adds the entries of array called rightValues to the values of an array leftValues.
 *         naive implementation.
 * @tparam T the type of the left array.
 * @tparam S the type of the right array.
 * @tparam N the size of the arrays.
 * @param  leftValues a reference the left array.
 * @param  rightValues a reference the right array.
 */
template <typename T, typename S, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
void addToArray(T (&leftValues)[N], S const (&rightValues)[N])
{
  //std::transform(/*std::execution::par, */std::begin(leftValues), std::end(leftValues), std::begin(rightValues), std::begin(leftValues),
  //               [](T const& x, T const& y){ return x + y;});

  auto x = leftValues;
  auto y = rightValues;
  for(const auto end = x + N; x != end; ++x, ++y)
  {
    *x += *y;
  }
}

/**
 * @brief  subtracts the entries of array called rightValues from the values of an array leftValues.
 *         naive implementation.
 * @tparam T the type of the arrays.
 * @tparam N the size of the arrays.
 * @param  leftValues a reference the left array.
 * @param  rightValues a reference the right array.
 */
template <typename T, std::size_t N>
void subtractFromArray(T (&leftValues)[N], T const (&rightValues)[N])
{
  //std::transform(/*std::execution::par, */std::begin(leftValues), std::end(leftValues), std::begin(rightValues), std::begin(leftValues),
  //               [](T const& x, T const& y){ return x - y;});

  auto x = leftValues;
  auto y = rightValues;
  for(const auto end = x + N; x != end; ++x, ++y)
  {
    *x -= *y;
  }
}

/**
 * @brief  subtracts the entries of array called rightValues from the values of an array leftValues.
 *         naive implementation.
 * @tparam T the type of the left array.
 * @tparam S the type of the right array.
 * @tparam N the size of the arrays.
 * @param  leftValues a reference the left array.
 * @param  rightValues a reference the right array.
 */
template <typename T, typename S, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
void subtractFromArray(T (&leftValues)[N], S const (&rightValues)[N])
{
  //std::transform(/*std::execution::par, */std::begin(leftValues), std::end(leftValues), std::begin(rightValues), std::begin(leftValues),
  //               [](T const& x, T const& y){ return x - y;});

  auto x = leftValues;
  auto y = rightValues;
  for(const auto end = x + N; x != end; ++x, ++y)
  {
    *x -= *y;
  }
}

/**
 * @brief  Multiplies the entries of a given array src with a factor and adds the result
 *         to a destination array. Naive implementation.
 * @tparam T the type of the arrays.
 * @param  dest a pointer to the destination array.
 * @param  src a pointer to the source array.
 * @param  size the size of the arrays.
 * @param  factor the factor the array is multiplied with.
 */
template <typename T>
inline void addMultipleOf(T* dest, T const* src, std::size_t const size, T const& factor)
{
  for(const auto end = src + size; src != end; ++src, ++dest)
  {
    *dest += factor * (*src);
  }
}

/**
 * @brief  a naive implementation of the dot product of two arrays of equal type.
 * @tparam T the type of the arrays.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 * @return the value of the dot product.
 */
template <typename T>
auto simpleDotProduct(T const* x, T const* y, std::size_t const size) -> T
{
  T result{};
  for(const auto end = x + size; x != end;)
  {
    result += (*x++) * (*y++);
  }
  return result;
}

/**
 * @brief  a naive implementation of the dot product of two arrays of equal type.
 * @tparam T the type of the arrays.
 * @param  dest a pointer to the destination where the result is stored.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <typename T>
void simpleDotProduct(T* dest, T const* x, T const* y, std::size_t const size)
{
  T result{};
  for(const auto end = x + size; x != end;)
  {
    result += (*x++) * (*y++);
  }
  *dest = result;
}

/**
 * @brief  a naive implementation of the dot product of two arrays of different type.
 * @tparam T the type of the left array.
 * @tparam S the type of the right array.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <typename T, typename S>
auto simpleDotProduct(T const* x, S const* y, std::size_t const size) -> std::common_type_t<T, S>
{
  std::common_type_t<T, S> result{};
  for(const auto end = x + size; x != end;)
  {
    result += (*x++) * (*y++);
  }
  return result;
}

/**
 * @brief  a naive implementation of the outer product of two arrays of equal type.
 * @tparam T the type of the arrays.
 * @param  dest a pointer the destination array.
 * @param  x a pointer the left array.
 * @param  y a pointer the right array.
 * @param  size the size of the arrays.
 */
template <storageOrdering, typename> struct OuterProductHelper;

template <typename T>
struct OuterProductHelper<storageOrdering::columnMajor, T> {

  static void outerProductImpl(T* dest, T const* x, T const* y, std::size_t const size_x,
                               std::size_t const size_y)
  {
    for(const auto yEnd = y + size_y; y != yEnd; ++y, dest += size_x)
    {
      multipleOf(dest, x, size_x, *y);
    }

    //const auto xEnd = x + size_x;
    //for(const auto yEnd = y + size_y; y != yEnd; ++y, x -= size_x)
    //{
    //  const auto yVal = *y;
    //  for(; x != xEnd; ++x, ++dest)
    //  {
    //    *dest = (*x) * yVal;
    //  }
    //}
  }
};

template <typename T>
struct OuterProductHelper<storageOrdering::rowMajor, T> {

  static void outerProductImpl(T* dest, T const* x, T const* y, std::size_t const size_x,
                               std::size_t const size_y)
  {
    for(const auto xEnd = x + size_x; x != xEnd; ++x, dest += size_y)
    {
      multipleOf(dest, y, size_y, *x);
    }

    //const auto yEnd = y + size_y;
    //for(const auto xEnd = x + size_x; x != xEnd; ++x, y -= size_y)
    //{
    //  const auto xVal = *x;
    //  for(;y != yEnd; ++y, ++dest)
    //  {
    //    *dest = xVal * (*y);
    //  }
    //}
  }
};

template <storageOrdering Ordering, typename T>
void outerProduct(T* dest, T const* x, T const* y, std::size_t const size_x,
                  std::size_t const size_y)
{
  OuterProductHelper<Ordering, T>::outerProductImpl(dest, x, y, size_x, size_y);
}

/**
 * @brief  a naive implementation of the matrix vector product.
 * @remark the matrix is assumed to be stored in row major order.
 * @tparam T the type of the matrix and the vector.
 * @param  dest a pointer the destination vector.
 * @param  matrix a pointer the array representing the matrix.
 * @param  vec a pointer the vector array.
 * @param  m the number of rows of the matrix.
 * @param  n the number of columns of the matrix.
 */
template <typename T>
inline void matrixVectorProduct(T* dest, T const* matrix, T const* vec, std::size_t const m, std::size_t const n)
{
  for(const auto end = dest + m; dest != end; ++dest, matrix += n)
  {
    simpleDotProduct(dest, matrix, vec, n);
  }
}

/**
 * @brief  a first parallel implementation of the matrix vector product.
 * @remark the matrix is assumed to be stored in row major order.
 * @tparam T the type of the matrix and the vector.
 * @param  dest a pointer the destination vector.
 * @param  matrix a pointer the array representing the matrix.
 * @param  vec a pointer the vector array.
 * @param  m the number of rows of the matrix.
 * @param  n the number of columns of the matrix.
 */
template <typename T>
inline void matrixVectorProductPar(T* dest, T const* matrix, T const* vec, std::size_t const m,
                                   std::size_t const n)
{
  static auto const hardwareThreads = static_cast<std::size_t>(std::thread::hardware_concurrency());

  static constexpr std::size_t minBlockSize{25ULL};

  auto const matrixSize = m * n;
  // matrixSize == 1 --> maxNumThreads = 1
  // matrixSize == k*minBlockSize + 1 --> maxNumThreads = k + 1
  const auto maxNumThreads = (std::min)((std::max)(hardwareThreads, 2ULL), (matrixSize + minBlockSize - 1ULL) / minBlockSize);

  // the block size has to be a multiple of the row size.
  auto blockSize = cpptools::alignUp(matrixSize / maxNumThreads, n);

  // now we can finally determine the number of threads.
  const auto numThreads = (matrixSize + blockSize - 1ULL) / blockSize;

  const auto rowsPerThread = blockSize / n;
  const auto end = dest + m;

  std::vector<std::future<void>> futures;
  futures.reserve(numThreads - 1ULL);
  for(std::size_t i{}; i < numThreads - 1ULL; ++i, dest += rowsPerThread, matrix += blockSize)
  {
    futures.push_back(std::async(std::launch::async, [=]() { matrixVectorProduct(dest, matrix, vec, rowsPerThread, n);}));
  }

  for(; dest != end; ++dest, matrix += n)
  {
    simpleDotProduct(dest, matrix, vec, n);
  }

  for(auto const& f : futures)
  {
    f.wait();
  }
}

/**
 * @brief  a first sequential implementation of the matrix matrix product.
 * @remark all matrices are assumed to be stored in column major order.
 * @tparam T the type of the matrices.
 * @param  dest a pointer the destination matrix.
 * @param  mat1 a pointer the array representing the left matrix.
 * @param  mat2 a pointer the array representing the right matrix.
 * @param  m the number of rows of the left matrix.
 * @param  n the number of columns of the left matrix and the number of rows of the right matrix.
 * @param  l the number of columns of the right matrix.
 */
template <typename T>
void mmProdParColMaj(T* dest, T const* mat1, T const* mat2, std::size_t const m,
                     std::size_t const n, std::size_t const l)
{
#define MULTITHREADED 0

#if MULTITHREADED
  static auto const hardwareThreads = static_cast<std::size_t>(std::thread::hardware_concurrency());
  // we have m * l * n multiplications
  //  static constexpr std::size_t minBlockSize{25ULL};

  const auto maxNumThreads = (std::min)((std::max)(hardwareThreads, 2ULL), l);
  const auto numThreads = maxNumThreads;
  std::vector<std::future<void>> futures;
  futures.reserve(numThreads - 1ULL);

  const auto sizeMat1 = m * n;
  auto const endMat1 = mat1 + m * n;
  auto const destEnd = dest + m * l;
  for(std::size_t i{}; i < numThreads - 1ULL; ++i, dest += m, mat2 += n)
  {
    futures.push_back(std::async(std::launch::async, [=]() mutable {
      multipleOf(dest, mat1, m, *mat2);
      ++mat2;
      mat1 += m;
      for(; mat1 != endMat1; ++mat2, mat1 += m)
      {
        addMultipleOf(dest, mat1, m, *mat2);
      }
      mat1 -= sizeMat1;    
    }));
  }

  for(; dest != destEnd; dest += m)
  {
    multipleOf(dest, mat1, m, *mat2);
    ++mat2;
    mat1 += m;
    for(; mat1 != endMat1; ++mat2, mat1 += m)
    {
      addMultipleOf(dest, mat1, m, *mat2);
    }
    mat1 -= sizeMat1;
  }

  for(auto const& f : futures)
  {
    f.wait();
  }
#else
  // m, n  x  n, l  -->  m, l
  const auto sizeMat1 = m * n;
  auto const endMat1 = mat1 + m * n;
  for(auto const destEnd = dest + m * l; dest != destEnd; dest += m)
  {
    multipleOf(dest, mat1, m, *mat2);
    ++mat2;
    mat1 += m;
    for(; mat1 != endMat1; ++mat2, mat1 += m)
    {
      addMultipleOf(dest, mat1, m, *mat2);
    }
    mat1 -= sizeMat1;
  }
#endif
}

/**
 * @brief  a first sequential implementation of the in place matrix matrix product.
 * @remark all matrices are assumed to be stored in column major order. The resulting
 *         matrix is stored in the leftmost array.
 * @tparam T the type of the matrices.
 * @param  mat1 a pointer the array representing the left matrix.
 * @param  mat2 a pointer the array representing the right matrix.
 * @param  m the number of rows of the left matrix.
 * @param  n the number of columns and rows of the left matrix and the number of rows of the right matrix.
 */
template <typename T>
void mmProdParColMajInPlace(T* mat1, T const* mat2, std::size_t const m, std::size_t const n)
{
  auto dest = std::make_unique<T[]>(m * n);
  mmProdParColMaj(dest.get(), mat1, mat2, m, n, n);
  std::memcpy(mat1, dest.get(), m * n * sizeof(T));
}


}   // namespace multiindex


// *************************************************************************** //
// ******************************* END OF FILE ******************************* //
// *************************************************************************** //

#endif // ARRAYOPERATIONS_H_16299848620354257801491122634181692861826826

