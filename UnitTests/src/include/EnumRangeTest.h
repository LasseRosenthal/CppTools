/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    EnumRangeTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    08.07.2020
 */ 
 
#ifndef ENUMRANGETEST_H_1530746186853021831591185922501625785200 
#define ENUMRANGETEST_H_1530746186853021831591185922501625785200 
 
 
// includes
#include <Utils/EnumRange.h>

#include <numeric>
#include <string>
#include <vector>

using namespace std::string_literals;

constexpr int firstEnumValue = 3;

enum class Days : int {
  monday = firstEnumValue,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday,
  sunday
};

std::string daysToStr(Days d)
{
  switch(d)
  {
    case Days::monday    : return "monday"s;
    case Days::tuesday   : return "tuesday"s;
    case Days::wednesday : return "wednesday"s;
    case Days::thursday  : return "thursday"s;
    case Days::friday    : return "friday"s;
    case Days::saturday  : return "saturday"s;
    case Days::sunday    : return "sunday"s;
    default              : return "default"s;
  }
}


using DaysEnumUtils =
  cpptools::EnumUtils<Days, Days::monday, Days::tuesday, Days::wednesday, Days::thursday,
                      Days::friday, Days::saturday, Days::sunday>;

using DaysRange = DaysEnumUtils::EnumRange;

TEST(EnumRange, LoopConvertToInt)
{
  std::vector<int> expectedDaysVals(7);
  std::iota(expectedDaysVals.begin(), expectedDaysVals.end(), firstEnumValue);

  std::vector<int> daysVals;
  daysVals.reserve(7);

  for(auto d : DaysRange())
  {
    daysVals.push_back(static_cast<int>(d));
  }

  EXPECT_EQ(daysVals, expectedDaysVals);
}

TEST(EnumRange, LoopConvertToString)
{
  std::vector<std::string> expectedDays{"monday"s, "tuesday"s, "wednesday"s, "thursday"s,
                                        "friday"s, "saturday"s, "sunday"s};

  std::vector<std::string> days;
  days.reserve(7);

  //const DaysRange daysRange;
  //for(auto d = daysRange.begin(); d != daysRange.end(); ++d)
  //{
  //  days.push_back(daysToStr(*d));
  //}

  for(auto d : DaysRange())
  {
    days.push_back(daysToStr(d));
  }

  EXPECT_EQ(days, expectedDays);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ENUMRANGETEST_H_1530746186853021831591185922501625785200 
