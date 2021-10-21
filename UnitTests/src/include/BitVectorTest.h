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

TEST(BitVector, ConstructorWithValue)
{
  bws::BitVectorT<std::uint8_t> b1{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  bws::BitVectorT<std::uint8_t> b2(b1.size(), true);

  EXPECT_EQ(b1.capacity(), b2.capacity());
  EXPECT_EQ(b1, b2);
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

TEST(BitVector, Comparison)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1};
  bws::BitVectorT<std::uint16_t> b2{0,0,1,1,0,1};

  EXPECT_EQ(b1, b2);

  b2[0] = true;
  EXPECT_NE(b1, b2);
}

TEST(BitVector, ComparisonAfterReserve)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1};
  bws::BitVectorT<std::uint16_t> b2{0,0,1,1,0,1};

  EXPECT_EQ(b1, b2);
  b2.reserve(64ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, ShrinkToFit)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1};
  bws::BitVectorT<std::uint16_t> b2{0,0,1,1,0,1};

  b1.reserve(164ULL);
  EXPECT_EQ(b1.capacity(), 168ULL);
  EXPECT_EQ(b1, b2);

  b1.shrink_to_fit();
  EXPECT_EQ(b1.capacity(), 8ULL);
  EXPECT_EQ(b1, b2);
}



TEST(BitVector, resizeNewSizeSmaller)
{
  bws::BitVectorT<std::uint8_t> b1{0,0,1,1,0,1,1,1,1,1};
  bws::BitVectorT<std::uint16_t> b2{0,0,1,1,0,1};

  EXPECT_EQ(b1.size(), 10ULL);
  EXPECT_EQ(b2.size(), 6ULL);
  EXPECT_EQ(b1.capacity(), 16ULL);
  EXPECT_NE(b1, b2);

  b1.resize(6ULL);

  EXPECT_EQ(b1.size(), 6ULL);
  EXPECT_EQ(b1.capacity(), 8ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, resizeAddNewElementsNoRealloc)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1,1,1,1,1};
  bws::BitVectorT<std::uint16_t> b2{0,0,1,1,0,1};

  EXPECT_NE(b1, b2);

  b2.resize(10ULL, true);

  EXPECT_EQ(b2.capacity(), 16ULL);
  EXPECT_EQ(b2.size(), 10ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, resizeAddNewElementsAndRealloc)
{
  bws::BitVectorT<std::uint8_t> b1{0,0,1,1,0,1,1,1,1,1};
  bws::BitVectorT<std::uint8_t> b2{0,0,1,1,0,1};

  EXPECT_NE(b1, b2);

  b2.resize(10ULL, true);

  EXPECT_EQ(b2.capacity(), 16ULL);
  EXPECT_EQ(b2.size(), 10ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, resizeAddNewElementsReallocAndMemset)
{
  constexpr std::size_t size = 1465;
  bws::BitVectorT<std::uint8_t> b1(size, true);
  bws::BitVectorT<std::uint64_t> b2{1,1,1,1};

  EXPECT_NE(b1, b2);

  b2.resize(b1.size(), true);

  EXPECT_EQ(b2.capacity(), cpptools::alignUp(b1.size(), 64ULL));
  EXPECT_EQ(b2.size(), size);
  EXPECT_EQ(b1, b2);

  std::size_t ctr = 0ULL;
  for(const auto b : b2)
  {
    EXPECT_TRUE(b);
    ++ctr;
  }

  EXPECT_EQ(ctr, size);
}

TEST(BitVector, resizeExceedMinCapacity)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1,1,1,1,1};
  bws::BitVectorT<std::uint8_t> b2{0,0,1,1,0,1};

  EXPECT_EQ(b1.size(), 10ULL);
  EXPECT_EQ(b2.size(), 6ULL);
  EXPECT_EQ(b2.capacity(), 8ULL);
  EXPECT_NE(b1, b2);

  b2.resize(10ULL, true);

  EXPECT_EQ(b2.capacity(), 16ULL);
  EXPECT_EQ(b2.size(), 10ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, resizeRemovingElements)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1,1,1,1,1};
  bws::BitVectorT<std::uint8_t> b2{0,0,1,1,0,1};

  EXPECT_EQ(b1.size(), 10ULL);
  EXPECT_EQ(b1.capacity(), 16ULL);
  EXPECT_EQ(b2.size(), 6ULL);
  EXPECT_EQ(b2.capacity(), 8ULL);
  EXPECT_NE(b1, b2);

  b1.resize(6ULL);

  EXPECT_EQ(b1.capacity(), 8ULL);
  EXPECT_EQ(b1.size(), 6ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, assignNewSizeSmaller)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1,1,1,1,1};
  bws::BitVectorT<type> b2{1,1,1,1};

  b1.assign(4, true);
  EXPECT_EQ(b1.size(), 4ULL);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, assignNewSizeSmallerMoreThanOneRegion)
{
  using type = std::uint16_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
  bws::BitVectorT<type> b2{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

  b1.assign(b2.size(), true);
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, assignNewSizeGreater)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b2{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  bws::BitVectorT<type> b1{1,1,1,1};

  b1.assign(b2.size(), false);
  EXPECT_EQ(b1.size(), b2.size());
  EXPECT_EQ(b1, b2);
}

TEST(BitVector, MoveConstructor)
{
  using type = std::uint8_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1};

  EXPECT_EQ(b1.size(), 6ULL);
  EXPECT_EQ(b1.capacity(), 8ULL);

  bws::BitVectorT<type> b2(std::move(b1));

  EXPECT_EQ(b1.size(), 0ULL);
  EXPECT_TRUE(b1.empty());
  EXPECT_EQ(b1.capacity(), 0ULL);
  EXPECT_EQ(b2.size(), 6ULL);
  EXPECT_EQ(b2.capacity(), 8ULL);

  EXPECT_FALSE(b2[0]);
  EXPECT_FALSE(b2[1]);
  EXPECT_TRUE(b2[2]);
  EXPECT_TRUE(b2[3]);
  EXPECT_FALSE(b2[4]);
  EXPECT_TRUE(b2[5]);
}

TEST(BitVector, MoveAssignment)
{
  using type = std::uint32_t;
  using myBitVector = bws::BitVectorT<type>;

  bws::BitVectorT<type> b1{0,0,1,1,0,1};

  EXPECT_EQ(b1.size(), 6ULL);
  EXPECT_EQ(b1.capacity(), 32ULL);

  bws::BitVectorT<type> b2;
  EXPECT_EQ(b2.size(), 0ULL);
  EXPECT_TRUE(b2.empty());
  EXPECT_EQ(b2.capacity(), 0ULL);

  b2 = std::move(b1);

  EXPECT_EQ(b1.size(), 0ULL);
  EXPECT_TRUE(b1.empty());
  EXPECT_EQ(b1.capacity(), 0ULL);
  EXPECT_EQ(b2.size(), 6ULL);
  EXPECT_EQ(b2.capacity(), 32ULL);

  EXPECT_FALSE(b2[0]);
  EXPECT_FALSE(b2[1]);
  EXPECT_TRUE(b2[2]);
  EXPECT_TRUE(b2[3]);
  EXPECT_FALSE(b2[4]);
  EXPECT_TRUE(b2[5]);
}

TEST(BitVector, iteratorDecrement)
{
  using type = std::uint16_t;
//  type data[2]{ 0b1101'0101, 0b1100'0011 };
  type data[1]{ 0b11000011'11010101 };

  using myBitVector = bws::BitVectorT<std::uint32_t>;
  myBitVector bitVec(data);
  auto it = bitVec.end();

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

TEST(BitVector, copyConstructor)
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

  myBitVector b2(b);
  std::size_t ctr = 0ULL;
  for(auto const& bVal : b2)
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

  b[0] = true;

  auto it = b.begin();
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
  EXPECT_TRUE(static_cast<bool>(*it));
  ++it;

  EXPECT_EQ(it, b.end());
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
  EXPECT_EQ(it, b.end());
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
