/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestMultiIndexVector.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    27.08.2020 
 */ 
 
#ifndef TESTMULTIINDEXVECTOR_H_2481726069194431738477579075403530828647 
#define TESTMULTIINDEXVECTOR_H_2481726069194431738477579075403530828647 
 
 
// includes
#include <MultiIndexVector/MultiIndexVector.h>

using namespace multiindex;

 
TEST(MultiIndexVector, MultiIndexVectorDefaultConstructor)
{
  MultiIndexVector<int, 3, storageOrdering::columnMajor> multVec;
  EXPECT_TRUE(multVec.empty());
  EXPECT_EQ(multVec.size(), 0);
  EXPECT_EQ(multVec.size(0), 0);
  EXPECT_EQ(multVec.size(1), 0);
  EXPECT_EQ(multVec.size(2), 0);
}

TEST(MultiIndexVector, MultiIndexVectorConstructorWIthParameters)
{
  const std::size_t d1 = 3;
  const std::size_t d2 = 35;
  const std::size_t d3 = 313;

  MultiIndexVector<int, 3, storageOrdering::columnMajor> multVec(d1, d2, d3);
  EXPECT_FALSE(multVec.empty());
  EXPECT_EQ(multVec.size(), d1*d2*d3);
  EXPECT_EQ(multVec.size(0), d1);
  EXPECT_EQ(multVec.size(1), d2);
  EXPECT_EQ(multVec.size(2), d3);
}

TEST(MultiIndexVector, MultiIndexVectorResize)
{
  const std::size_t d1 = 31;
  const std::size_t d2 = 3;
  const std::size_t d3 = 33;

  MultiIndexVector<int, 3, storageOrdering::columnMajor> multVec;
  multVec.resize(d1, d2, d3);
  EXPECT_FALSE(multVec.empty());
  EXPECT_EQ(multVec.size(), d1*d2*d3);
  EXPECT_EQ(multVec.size(0), d1);
  EXPECT_EQ(multVec.size(1), d2);
  EXPECT_EQ(multVec.size(2), d3);
}

TEST(MultiIndexVector, MultiIndexVectorConstructorWithInitializerList)
{
  const std::size_t d1 = 2;
  const std::size_t d2 = 2;

  MultiIndexVector<int, 2, storageOrdering::columnMajor> multVec({1,2,3,4}, d1, d2);
  EXPECT_FALSE(multVec.empty());
  EXPECT_EQ(multVec(0,0), 1);
  EXPECT_EQ(multVec(0,1), 3);
  EXPECT_EQ(multVec(1,0), 2);
  EXPECT_EQ(multVec(1,1), 4);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTMULTIINDEXVECTOR_H_2481726069194431738477579075403530828647 
