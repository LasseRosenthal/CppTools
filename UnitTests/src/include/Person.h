/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    Person.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    17.03.2021 
 */ 
 
#ifndef PERSON_H_27630100831461223300180741953827566774711772 
#define PERSON_H_27630100831461223300180741953827566774711772 
 
 
// includes
#include <string>
 
namespace test { 


class NoDefault {

  public:

    NoDefault() = delete;
    NoDefault(int vv)
      : v{vv} {};

    int value() const noexcept
    {
      return v;
    }

  private:

    int v;
};

/** 
 * @class Person
 * @brief Testclass
 */
class Person {

public:

  Person() = default;
  Person(Person const&) = default;
  Person(Person&&) = default;
  auto operator=(Person const&) -> Person& = default;
  auto operator=(Person&&) -> Person& = default;
  Person(int a, std::string n);
  ~Person() = default;

  auto getAge() const noexcept -> int { return age;}
  auto getName() const -> std::string const& { return name;}
  void setAge(int a) noexcept { age = a;}
  void setName(std::string n) { name = std::move(n);}

private:

  int age{1};
  std::string name{"Anonymous"};
};


inline Person::Person(int a, std::string n)
  : age  {a}
  , name {std::move(n)}
{}

inline auto operator==(Person const& p1, Person const& p2) noexcept -> bool
{
  return p1.getAge() == p2.getAge();
}

inline auto operator<(Person const& p1, Person const& p2) noexcept -> bool
{
  return p1.getAge() < p2.getAge();
}

inline auto operator<=(Person const& p1, Person const& p2) noexcept -> bool
{
  return p1 == p2 || p1 < p2;
}

inline auto operator>(Person const& p1, Person const& p2) noexcept -> bool
{
  return p2 < p1;
}

inline auto operator>=(Person const& p1, Person const& p2) noexcept -> bool
{
  return p1 == p2 || p1 > p2;
}


}   // namespace test


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // PERSON_H_27630100831461223300180741953827566774711772 
