/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestVectorMatrixAlgebra.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    24.09.2020 
 */ 
 
#ifndef TESTVECTORMATRIXALGEBRA_H_1431917833118491710121939112111835671633 
#define TESTVECTORMATRIXALGEBRA_H_1431917833118491710121939112111835671633 
 
 
// includes
#include <MultiIndexVector/VectorMatrixAlgebra.h>
 
 
TEST(VectorMatrixAlgebra, MatrixTimesColumnVector)
{
  constexpr std::size_t size1 = 7;
  constexpr std::size_t size2 = 5;

  StackMatrix<std::size_t, storageOrdering::rowMajor, size1, size2> matrix;
  for(std::size_t i{}; i < size1; ++i)
  {
    for(std::size_t j{}; j < size2; ++j)
    {
      matrix(i, j) = (i + 1ULL) * (j + 1ULL);
    }
  }

  constexpr ColumnStackVector<std::size_t, size2> v{1ULL,2ULL,3ULL,4ULL,5ULL};
  constexpr ColumnStackVector<std::size_t, size1> expectedValue{55ULL,55ULL*2ULL,55ULL*3ULL,55ULL*4ULL,55ULL*5ULL,55ULL*6ULL,55ULL*7ULL};
  const auto result = matrix * v;
  EXPECT_EQ(result, expectedValue);
}

TEST(VectorMatrixAlgebra, RowVectorTimesMatrix)
{
  constexpr std::size_t size1 = 5;
  constexpr std::size_t size2 = 7;

  StackMatrix<std::size_t, storageOrdering::columnMajor, size1, size2> matrix;
  for(std::size_t i{}; i < size2; ++i)
  {
    for(std::size_t j{}; j < size1; ++j)
    {
      matrix(j, i) = (i + 1ULL) * (j + 1ULL);
    }
  }

  constexpr RowStackVector<std::size_t, size1> v{1ULL,2ULL,3ULL,4ULL,5ULL};
  constexpr RowStackVector<std::size_t, size2> expectedValue{55ULL,55ULL*2ULL,55ULL*3ULL,55ULL*4ULL,55ULL*5ULL,55ULL*6ULL,55ULL*7ULL};
  const auto result = v * matrix;
  EXPECT_EQ(result, expectedValue);
}

TEST(VectorMatrixAlgebra, MatrixTimesMatrixColumnMajorOnlyOneEntry)
{
  constexpr std::size_t m = 1ULL;
  constexpr std::size_t n = 1ULL;
  constexpr std::size_t l = 1ULL;
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, m, n> m1{9ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, n, l> m2{6ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, n, l> expected{54ULL};
  const auto result = m1 * m2;
  EXPECT_EQ(result, expected);
}

TEST(VectorMatrixAlgebra, MatrixTimesMatrixColumnMajor)
{
  constexpr std::size_t m = 3ULL;
  constexpr std::size_t n = 3ULL;
  constexpr std::size_t l = 2ULL;
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, m, n> m1{1ULL, 4ULL, 7ULL, 2ULL, 5ULL, 8ULL, 3ULL, 6ULL, 9ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, n, l> m2{1ULL, 3ULL, 5ULL, 2ULL, 4ULL, 6ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, n, l> expected{22ULL, 49ULL, 76ULL, 28ULL, 64ULL, 100ULL};
  const auto result = m1 * m2;
  EXPECT_EQ(result, expected);
}

TEST(VectorMatrixAlgebra, MatrixTimesMatrixColumnMajorSquare)
{
  constexpr std::size_t m = 2ULL;
  constexpr std::size_t n = 2ULL;
  constexpr std::size_t l = 2ULL;
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, m, n> m1{1ULL, 0ULL, 1ULL, 1ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, n, l> m2{1ULL, 1ULL, 0ULL, 1ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::columnMajor, n, l> expected{2ULL, 1ULL, 1ULL, 1ULL};
  const auto result = m1 * m2;
  EXPECT_EQ(result, expected);
}


TEST(VectorMatrixAlgebra, MatrixTimesMatrixRowMajorOnlyOneEntry)
{
  constexpr std::size_t m = 1ULL;
  constexpr std::size_t n = 1ULL;
  constexpr std::size_t l = 1ULL;
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, m, n> m1{9ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, n, l> m2{6ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, n, l> expected{54ULL};
  const auto result = m1 * m2;
  EXPECT_EQ(result, expected);
}

TEST(VectorMatrixAlgebra, MatrixTimesMatrixRowMajor)
{
  constexpr std::size_t m = 3ULL;
  constexpr std::size_t n = 3ULL;
  constexpr std::size_t l = 2ULL;
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, m, n> m1{1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 6ULL, 7ULL, 8ULL, 9ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, n, l> m2{1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 6ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, n, l> expected{22ULL, 28ULL, 49ULL, 64ULL, 76ULL, 100ULL};
  const auto result = m1 * m2;
  EXPECT_EQ(result, expected);
}

TEST(VectorMatrixAlgebra, MatrixTimesMatrixRowMajorSquare)
{
  constexpr std::size_t m = 2ULL;
  constexpr std::size_t n = 2ULL;
  constexpr std::size_t l = 2ULL;
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, m, n> m1{1ULL, 1ULL, 0ULL, 1ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, n, l> m2{1ULL, 0ULL, 1ULL, 1ULL};
  constexpr StackMatrix<std::size_t, storageOrdering::rowMajor, n, l> expected{2ULL, 1ULL, 1ULL, 1ULL};
  const auto result = m1 * m2;
  EXPECT_EQ(result, expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTVECTORMATRIXALGEBRA_H_1431917833118491710121939112111835671633 
