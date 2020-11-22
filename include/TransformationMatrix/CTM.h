/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    CTM.h 
 * @brief   definition of the CTM class.
 * 
 * @author  Lasse Rosenthal 
 * @date    29.09.2020 
 */ 
 
#ifndef CTM_H_22754306632526227748195662444719631464823058 
#define CTM_H_22754306632526227748195662444719631464823058 
 
 
// includes
#include <MultiIndexVector/StackMatrix.h>
#include <MultiIndexVector/StackVector.h>

 
namespace cpptools {


/** 
 * @class CTM
 * @brief CTM models the current transformation matrix.
 */
template <typename T = double>
class CTM {

  // ---------------------------------------------------
  // for simplicity all values smaller than epsilon are
  // considered to be equal to zero.
  static constexpr T epsilon = 1.0E-6;

  // ---------------------------------------------------
  // constants for pi and pi half
  static constexpr double pi = 3.14159265358979323846;
  static constexpr double pi2 = 1.57079632679489661923;

public:

  // ---------------------------------------------------
  // public types
  using value_type = T;
  using matrix = multiindex::SquareStackMatrix<value_type, multiindex::storageOrdering::columnMajor, 2ULL>;
  using vector = multiindex::StackVector<value_type, multiindex::storageOrdering::rowMajor, 2ULL>;

  // ---------------------------------------------------
  // ctor & dtor
  constexpr CTM (matrix const& transform = {1.0, 0.0, 0.0, 1.0},
                 vector const& translation = {0.0, 0.0});

  // ---------------------------------------------------
  // public api
  auto translation            () const noexcept -> vector const&;
  auto transformationMatrix   () const noexcept -> matrix const&;
  void setTranslation         (vector const& t);
  void setTransMatrix         (matrix const& m);
  void setRotation            (value_type angle);
  constexpr auto isInvertible () const noexcept -> bool;
  auto isOrthogonal           () const -> bool;
  void scale                  (value_type scaleX, value_type scaleY) noexcept;
  auto operator*=             (CTM const& src) -> CTM&;
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto put                    (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;


  // ---------------------------------------------------
  // friends
  template <typename S>
  friend bool operator== (CTM<S> const&, CTM<S> const&);
  template <typename S>
  friend auto operator*  (typename CTM<S>::vector const& x, CTM<S> const& ctm) -> typename CTM<S>::vector;
  template <typename S>
  friend auto operator*  (CTM<S> const& ctm1, CTM<S> const& ctm2) -> CTM<S>;

private:

  // ---------------------------------------------------
  // private types
  matrix A {1.0, 0.0, 0.0, 1.0};  ///< the transformation matrix
  vector t {0.0, 0.0};            ///< the translation vector
};


/**
 * @brief Constructor. Initializes the translation vector and the transformation matrix.
 */
template <typename T>
constexpr CTM<T>::CTM(matrix const& transform, vector const& translation)
  : A {transform}
  , t {translation}
{}

/**
 * @brief Returns a const reference to the translation vector.
 */
template <typename T>
inline auto CTM<T>::translation() const noexcept -> vector const&
{
  return t;
}

/**
 * @brief Returns a const reference to the transformation matrix.
 */
template <typename T>
inline auto CTM<T>::transformationMatrix() const noexcept -> matrix const&
{
  return A;
}

/**
 * @brief Sets the translation vector.
 * @param t the new translation vector.
 */
template <typename T>
inline void CTM<T>::setTranslation(vector const& tNew)
{
  t = tNew;
}

/**
 * @brief Sets the transformation matrix.
 * @param m the new transformation matrix.
 */
template <typename T>
inline void CTM<T>::setTransMatrix(matrix const& Anew)
{
  A = Anew;
}

/**
 * @brief  Sets the transformation matrix to a rotation with a given angle.
 * @param  angle the angle of the rotation matrix.
 * @remark the angle is given in radians.
 */
template <typename T>
inline void CTM<T>::setRotation(value_type alpha)
{
  const auto cosAlpha = std::cos(alpha);
  const auto sinAlpha = std::sin(alpha);
  A(0ULL, 0ULL) = cosAlpha;
  A(1ULL, 0ULL) = -sinAlpha;
  A(0ULL, 1ULL) = sinAlpha;
  A(1ULL, 1ULL) = cosAlpha;
}

/**
 * @brief Streams the CTM into an ostream.
 * @param ostr the stream the CTM is streamed in.
 */
template <typename T>
template <typename CharT, typename CharTraits>
auto CTM<T>::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
  const auto space    = ostr.widen(' ');
  const auto newLine  = ostr.widen('\n');
  const auto braceL   = ostr.widen('(');
  const auto braceR   = ostr.widen(')');
  const auto comma    = ostr.widen(',');
  constexpr int width = 5;
  const auto oldFlags = ostr.flags();
  ostr.setf(std::ios::fixed);
  ostr.fill(space);
  const auto curPrec = ostr.precision(2);
  for(std::size_t i = 0ULL; i < 2ULL; ++i)
  {
    ostr << braceL << std::setw(width) << A(i, 0ULL) << space << std::setw(width) << A(i, 1ULL) << comma << space
         << std::setw(width) << t(i) << braceR << newLine;
  }

  ostr.flags(oldFlags);
  ostr.precision(curPrec);
  return ostr;
}

/**
 * @brief Stream operator for CTMs.
 */
template <typename T, typename CharT, typename CharTraits>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, CTM<T> const& ctm) -> std::basic_ostream<CharT, CharTraits>&
{
  return ctm.put(ostr);
}

/**
 * @brief Checks if the CTM is invertible.
 */
template <typename T>
constexpr auto CTM<T>::isInvertible() const noexcept -> bool
{
  return A.det() > epsilon;
}

/**
 * @brief Checks if the transformation matrix is orthogonal.
 */
template <typename T>
inline auto CTM<T>::isOrthogonal() const -> bool
{
  return std::abs(orientedAngle(vector{A(0ULL, 0ULL), A(0ULL, 1ULL)}, vector{A(1ULL, 0ULL), A(1ULL, 1ULL)}) - pi2) < epsilon;
}

/**
 * @brief  Multiplies the entries of the transformation matrix with the guven scaling factors.
 * @remark Scaling by scaleX and scaleY is equivalent to pre-multiplying with the scaleX, scaleY scale matrix.
 *         THis means that the transition vector is not affected by this operation.
 */
template <typename T>
inline void CTM<T>::scale(value_type scaleX, value_type scaleY) noexcept
{
  A(0ULL, 0ULL) *= scaleX;
  A(0ULL, 1ULL) *= scaleX;
  A(1ULL, 0ULL) *= scaleY;
  A(1ULL, 1ULL) *= scaleY;
}

/**
 * @brief Combines the CTM with another one.
 * @param ostr the stream the CTM is streamed in.
 * @remark x*(A1, t1)*(A2, t2) = (x*A1 + t1)*(A2, t2) = x*A1*A2 + t1*A2 + t2
 *         --> (A1, t1)*(A2, t2) = (A1*A2, t1*A2 + t2) = (A1*A2, t1*(A2, t2)) 
 */
template <typename T>
auto CTM<T>::operator*=(CTM<T> const& src) -> CTM<T>&
{
  A *= src.A;
  t = t * src;
  return *this;
}

/**
 * @brief  Applies a given transformation to a given vector.
 * @param  x the vector to which the transformation is applied.
 * @param  ctm the actual transformation.
 * @remark the transformation is applied first: x*(A, t) = x*A + t 
 */
template <typename T>
inline auto operator*(typename CTM<T>::vector const& x, CTM<T> const& ctm) -> typename CTM<T>::vector
{
  return x * ctm.A + ctm.t;
}

/**
 * @brief  Combines two transformations into one.
 * @param  ctm1 the transformation on the left side.
 * @param  ctm2 the transformation on the right side.
 * @remark x*(A1, t1)*(A2, t2) = (x*A1 + t1)*(A2, t2) = x*A1*A2 + t1*A2 + t2
 *         --> (A1, t1)*(A2, t2) = (A1*A2, t1*A2 + t2) = (A1*A2, t1*(A2, t2)) 
 */
template <typename T>
inline auto operator*(CTM<T> const& ctm1, CTM<T> const& ctm2) -> CTM<T>
{
  CTM<T> tmp(ctm1);
  return tmp *= ctm2;
//  return CTM<T>{ctm1.A * ctm2.A, ctm1.t * ctm2};
}

/**
 * @brief comparison operators for CTMs.
 */
template <typename T>
inline bool operator==(CTM<T> const& ctm1, CTM<T> const& ctm2)
{
  return ctm1.A == ctm2.A && ctm1.t == ctm2.t;
}

template <typename T>
inline bool operator!=(CTM<T> const& ctm1, CTM<T> const& ctm2)
{
  return !(ctm1 == ctm2);
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // CTM_H_22754306632526227748195662444719631464823058 
