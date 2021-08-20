/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    DurationTypesTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.06.2021 
 */ 
 
#ifndef DURATIONTYPESTEST_H_87422077381227581128561315312038202332329 
#define DURATIONTYPESTEST_H_87422077381227581128561315312038202332329 
 
 
// includes
#include <Benchmark/DurationTypes.h>

using namespace std::string_view_literals;



TEST(DurationTypes, UnitRepresentationWeeksWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::weeks, wchar_t>), L"week"sv);
}

TEST(DurationTypes, unitStringMethodWeeksWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::weeks::zero()), L"week"sv);
}

TEST(DurationTypes, UnitRepresentationDaysWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::days, wchar_t>), L"d"sv);
}

TEST(DurationTypes, unitStringMethodDaysWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::days::zero()), L"d"sv);
}

TEST(DurationTypes, UnitRepresentationHoursWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::hours, wchar_t>), L"h"sv);
}

TEST(DurationTypes, unitStringMethodHoursWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::hours::zero()), L"h"sv);
}

TEST(DurationTypes, UnitRepresentationMinutesWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::minutes, wchar_t>), L"min"sv);
}

TEST(DurationTypes, unitStringMethodMinutesWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::minutes::zero()), L"min"sv);
}

TEST(DurationTypes, UnitRepresentationSecondsWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::seconds, wchar_t>), L"s"sv);
}

TEST(DurationTypes, unitStringMethodSecondsWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::seconds::zero()), L"s"sv);
}

TEST(DurationTypes, UnitRepresentationMilliSecondsWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::milliSeconds, wchar_t>), L"ms"sv);
}

TEST(DurationTypes, unitStringMethodMilliSecondsWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::milliSeconds::zero()), L"ms"sv);
}

TEST(DurationTypes, UnitRepresentationMicroSecondsWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::microSeconds, wchar_t>), L"mus"sv);
}

TEST(DurationTypes, unitStringMethodMicroSecondsWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::microSeconds::zero()), L"mus"sv);
}

TEST(DurationTypes, UnitRepresentationNanoSecondsWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::nanoSeconds, wchar_t>), L"ns"sv);
}

TEST(DurationTypes, unitStringMethodNanoSecondsWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::nanoSeconds::zero()), L"ns"sv);
}

TEST(DurationTypes, UnitRepresentationPicoSecondsWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::picoSeconds, wchar_t>), L"ps"sv);
}

TEST(DurationTypes, unitStringMethodPicoSecondsWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::picoSeconds::zero()), L"ps"sv);
}

TEST(DurationTypes, UnitRepresentationFemtoSecondsChar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::femtoSeconds, char>), "fs"sv);
}

TEST(DurationTypes, unitStringMethodFemtoSecondsChar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::femtoSeconds::zero()), L"fs"sv);
}

TEST(DurationTypes, UnitRepresentationFemtoSecondsWchar)
{
  EXPECT_EQ((benchmark::DurationString<benchmark::femtoSeconds, wchar_t>), L"fs"sv);
}

TEST(DurationTypes, unitStringMethodFemtoSecondsWchar)
{
  EXPECT_EQ(benchmark::unitString(benchmark::femtoSeconds::zero()), L"fs"sv);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DURATIONTYPESTEST_H_87422077381227581128561315312038202332329 
