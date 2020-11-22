/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    DummyClass.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.09.2020 
 */ 
 
#ifndef DUMMYCLASS_H_2391226468236369675281004395152801072221048 
#define DUMMYCLASS_H_2391226468236369675281004395152801072221048 
 
 
// includes 
 
 
/** 
 * @class Person
 * @brief 
 */
struct Person {

  auto getAge  () const noexcept { return age;}
  auto getSize () const noexcept { return size;}
  void setAge  (int a) noexcept { age = a;}
  void setSize (int s) noexcept { size = s;}

private:

  int age{};
  int size{};
};
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // DUMMYCLASS_H_2391226468236369675281004395152801072221048 
