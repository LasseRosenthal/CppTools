/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    TestMultiArrayAccessor.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.08.2020 
 */ 
 
#ifndef TESTMULTIARRAYACCESSOR_H_101523667204031172487451395832612281811276 
#define TESTMULTIARRAYACCESSOR_H_101523667204031172487451395832612281811276 
 
 
// includes
#include <MultiIndexVector/MultiArrayAccessor.h>

#include <numeric>

using namespace multiindex;
 
 
TEST(MultiIndexVector, MultiArrayAccessorRowMajor2D)
{
  constexpr std::size_t D = 2;
  std::size_t Dims[D] = {3,2};

  int vals[6];
  std::iota(std::begin(vals), std::end(vals), 1);

  MultiArrayAccessor<int, D, storageOrdering::rowMajor> acc{Dims, vals};

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      EXPECT_EQ(acc(i,j), i*Dims[1] + j + 1);
    }
  }
}

TEST(MultiIndexVector, MultiArrayAccessorColumnMajor2D)
{
  constexpr std::size_t D = 2;
  std::size_t Dims[D] = {3,2};

  int vals[6];
  std::iota(std::begin(vals), std::end(vals), 1);

  MultiArrayAccessor<int, D, storageOrdering::columnMajor> acc{Dims, vals};

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      EXPECT_EQ(acc(i,j), i + j*Dims[0] + 1);
    }
  }
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // TESTMULTIARRAYACCESSOR_H_101523667204031172487451395832612281811276 
