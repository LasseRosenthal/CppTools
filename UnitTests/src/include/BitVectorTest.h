/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    BitVectorTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    14.06.2021 
 */ 
 
#ifndef BITVECTORTEST_H_154721632517873558130392666698291257025402 
#define BITVECTORTEST_H_154721632517873558130392666698291257025402 
 
 
// includes
#include <Bitwise/BitVector.h>

#include <random>
#include <vector>
 
using BitVector8 = bws::BitVectorT<std::uint8_t>;
using BitVector16 = bws::BitVectorT<std::uint16_t>;


TEST(BitVector, DefaultConstructorExpectNoThrow)
{
  EXPECT_NO_THROW(
    BitVector8 bitVec;
  );
}

TEST(BitVector, DefaultConstructorSizeZero)
{
  BitVector8 bitVec;
  EXPECT_EQ(bitVec.size(), 0ULL);
}

TEST(BitVector, ConstructorSizeAndCapacity)
{
  constexpr std::size_t size = 23ULL;
  constexpr std::size_t align = 24ULL;
  BitVector8 bitVec(size);
  EXPECT_EQ(bitVec.size(), size);
  EXPECT_EQ(bitVec.capacity(), align);
}

TEST(BitVector, CapacityAfterReserve)
{
  constexpr std::size_t size = 23ULL;
  constexpr std::size_t align = 24ULL;
  BitVector8 bitVec(size);
  EXPECT_EQ(bitVec.size(), size);
  EXPECT_EQ(bitVec.capacity(), align);

  bitVec.reserve(37);
  EXPECT_EQ(bitVec.size(), size);
  EXPECT_EQ(bitVec.capacity(), 40);
}

TEST(BitVector, Iterator)
{
  std::uint8_t data[2]{ 0b11010101, 0b11000011 };
  BitVector8::iterator it(data, 10ULL, 16ULL);
  EXPECT_FALSE(static_cast<bool>(*it));
}

TEST(BitVector, isAtEndExpectFalse)
{
  std::uint8_t data[2]{ 0b11010101, 0b11000011 };
  BitVector8::iterator it(data, 10ULL, 16ULL);
  EXPECT_FALSE(it.isAtEnd());
}

TEST(BitVector, isAtEndExpectTrue)
{
  std::uint8_t data[2]{ 0b11010101, 0b11000011 };
  BitVector8::iterator it(data, 16ULL, 16ULL);
  EXPECT_TRUE(it.isAtEnd());
}

TEST(BitVector, iteratorIncrement)
{
  std::uint8_t data[2]{ 0b11010101, 0b11000011 };
  BitVector8::iterator it(data, 0ULL, 16ULL);

  EXPECT_TRUE(static_cast<bool>(*it));
  it += 1;
  EXPECT_FALSE(static_cast<bool>(*it));
  it += 1;
  EXPECT_TRUE(static_cast<bool>(*it));
  it += 1;
  EXPECT_FALSE(static_cast<bool>(*it));
  it += 1;
  EXPECT_TRUE(static_cast<bool>(*it));
  it += 1;
  EXPECT_FALSE(static_cast<bool>(*it));
  it += 1;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  it += 7;
  EXPECT_TRUE(static_cast<bool>(*it));
  it += 1;
  EXPECT_TRUE(static_cast<bool>(*it));
  it += 1;
  EXPECT_TRUE(it.isAtEnd());
}

TEST(BitVector, iteratorDecrement)
{
  std::uint8_t data[2]{ 0b11010101, 0b11000011 };
  BitVector8::iterator it(data, 16ULL, 16ULL);

  EXPECT_TRUE(it.isAtEnd());

  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
  --it;
  EXPECT_FALSE(static_cast<bool>(*it));
  --it;
  EXPECT_TRUE(static_cast<bool>(*it));
}

TEST(BitVector, indexAccess)
{
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(0, 1);

  using type = std::uint64_t;
  using myBitVector = bws::BitVectorT<type>;
  constexpr std::size_t size = 12151;

  myBitVector b(size);
  std::vector<bool> vb(size);

  EXPECT_EQ(b.size(), size);

  for(std::size_t i{}; i < size; ++i)
  {
    const bool val = static_cast<bool>(distrib(gen));
    b[i] = val;
    vb[i] = val;
  }

  std::size_t ctr = 0ULL;
  for(auto const& bVal : b)
  {
    EXPECT_EQ(static_cast<bool>(bVal), vb[ctr]);
    ++ctr;
  }
}

TEST(BitVector, initializerListConstructor)
{
  using type = std::uint16_t;
  using myBitVector = bws::BitVectorT<type>;
  myBitVector b{0,0,1,1,0,1};

  auto it = b.begin();
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;

  EXPECT_TRUE(it.isAtEnd());
}

TEST(BitVector, pushBackWithReserve)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;
  myBitVector b{0,0,1,1,0,1};

  b.push_back(0);
  b.push_back(1);
  b.push_back(1);
  b.push_back(0);

  auto it = b.begin();
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;
  EXPECT_FALSE(static_cast<bool>(*it));
  ++it;
  EXPECT_TRUE(it.isAtEnd());
}

TEST(BitVector, popBack)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;
  myBitVector b{0,0,1,1,0,1};

  EXPECT_EQ(b.size(), 6ULL);

  EXPECT_TRUE(b.back());
  b.pop_back();
  EXPECT_EQ(b.size(), 5ULL);

  EXPECT_FALSE(b.back());
  b.pop_back();
  EXPECT_EQ(b.size(), 4ULL);

  EXPECT_TRUE(b.back());
  b.pop_back();
  EXPECT_EQ(b.size(), 3ULL);

  EXPECT_TRUE(b.back());
  b.pop_back();
  EXPECT_EQ(b.size(), 2ULL);

  EXPECT_FALSE(b.back());
  b.pop_back();
  EXPECT_EQ(b.size(), 1ULL);

  EXPECT_FALSE(b.back());
  b.pop_back();
  EXPECT_TRUE(b.empty());
}

TEST(BitVector, accessOutOfBounds)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;
  myBitVector b{ 0,0,1,1,0,1 };

  EXPECT_THROW(
    const auto v = b.at(6ULL),
    std::out_of_range
  );
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // BITVECTORTEST_H_154721632517873558130392666698291257025402 
