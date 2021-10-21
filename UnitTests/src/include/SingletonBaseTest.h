/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    SingletonBaseTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal
 * @date    30.09.2021 
 */ 
 
#ifndef SINGLETONBASETEST_H_238709594214091134120947199847731906415849 
#define SINGLETONBASETEST_H_238709594214091134120947199847731906415849 
 
 
// includes
#include <Utils/SingletonBase.h>

#include <string>

using namespace std::string_literals;


namespace cppttest {


template<cpptools::singletonMode Mode, bool EnableWithoutInit>
class mySingleton : public cpptools::SingletonBase<mySingleton<Mode, EnableWithoutInit>, Mode, EnableWithoutInit> {

  friend cpptools::SingletonBase<mySingleton<Mode, EnableWithoutInit>, Mode, EnableWithoutInit>;

public:

  auto getName() const -> std::string const& { return name;}
  static auto getCtorMessage() -> std::string const& { return ctorMessage;}

private:

  mySingleton() : name("default") { ctorMessage = "called"; }
  mySingleton(std::string n) : name {std::move(n)} { ctorMessage = "called";}

  std::string name;
  inline static std::string ctorMessage;
};

template <bool EnableWithoutInit>
using myLazySingleton = mySingleton<cpptools::singletonMode::lazy, EnableWithoutInit>;

template <bool EnableWithoutInit>
using myEagerSingleton = mySingleton<cpptools::singletonMode::eager, EnableWithoutInit>;


}   // namespace cppttest


TEST(SingletonBase, SingletonProperties)
{
  EXPECT_FALSE((std::is_copy_constructible_v<cppttest::myLazySingleton<false>>));
  EXPECT_FALSE((std::is_move_constructible_v<cppttest::myLazySingleton<false>>));
  EXPECT_FALSE((std::is_copy_assignable_v<cppttest::myLazySingleton<false>>));
  EXPECT_FALSE((std::is_move_assignable_v<cppttest::myLazySingleton<false>>));
}

TEST(LazySingleton, WithInit)
{
  using mySingleton = cppttest::myLazySingleton<false>;
  const auto expected = "this is the name"s;
  auto name = expected;

  mySingleton::initialize(std::move(expected));
  EXPECT_EQ(mySingleton::getCtorMessage(), ""s);

  const auto& myInstance = mySingleton::instance();

  EXPECT_EQ(mySingleton::getCtorMessage(), "called"s);
  EXPECT_EQ(myInstance.getName(), expected);
}

TEST(LazySingleton, EnabledWithoutInitPerformNoInit)
{
  using mySingleton = cppttest::myLazySingleton<true>;

  const auto expected = "default"s;
  auto name = expected;

  EXPECT_EQ(mySingleton::getCtorMessage(), ""s);

  const auto& myInstance = mySingleton::instance();

  EXPECT_EQ(mySingleton::getCtorMessage(), "called"s);
  EXPECT_EQ(myInstance.getName(), expected);
}


TEST(EagerSingleton, WithInit)
{
  using mySingleton = cppttest::myEagerSingleton<false>;

  const auto expected = "this is the name"s;
  const auto expectedDefault = "default"s;
  auto name = expected;

  mySingleton::initialize(std::move(expected));
  EXPECT_EQ(mySingleton::getCtorMessage(), "called"s);

  const auto& myInstance = mySingleton::instance();

  EXPECT_EQ(mySingleton::getCtorMessage(), "called"s);
  EXPECT_EQ(myInstance.getName(), expected);
}

TEST(EagerSingleton, EnabledWithoutInitPerformInit)
{
  using mySingleton = cppttest::myEagerSingleton<true>;

  const auto expected = "default"s;
  auto name = expected;

  mySingleton::initialize();
  EXPECT_EQ(mySingleton::getCtorMessage(), "called"s);

  const auto& myInstance = mySingleton::instance();

  EXPECT_EQ(mySingleton::getCtorMessage(), "called"s);
  EXPECT_EQ(myInstance.getName(), expected);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // SINGLETONBASETEST_H_238709594214091134120947199847731906415849 
