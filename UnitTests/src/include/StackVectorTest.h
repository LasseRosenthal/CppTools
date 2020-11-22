/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StackVectorTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    22.09.2020 
 */ 
 
#ifndef STACKVECTORTEST_H_3226379123174815589274822448319198683730439 
#define STACKVECTORTEST_H_3226379123174815589274822448319198683730439 
 
 
// includes
#include <MultiIndexVector/VectorMatrixAlgebra.h>
 
using namespace multiindex;


TEST(StackVector, CompoundMultiplicationWithScalar)
{
  constexpr std::size_t size = 5;
  ColumnStackVector<int, size> v{1,2,3,4,5};
  ColumnStackVector<int, size> expectedValue{3,6,9,12,15};
  v *= 3;
  EXPECT_TRUE((std::is_same_v<decltype(v), decltype(expectedValue)>));
  EXPECT_EQ(v, expectedValue);
}

TEST(StackVector, MultiplicationWithScalar)
{
  constexpr std::size_t size = 5;
  ColumnStackVector<int, size> v{1,2,3,4,5};
  ColumnStackVector<int, size> expectedValue{3,6,9,12,15};
  auto multiple = 3 * v;
  EXPECT_TRUE((std::is_same_v<decltype(multiple), decltype(expectedValue)>));
  EXPECT_EQ(multiple, expectedValue);
}

TEST(StackVector, DotProductTwoColumnVectors)
{
  constexpr std::size_t size = 5;
  ColumnStackVector<int, size> v1{1,2,3,4,5};
  ColumnStackVector<int, size> v2{1,2,3,4,5};
  const auto dot = v1 * v2;
  constexpr auto expectedValue = 55;
  EXPECT_EQ(dot, expectedValue);
}

TEST(StackVector, DotProductTwoColumnVectorsDifferentTypes)
{
  constexpr std::size_t size = 5;
  ColumnStackVector<int, size> v1{1,2,3,4,5};
  ColumnStackVector<unsigned int, size> v2{1U,2U,3U,4U,5U};
  const auto dot = v1 * v2;
  constexpr auto expectedValue = 55;
  EXPECT_EQ(dot, expectedValue);
}

TEST(StackVector, DotProductTwoColumnVectorsWithMultiplication)
{
  constexpr std::size_t size = 5;
  ColumnStackVector<int, size> v1{1,2,3,4,5};
  ColumnStackVector<int, size> v2{1,2,3,4,5};
  const auto dot = (v1 * 4) * (2 * v2);
  constexpr auto expectedValue = 55 * 2 * 4;
  EXPECT_EQ(dot, expectedValue);
}

TEST(StackVector, DotProductTwoRowVectors)
{
  constexpr std::size_t size = 5;
  RowStackVector<int, size> v1{1,2,3,4,5};
  RowStackVector<int, size> v2{1,2,3,4,5};
  const auto dot = v1 * v2;
  constexpr auto expectedValue = 55;
  EXPECT_EQ(dot, expectedValue);
}

TEST(StackVector, DotProductRowVectorTimesColumnVector)
{
  constexpr std::size_t size = 5;
  RowStackVector<int, size> v1{1,2,3,4,5};
  ColumnStackVector<int, size> v2{1,2,3,4,5};
  const auto dot = v1 * v2;
  constexpr auto expectedValue = 55;
  EXPECT_EQ(dot, expectedValue);
}

TEST(StackVector, OuterProduct)
{
  constexpr std::size_t size1 = 5;
  constexpr std::size_t size2 = 4;

  ColumnStackVector<int, size1> v1{1,2,3,4,5};
  RowStackVector<int, size2> v2{1,2,3,4};
  const auto outerProduct = v1 * v2;
  const StackMatrix<int, storageOrdering::rowMajor, size1, size2> expectedValue{
    1,2,3,4,
    2,4,6,8,
    3,6,9,12,
    4,8,12,16,
    5,10,15,20};
  EXPECT_EQ(outerProduct, expectedValue);
}

TEST(StackVector, magnitudeIntVector)
{
  constexpr std::size_t size = 4;
  ColumnStackVector<int, size> v{1,3,3,4};
  const double expectedValue = std::sqrt(35.0);
  EXPECT_EQ(v.magnitude(), expectedValue);
}

TEST(StackVector, magnitudeDoubleVector)
{
  constexpr std::size_t size = 2;
  ColumnStackVector<double, size> v{3.0, 4.0};
  constexpr double expectedValue = 5.0;
  EXPECT_EQ(v.magnitude(), expectedValue);
}

TEST(StackVector, orientedAngleZero)
{
  constexpr std::size_t size = 2;
  ColumnStackVector<double, size> v1{3.0, 4.0};
  ColumnStackVector<double, size> v2{3.0, 4.0};

  constexpr double expectedValue = 0.0;
  EXPECT_EQ(orientedAngle(v1, v2), expectedValue);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STACKVECTORTEST_H_3226379123174815589274822448319198683730439 
