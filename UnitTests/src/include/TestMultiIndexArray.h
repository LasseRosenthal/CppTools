/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestMultiIndexArray.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    27.08.2020 
 */ 
 
#ifndef TESTMULTIINDEXARRAY_H_3440123343189417766156272897160192086115837 
#define TESTMULTIINDEXARRAY_H_3440123343189417766156272897160192086115837 
 
 
// includes 
#include <MultiIndexVector/MultiIndexArray.h>
#include "DummyClass.h"

using namespace multiindex;

 
TEST(MultiIndexArray, DefaultConstructor)
{
  constexpr std::size_t d1 = 37;
  constexpr std::size_t d2 = 12;
  constexpr std::size_t d3 = 135;

  MultiIndexArray<int, storageOrdering::columnMajor, d1, d2, d3> multVec;
  EXPECT_FALSE(multVec.empty());
  EXPECT_EQ(multVec.size(), d1*d2*d3);
  EXPECT_EQ(multVec.size(0), d1);
  EXPECT_EQ(multVec.size(1), d2);
  EXPECT_EQ(multVec.size(2), d3);
}

TEST(MultiIndexArray, CopyContructorTestEquality)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray1{0,1,2,3,4,5};
  auto multArray2(multArray1);

  const bool isEqual = multArray1 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, TemplatedCopyContructorTestEquality)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray1{0,1,2,3,4,5};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,1.0,2.0,3.0,4.0,5.0};

  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3(multArray1);

  const bool isEqual = multArray3 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, IndexOperatorZeroInitialized)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 2;

  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multVec;
  for(std::size_t i = 0; i < d1; ++i)
  {
    for(std::size_t j = 0; j < d2; ++j)
    {
      EXPECT_EQ(multVec(i,j), 0);
    }
  }
}

TEST(MultiIndexArray, IndexOperatorInitialValue)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 2;
  constexpr int initialValue = 1345;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multVec{initialValue};
  for(std::size_t i = 0; i < d1; ++i)
  {
    for(std::size_t j = 0; j < d2; ++j)
    {
      EXPECT_EQ(multVec(i,j), initialValue);
    }
  }
}

TEST(MultiIndexArray, IndexOperatorInitializerListColumnMajor)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multVec{1,2,3,4,5,6};
  EXPECT_EQ(multVec(0,0), 1);
  EXPECT_EQ(multVec(0,1), 3);
  EXPECT_EQ(multVec(0,2), 5);
  EXPECT_EQ(multVec(1,0), 2);
  EXPECT_EQ(multVec(1,1), 4);
  EXPECT_EQ(multVec(1,2), 6);
}

TEST(MultiIndexArray, IndexOperatorInitializerListRowMajor)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::rowMajor, d1, d2> multVec{1,2,3,4,5,6};
  EXPECT_EQ(multVec(0,0), 1);
  EXPECT_EQ(multVec(0,1), 2);
  EXPECT_EQ(multVec(0,2), 3);
  EXPECT_EQ(multVec(1,0), 4);
  EXPECT_EQ(multVec(1,1), 5);
  EXPECT_EQ(multVec(1,2), 6);
}

TEST(MultiIndexArray, DirectDataAccessRowMajor)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::rowMajor, d1, d2> multVec{0,1,2,3,4,5};

  auto ptr = multVec.data();
  for(int i = 0; i < multVec.size(); ++i)
  {
    EXPECT_EQ(*ptr++, i);
  }
}

TEST(MultiIndexArray, DirectDataAccessColumnMajor)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multVec{0,1,2,3,4,5};

  auto ptr = multVec.data();
  for(int i = 0; i < multVec.size(); ++i)
  {
    EXPECT_EQ(*ptr++, i);
  }
}

TEST(MultiIndexArray, ModifyValue)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  MultiIndexArray<Person, storageOrdering::columnMajor, d1, d2> multVec;

  multVec(1,1).setAge(38);
  multVec(1,1).setSize(192);

  EXPECT_EQ(multVec(1,1).getAge(), 38);
  EXPECT_EQ(multVec(1,1).getSize(), 192);
}

TEST(MultiIndexArray, ComparisonExpectFalse)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray1{0,1,2,3,4,5};
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{0,1,2,3,4,52};

  const bool isEqual = multArray1 == multArray2;
  EXPECT_FALSE(isEqual);
}

TEST(MultiIndexArray, ComparisonExpectTrue)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray1{0,1,2,3,4,5};
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{0,1,2,3,4,5};

  const bool isEqual = multArray1 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, Assignment)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray1{0,1,2,3,4,5};
  MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{14,1,22,3,41,59};

  multArray2 = multArray1;

  const bool isEqual = multArray1 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, CompoundMultiplication)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};

  multArray1 *= 3;

  const bool isEqual = multArray1 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, MultiplicationFactorRight)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};

  const auto multArray3 = multArray1 * 3.0;

  const bool isEqual = multArray3 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, MultiplicationFactorLeft)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};

  const auto multArray3 = 3.0 * multArray1;

  const bool isEqual = multArray3 == multArray2;
  EXPECT_TRUE(isEqual);
}


TEST(MultiIndexArray, Division)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};

  const auto multArray3 = multArray2 / 3.0;

  const bool isEqual = multArray3 == multArray1;
  EXPECT_TRUE(isEqual);
}


TEST(MultiIndexArray, CompoundDivision)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.5,7.0,10.5,14.0,17.5};

  multArray2 /= 3.5;

  const bool isEqual = multArray1 == multArray2;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, CompoundAddition)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};

  multArray1 += multArray2;

  const bool isEqual = multArray1 == multArray3;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, CompoundAdditionDifferentTypes)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{0,3,6,9,12,15};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};

  multArray1 += multArray2;

  const bool isEqual = multArray1 == multArray3;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, Addition)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};
  const auto multArray4 = multArray1 + multArray2;
  const bool isEqual = multArray4 == multArray3;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, AdditionDifferentTypes)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{0,3,6,9,12,15};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};

  auto multArray4 = multArray1 + multArray2;

  const bool isEqual = multArray4 == multArray3;
  EXPECT_TRUE((isEqual && std::is_same_v<decltype(multArray4), MultiIndexArray<double, storageOrdering::columnMajor, d1, d2>>));
}

TEST(MultiIndexArray, CompoundSubtraction)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};
  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};

  multArray3 -= multArray2;

  const bool isEqual = multArray1 == multArray3;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, CompoundSubtractionDifferentTypes)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{0,3,6,9,12,15};
  MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};

  multArray3 -= multArray2;

  const bool isEqual = multArray1 == multArray3;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, Subtraction)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray2{0.0,3.0,6.0,9.0,12.0,15.0};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};
  const auto multArray4 = multArray3 - multArray2;
  const bool isEqual = multArray4 == multArray1;
  EXPECT_TRUE(isEqual);
}

TEST(MultiIndexArray, SubtractionDifferentTypes)
{
  constexpr std::size_t d1 = 2;
  constexpr std::size_t d2 = 3;
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray1{0.0,1.0,2.0,3.0,4.0,5.0};
  constexpr MultiIndexArray<int, storageOrdering::columnMajor, d1, d2> multArray2{0,3,6,9,12,15};
  constexpr MultiIndexArray<double, storageOrdering::columnMajor, d1, d2> multArray3{0.0,4.0,8.0,12.0,16.0,20.0};

  auto multArray4 = multArray3 - multArray2;

  const bool isEqual = multArray4 == multArray1;
  EXPECT_TRUE((isEqual && std::is_same_v<decltype(multArray4), MultiIndexArray<double, storageOrdering::columnMajor, d1, d2>>));
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTMULTIINDEXARRAY_H_3440123343189417766156272897160192086115837 
