/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FactoryTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    09.08.2021 
 */ 
 
#ifndef FACTORYTEST_H_3067425488898218233127651208830724125362623 
#define FACTORYTEST_H_3067425488898218233127651208830724125362623 
 
 
// includes
#include <Patterns/Factory.h>

#include <functional>
 
class Base {
public:
  virtual ~Base() = default;
  using BaseFactory = patterns::Factory<Base, int>;
};

class DerivedFromBase : public Base {
public:

  static auto create() ->std::unique_ptr<Base>{
    return std::unique_ptr<DerivedFromBase>(new DerivedFromBase);
  }
  inline static const bool isRegistered = BaseFactory::registerBuilder(1, DerivedFromBase::create);
};


class Shape {

public:

  enum class id {
    one,
    two
  };

  virtual auto getId() const noexcept -> id = 0;
  virtual ~Shape() = default;

  using ShapeFactory = patterns::Factory<Shape, Shape::id,
  std::unique_ptr<Shape>(*)(int, std::string)>;
};

class Rectangle1 : public Shape {

public:
  
  static auto create(int i, std::string name) ->std::unique_ptr<Shape>{
    return std::unique_ptr<Rectangle1>(new Rectangle1(i, name));
  }

  ~Rectangle1() = default;
  auto getId() const noexcept -> id override
  {
    return Shape::id::one;
  }

  inline static const bool isRegistered = ShapeFactory::registerBuilder(Shape::id::one, Rectangle1::create);

private:

  Rectangle1(int i, std::string name) : i{i} {}
  int i;
};

class Rectangle2 : public Shape {

public:

  ~Rectangle2() = default;

  static auto create(int i, std::string name) ->std::unique_ptr<Shape>{
    return std::unique_ptr<Rectangle2>(new Rectangle2(i, name));
  }

  auto getId() const noexcept -> id override
  {
    return Shape::id::two;
  }

  inline static const bool isRegistered = ShapeFactory::registerBuilder(Shape::id::two, Rectangle2::create);

private:

  Rectangle2(int i, std::string name) : i{i} {}
  int i;
};

TEST(ShapeFactory, isRegistered)
{
  auto rec1 = Rectangle1::create(1, "Rec1"s);
  auto rec2 = Rectangle2::create(1, "Rec2"s);

  EXPECT_TRUE(Rectangle1::isRegistered);
  EXPECT_TRUE(Rectangle2::isRegistered);
}
 
TEST(BaseFactoryNoArguments, creation)
{
  Base::BaseFactory f;
  auto d = f.create(1);
}

TEST(ShapeFactoryWithArguments, creation)
{
  Shape::ShapeFactory fac;

  auto rec1 = fac.create(Shape::id::one, 1, "Rec1"s);
  auto rec2 = fac.create(Shape::id::two, 2, "Rec2"s);

  EXPECT_EQ(rec1->getId(), Shape::id::one);
  EXPECT_EQ(rec2->getId(), Shape::id::two);
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FACTORYTEST_H_3067425488898218233127651208830724125362623 
