/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    CountedObjectTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    06.08.2021 
 */ 
 
#ifndef COUNTEDOBJECTTEST_H_25074302692293429449121999372133771151312874 
#define COUNTEDOBJECTTEST_H_25074302692293429449121999372133771151312874 
 
 
// includes
#include <Utils/CountedObject.h>
#include <vector>
 

struct myCountedClass : public cpptools::CountedObject<myCountedClass> {};

 
TEST(CountedObject, zeroObjects)
{
  EXPECT_EQ(myCountedClass::count(), 0ULL);
}

TEST(CountedObject, OneObjectAfterConstructor)
{
  myCountedClass c;
  EXPECT_EQ(myCountedClass::count(), 1ULL);
}

TEST(CountedObject, ObjectsInVector)
{
  constexpr std::size_t n = 134ULL;
  std::vector<myCountedClass> v(n);
  EXPECT_EQ(myCountedClass::count(), n);
}

TEST(CountedObject, Destructor)
{
  {
    constexpr std::size_t n = 134ULL;
    std::vector<myCountedClass> v(n);
    EXPECT_EQ(myCountedClass::count(), n);

    for(std::size_t i = 1ULL; i <= n;  ++i)
    {
      v.pop_back();
      EXPECT_EQ(myCountedClass::count(), n - i);
    }
  }

  EXPECT_EQ(myCountedClass::count(), 0ULL);
}

TEST(CountedObject, copyConstructor)
{
  myCountedClass c;
  EXPECT_EQ(myCountedClass::count(), 1ULL);
  myCountedClass c2(c);
  EXPECT_EQ(myCountedClass::count(), 2ULL);
  myCountedClass c3(c2);
  EXPECT_EQ(myCountedClass::count(), 3ULL);
}

TEST(CountedObject, copyAssignment)
{
  myCountedClass c;
  myCountedClass c2(c);
  EXPECT_EQ(myCountedClass::count(), 2ULL);
  myCountedClass c3(c2);
  EXPECT_EQ(myCountedClass::count(), 3ULL);
  c3 = c;
  EXPECT_EQ(myCountedClass::count(), 3ULL);
}

TEST(CountedObject, moveAssignment)
{
  myCountedClass c;
  myCountedClass c2(c);
  EXPECT_EQ(myCountedClass::count(), 2ULL);
  myCountedClass c3(c2);
  EXPECT_EQ(myCountedClass::count(), 3ULL);
  c3 = std::move(c);
  EXPECT_EQ(myCountedClass::count(), 3ULL);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // COUNTEDOBJECTTEST_H_25074302692293429449121999372133771151312874 
