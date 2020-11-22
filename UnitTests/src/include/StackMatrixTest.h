/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StackMatrixTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.10.2020 
 */ 
 
#ifndef STACKMATRIXTEST_H_15109105011527274177812526448541630516524 
#define STACKMATRIXTEST_H_15109105011527274177812526448541630516524 
 
 
// includes
#include <MultiIndexVector/VectorMatrixAlgebra.h>


using namespace multiindex;
 
 
TEST(StackMatrix, Determinant)
{
  constexpr SquareStackMatrix<int, storageOrdering::columnMajor, 2ULL> squareMatrix{12, 4, 3, 27};
  constexpr auto det = squareMatrix.det();
  constexpr int expected = 312;
  EXPECT_EQ(det, expected);
}

TEST(StackMatrix, InverseRowMajor)
{
  constexpr auto ordering = storageOrdering::rowMajor;
  constexpr SquareStackMatrix<double, ordering, 2ULL> squareMatrix{12.0, 4.0, 3.0, 9.0};
  const auto inverseMatrix = multiindex::invert(squareMatrix);
  constexpr SquareStackMatrix<double, ordering, 2ULL> unity{1.0, 0.0, 0.0, 1.0};
  const auto prod = squareMatrix * inverseMatrix;
  EXPECT_EQ(prod, unity);
}

TEST(StackMatrix, InverseColumnMajor)
{
  constexpr auto ordering = storageOrdering::columnMajor;
  constexpr SquareStackMatrix<double, ordering, 2ULL> squareMatrix{12.0, 4.0, 3.0, 9.0};
  const auto inverseMatrix = multiindex::invert(squareMatrix);
  constexpr SquareStackMatrix<double, ordering, 2ULL> unity{1.0, 0.0, 0.0, 1.0};
  const auto prod = squareMatrix * inverseMatrix;
  EXPECT_EQ(prod, unity);
}

 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STACKMATRIXTEST_H_15109105011527274177812526448541630516524 
