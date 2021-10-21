/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    MultiIndexBitArrayAccessorTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.08.2021 
 */ 
 
#ifndef MULTIINDEXBITARRAYACCESSORTEST_H_19214250021233332076237842210665706008706 
#define MULTIINDEXBITARRAYACCESSORTEST_H_19214250021233332076237842210665706008706 
 
 
// includes 
#include <Bitwise/details/MultiIndexBitArrayAccessor.h>

#include <memory>
 

TEST(MultiIndexBitArrayAccessor, OneDimensionalStatic)
{
  using type = std::uint8_t;
  constexpr std::size_t rank = 1ULL;
  constexpr auto ordering = multiindex::storageOrdering::columnMajor;
  using accessorType = bws::MultiIndexBitArrayAccessor<type, rank, ordering>;

  type myData[2ULL]{ 0b11010101, 0b11000011 };
  std::size_t dim[1ULL]{ 16ULL };


  accessorType accessor{myData, dim};

  auto b0 = accessor(0ULL);
  EXPECT_TRUE(b0);
  auto b1 = accessor(1ULL);
  EXPECT_FALSE(b1);
  auto b2 = accessor(2ULL);
  EXPECT_TRUE(b2);

}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTIINDEXBITARRAYACCESSORTEST_H_19214250021233332076237842210665706008706 
