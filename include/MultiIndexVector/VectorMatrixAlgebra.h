/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    VectorMatrixAlgebra.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    23.09.2020 
 */ 
 
#ifndef VECTORMATRIXALGEBRA_H_28835227752228921360262374173142451080221501 
#define VECTORMATRIXALGEBRA_H_28835227752228921360262374173142451080221501 
 
 
// includes
#include <MultiIndexVector/StackMatrix.h>
#include <MultiIndexVector/StackVector.h>


namespace multiindex {


/**
 * @brief outer product of a column vector and a row vector of the same type.
 */
template <typename T, std::size_t N, std::size_t M>
inline auto operator*(ColumnStackVector<T, N> const& x, RowStackVector<T, M> const& y)
{
  static constexpr auto ordering = storageOrdering::rowMajor;
  StackMatrix<T, ordering, N, M> result;
  outerProduct<ordering>(result.data(), x.data(), y.data(), N, M);
  return result;
}

/**
 * @brief matrix vector product.
 */
template <typename T, std::size_t M, std::size_t N>
inline auto operator*(StackMatrix<T, storageOrdering::rowMajor, M, N> const& matrix, ColumnStackVector<T, N> const& x)
{
  ColumnStackVector<T, M> result;
  matrixVectorProductPar(result.data(), matrix.data(), x.data(), M, N);
//  matrixVectorProduct(result.data(), matrix.data(), x.data(), M, N);
  return result;
}

/**
 * @brief vector matrix product.
 */
template <typename T, std::size_t M, std::size_t N>
inline auto operator*(RowStackVector<T, M> const& x, StackMatrix<T, storageOrdering::columnMajor, M, N> const& matrix)
{
  RowStackVector<T, N> result;
  matrixVectorProductPar(result.data(), matrix.data(), x.data(), N, M);
//  matrixVectorProduct(result.data(), matrix.data(), x.data(), N, M);
  return result;
}

/**
 * @brief matrix matrix product.
 */
template <typename T, std::size_t M, std::size_t N, std::size_t L>
inline auto operator*(StackMatrix<T, storageOrdering::columnMajor, M, N> const& mat1,
                      StackMatrix<T, storageOrdering::columnMajor, N, L> const& mat2)
{
  StackMatrix<T, storageOrdering::columnMajor, M, L> result;
  mmProdParColMaj(result.data(), mat1.data(), mat2.data(), M, N, L);
  return result;
}

/**
 * @brief matrix matrix product.
 */
template <typename T, std::size_t M, std::size_t N, std::size_t L>
inline auto operator*(StackMatrix<T, storageOrdering::rowMajor, M, N> const& mat1,
                      StackMatrix<T, storageOrdering::rowMajor, N, L> const& mat2)
{
  StackMatrix<T, storageOrdering::rowMajor, M, L> result;
  mmProdParColMaj(result.data(), mat2.data(), mat1.data(), L, N, M);
  return result;
}


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // VECTORMATRIXALGEBRA_H_28835227752228921360262374173142451080221501 
