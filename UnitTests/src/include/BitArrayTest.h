/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitArrayTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    27.08.2021 
 */ 
 
#ifndef BITARRAYTEST_H_2845311865985201222110732464298693127027834 
#define BITARRAYTEST_H_2845311865985201222110732464298693127027834 
 
 
// includes
#include <Bitwise/BitArray.h>
 
 
TEST(BitArray, byteTypeNumRegions)
{
  EXPECT_TRUE((std::is_same_v<bws::BitArray<6ULL>::storageType, std::uint8_t>));
  EXPECT_TRUE((std::is_same_v<bws::BitArray<11ULL>::storageType, std::uint16_t>));
  EXPECT_TRUE((std::is_same_v<bws::BitArray<27ULL>::storageType, std::uint32_t>));
  EXPECT_TRUE((std::is_same_v<bws::BitArray<32ULL>::storageType, std::uint32_t>));
  EXPECT_TRUE((std::is_same_v<bws::BitArray<34ULL>::storageType, std::uint64_t>));
  EXPECT_TRUE((std::is_same_v<bws::BitArray<64ULL>::storageType, std::uint64_t>));
  EXPECT_TRUE((std::is_same_v<bws::BitArray<65ULL>::storageType, std::uint64_t>));

  EXPECT_EQ(bws::BitArray<6ULL>::numRegions, 1ULL);
  EXPECT_EQ(bws::BitArray<11ULL>::numRegions, 1ULL);
  EXPECT_EQ(bws::BitArray<26ULL>::numRegions, 1ULL);
  EXPECT_EQ(bws::BitArray<32ULL>::numRegions, 1ULL);
  EXPECT_EQ(bws::BitArray<33ULL>::numRegions, 1ULL);
  EXPECT_EQ(bws::BitArray<64ULL>::numRegions, 1ULL);
  EXPECT_EQ(bws::BitArray<65ULL>::numRegions, 2ULL);
  EXPECT_EQ(bws::BitArray<128ULL>::numRegions, 2ULL);
  EXPECT_EQ(bws::BitArray<129ULL>::numRegions, 3ULL);
  EXPECT_EQ(bws::BitArray<15'888ULL>::numRegions, 249ULL);
  EXPECT_EQ(bws::BitArray<1'650'499ULL>::numRegions, 25'790ULL);
}

TEST(BitArray, constexprCtor)
{
//  bws::BitArray<6ULL> myBitArray(true);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITARRAYTEST_H_2845311865985201222110732464298693127027834 
