/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    EnumBitmaskTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    16.03.2021 
 */ 
 
#ifndef ENUMBITMASKTEST_H_5727257401471110831161752656582092712422820 
#define ENUMBITMASKTEST_H_5727257401471110831161752656582092712422820 
 
 
// includes
#include <Utils/EnumBitmask.h>


enum class permissions {
  none = 0,
  readable = 1,
  writable = 1 << 1,
  copyable = 1 << 2
};

template <>
struct cpptools::EnableBitmaskOperationsT<permissions> : std::true_type{};

TEST(EnumBitmask, combineFlags)
{
  constexpr auto rc = permissions::readable | permissions::copyable;
  EXPECT_EQ(rc & permissions::readable, permissions::readable);
  EXPECT_EQ(rc & permissions::copyable, permissions::copyable);
  EXPECT_EQ(rc & permissions::writable, permissions::none);
}

TEST(EnumBitmask, clearFlag)
{
  auto rc = permissions::readable | permissions::copyable;
  rc &= ~permissions::copyable;

  EXPECT_EQ(rc & permissions::readable, permissions::readable);
  EXPECT_EQ(rc & permissions::copyable, permissions::none);
}

TEST(EnumBitmask, setFlag)
{
  auto rc = permissions::readable;
  EXPECT_EQ(rc & permissions::copyable, permissions::none);

  rc |= permissions::copyable;
  EXPECT_EQ(rc & permissions::copyable, permissions::copyable);
}

 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ENUMBITMASKTEST_H_5727257401471110831161752656582092712422820 
