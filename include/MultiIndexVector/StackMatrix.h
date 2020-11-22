/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StackMatrix.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.09.2020 
 */ 
 
#ifndef STACKMATRIX_H_114744425239817695103782923520911300227696 
#define STACKMATRIX_H_114744425239817695103782923520911300227696 
 
 
// includes 
#include <MultiIndexVector/MultiIndexArray.h>

#include <algorithm>
#include <execution>
#include <utility>


namespace multiindex {


/**
 * @class StackMatrix
 * @brief
 */
template <typename T, storageOrdering Ordering, std::size_t M, std::size_t N,
          typename Enable = std::enable_if_t<std::is_arithmetic_v<T>>>
class StackMatrix : public ArrayMatrix<T, Ordering, M, N> {

  using Base = ArrayMatrix<T, Ordering, M, N>;

public:

  // ---------------------------------------------------
  // public types
  using value_type      = typename Base::value_type;
  using reference       = typename Base::reference;
  using const_reference = typename Base::const_reference;
  using pointer         = typename Base::pointer;
  using const_pointer   = typename Base::const_pointer;
  using size_type       = typename Base::size_type;
  using iterator        = pointer;
  using const_iterator  = const_pointer;

  static constexpr bool isSquareMatrix = M == N;

  // ---------------------------------------------------
  // inherit the constructors from Base
  using Base::Base;

  // ---------------------------------------------------
  // public api
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto           put (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;
  template <typename Enable = std::enable_if_t<isSquareMatrix && M == 2ULL>>
  constexpr auto det () const noexcept -> value_type;
  template <typename Enable = std::enable_if_t<isSquareMatrix>>
  auto operator*=    (StackMatrix<T, Ordering, N, N> const& src) -> StackMatrix&;
};


/**
 * @brief convenient alias for square matrices.
 */
template <typename T, storageOrdering Ordering, std::size_t N>
using SquareStackMatrix = StackMatrix<T, Ordering, N, N>;


/**
 * @brief returns the determinant of the matrix.
 * @remark only enabled for 2 x 2 matrices.
 */
template <typename T, storageOrdering Ordering, std::size_t M, std::size_t N, typename Enable>
template <typename>
constexpr auto StackMatrix<T, Ordering, M, N, Enable>::det() const noexcept -> value_type
{
  return Base::values[0ULL] * Base::values[3ULL] - Base::values[1ULL] * Base::values[2ULL];
}

/**
 * @brief Multiplies the matrix with another matrix.
 */
template <typename T, storageOrdering Ordering, std::size_t M, std::size_t N, typename Enable>
template <typename>
inline auto StackMatrix<T, Ordering, M, N, Enable>::operator*= (StackMatrix<T, Ordering, N, N> const& src) -> StackMatrix&
{
  mmProdParColMajInPlace(this->data(), src.data(), N, N);
  return *this;
}

/**
 * @brief streams the matrix to an ostream object.
 */
template <typename T, storageOrdering Ordering, std::size_t M, std::size_t N, typename Enable>
template <typename CharT, typename CharTraits>
auto StackMatrix<T, Ordering, M, N, Enable>::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
  const auto space    = ostr.widen(' ');
  const auto newLine  = ostr.widen('\n');
  const auto oldFlags = ostr.flags();
  ostr.setf(std::ios::fixed);
  ostr.fill(space);
  const auto curPrec = ostr.precision(2);
  for(size_type i = 0ULL; i < M; ++i)
  {
    for(size_type j = 0ULL; j < N; ++j)
    {
      ostr << std::setw(6) << (*this)(i, j) << space;
    }
    ostr << newLine;
  }

  ostr.flags(oldFlags);
  ostr.precision(curPrec);
  return ostr;
}

/**
 * @brief Returns the inverse of a 2 x 2 matrix.
 */
template <typename T, storageOrdering Ordering, std::size_t N, typename = std::enable_if_t<N == 2ULL>>
auto invert(SquareStackMatrix<T, Ordering, N> const& mat) -> SquareStackMatrix<std::common_type_t<T, double>, Ordering, N>
{
  using ReturnType = SquareStackMatrix<std::common_type_t<T, double>, Ordering, N>;
  if constexpr(Ordering == storageOrdering::columnMajor)
  {
    return (1.0 / mat.det()) * ReturnType{mat(1ULL, 1ULL), -mat(1ULL, 0ULL),
                                         -mat(0ULL, 1ULL), mat(0ULL, 0ULL)};
  }
  if constexpr(Ordering == storageOrdering::rowMajor)
  {
    return (1.0 / mat.det()) * ReturnType{mat(1ULL, 1ULL), -mat(0ULL, 1ULL),
                                         -mat(1ULL, 0ULL), mat(0ULL, 0ULL)};
  }
}

/**
 * @brief stream operator.
 */
template <typename CharT, typename CharTraits, typename T, storageOrdering Ordering, std::size_t M, std::size_t N>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, StackMatrix<T, Ordering, M, N> const& mat) 
 -> std::basic_ostream<CharT, CharTraits>&
{
  return mat.put(ostr);
}


}   // namespace multiindex

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STACKMATRIX_H_114744425239817695103782923520911300227696 
