/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    MultiIndexArray.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    25.08.2020 
 */ 
 
#ifndef MULTIINDEXARRAY_H_1774211191304802872220982451316113153173835 
#define MULTIINDEXARRAY_H_1774211191304802872220982451316113153173835 
 
 
// includes
#include <MultiIndexVector/ArrayOperations.h>
#include <MultiIndexVector/MultiArrayAccessor.h>

#include <iomanip>
#include <ostream>


namespace multiindex {


/**
 * @class MultiIndexArray
 * @brief MultiIndexArray provides multi-dimensional access to a static linear array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
class MultiIndexArray {

  using ArrayAccessor = MultiArrayAccessor<T[(... * Dimensions)], sizeof...(Dimensions), Ordering>;

  template <typename S>
  static constexpr bool isSuitableForBitOperations = std::is_trivial_v<S> || std::is_standard_layout_v<S>;

  template <typename S>
  using requiresNonPOD = std::enable_if_t<!isSuitableForBitOperations<S> && !std::is_arithmetic_v<S>>;

  template <typename S>
  using requiresPOD = std::enable_if_t<isSuitableForBitOperations<S>>;

  template <typename S>
  using requiresPODClassType = std::enable_if_t<isSuitableForBitOperations<S> && !std::is_arithmetic_v<S>>;

  template <typename S>
  using requiresArithmetic = std::enable_if_t<std::is_arithmetic_v<S>>;

public:

  // ---------------------------------------------------
  // public types
  using value_type      = typename ArrayAccessor::value_type;
  using reference       = typename ArrayAccessor::reference;
  using const_reference = typename ArrayAccessor::const_reference;
  using pointer         = typename ArrayAccessor::pointer;
  using const_pointer   = typename ArrayAccessor::const_pointer;
  using iterator        = typename ArrayAccessor::iterator;
  using const_iterator  = typename ArrayAccessor::const_iterator;
  using size_type       = typename ArrayAccessor::size_type;

  // ---------------------------------------------------
  // public constants
  static constexpr std::size_t     rank      {sizeof...(Dimensions)};
  static constexpr std::size_t     totalSize {(... * Dimensions)};
  static constexpr storageOrdering ordering  {Ordering};

  // ---------------------------------------------------
  // ctor & dtor and assignment
  constexpr MultiIndexArray (value_type const& val = value_type{});
  template <typename... Vs, typename = std::enable_if_t<meta::AllConvertible<value_type, Vs...>>>
  constexpr MultiIndexArray (Vs&&... vals);
  constexpr MultiIndexArray (MultiIndexArray const& src);
  constexpr MultiIndexArray (MultiIndexArray&& src);
  auto operator=            (MultiIndexArray const& src) -> MultiIndexArray&;
  auto operator=            (MultiIndexArray&& src) -> MultiIndexArray&;
  template <typename S, typename Enable = std::enable_if_t<std::is_convertible_v<S, T>>>
  constexpr MultiIndexArray (MultiIndexArray<S, Ordering, Dimensions...> const& src);
  ~MultiIndexArray          () = default;

  // ---------------------------------------------------
  // public api
  [[nodiscard]] constexpr auto size  () const noexcept -> size_type;
  [[nodiscard]] constexpr auto size  (std::size_t i) const -> size_type;
  [[nodiscard]] constexpr auto empty () const noexcept -> bool;
  constexpr auto begin               () noexcept -> iterator;
  constexpr auto end                 () noexcept -> iterator;
  constexpr auto begin               () const noexcept -> const_iterator;
  constexpr auto end                 () const noexcept -> const_iterator;
  constexpr auto cbegin              () const noexcept -> const_iterator;
  constexpr auto cend                () const noexcept -> const_iterator;
  template <typename... Is>
  constexpr auto operator()          (Is... is) -> reference;
  template <typename... Is>
  constexpr auto operator()          (Is... is) const -> const_reference;
  constexpr auto data                () -> pointer;
  constexpr auto data                () const -> const_pointer;
  template <typename = requiresArithmetic<T>>
  constexpr auto equals              (MultiIndexArray const& src, int = 0, int = 0) const noexcept -> bool;
  template <typename Enable = requiresPODClassType<T>>
  auto equals                        (MultiIndexArray const& src) const -> bool;
  template <typename Enable = requiresNonPOD<T>>
  auto equals                        (MultiIndexArray const& src, int = 0) const -> bool;

  // ---------------------------------------------------
  // arithmetic operations
  void operator*=                    (value_type const& factor);
  void operator/=                    (value_type const& divisor);
  void operator+=                    (MultiIndexArray const& src);
  template <typename S, typename Enable = std::enable_if_t<std::is_convertible_v<S, T>>>
  void operator+=                    (MultiIndexArray<S, Ordering, Dimensions...> const& src);
  void operator-=                    (MultiIndexArray const& src);
  template <typename S, typename Enable = std::enable_if_t<std::is_convertible_v<S, T>>>
  void operator-=                    (MultiIndexArray<S, Ordering, Dimensions...> const& src);
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto put                           (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;

  // ---------------------------------------------------
  // friendships
  template <typename, storageOrdering, std::size_t...>
  friend class MultiIndexArray;

protected:

  // ---------------------------------------------------
  // protected data
  value_type      values[totalSize];
  const size_type dimensions[rank];
  ArrayAccessor   arrayAccessor;

private:

  // ---------------------------------------------------
  // private methods
  template <typename Enable = requiresPOD<T>>
  void copyFrom (MultiIndexArray const& src);
  template <typename Enable = requiresNonPOD<T>>
  void copyFrom (MultiIndexArray const& src, int = 0);
};


/**
 * @brief Convenient alias for matrices.
 */
template <typename T, storageOrdering Ordering, std::size_t M, std::size_t N>
using ArrayMatrix = MultiIndexArray<T, Ordering, M, N>;


/**
 * @brief constructor. Construct an array and fills it with a given value.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr MultiIndexArray<T, Ordering, Dimensions...>::MultiIndexArray(value_type const& val)
  : values        {value_type{}}
  , dimensions    {Dimensions...}
  , arrayAccessor {dimensions, values}
{
  for(auto& value : values)
  {
    value = val;
  }
}

/**
 * @brief constructor. Construct an array with given values.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename... Vs, typename>
constexpr MultiIndexArray<T, Ordering, Dimensions...>::MultiIndexArray(Vs&&... vals)
  : values        {std::forward<Vs>(vals)...}
  , dimensions    {Dimensions...}
  , arrayAccessor {dimensions, values}
{}

/**
 * @brief copy constructor.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr MultiIndexArray<T, Ordering, Dimensions...>::MultiIndexArray(MultiIndexArray const& src)
  : dimensions    {Dimensions...}
  , arrayAccessor {dimensions, values}
{
  for(std::size_t i{}; i < totalSize; ++i)
  {
    values[i] = src.values[i];
  }
}

/**
 * @brief move constructor.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr MultiIndexArray<T, Ordering, Dimensions...>::MultiIndexArray(MultiIndexArray&& src)
  : MultiIndexArray(src)
{}

/**
 * @brief templated copy constructor. Construct an array from an array of a different type
 *        The type of the source array has to be convertible to the type of the destination array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename S, typename>
constexpr MultiIndexArray<T, Ordering, Dimensions...>::MultiIndexArray(MultiIndexArray<S, Ordering, Dimensions...> const& src)
  : dimensions    {Dimensions...}
  , arrayAccessor {dimensions, values}
{
  for(std::size_t i{}; i < totalSize; ++i)
  {
    values[i] = src.values[i];
  }
}

/**
 * @brief Assignment operator. Replaces the contents of the array with the content of a given array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto MultiIndexArray<T, Ordering, Dimensions...>::operator=(MultiIndexArray const& src) -> MultiIndexArray&
{
  if(this != &src)
  {
    copyFrom(src);
  }
  return *this;
}

/**
 * @brief move assignment operator. Replaces the contents of the array with the content of a given array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto MultiIndexArray<T, Ordering, Dimensions...>::operator=(MultiIndexArray&& src) -> MultiIndexArray&
{
  copyFrom(src);
  return *this;
}

/**
 * @brief Returns the total size of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::size() const noexcept -> size_type
{
  return totalSize;
}

/**
 * @brief Returns the size of the i-th dimension of the array.
 * @param i the zero based index of the dimension.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::size(std::size_t i) const -> size_type
{
  return dimensions[i];
}

/**
 * @brief Checks if the array is empty, i.e if its size equals zero.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::empty() const noexcept -> bool
{
  return totalSize == 0ULL;
}

/**
 * @brief Returns an iterator to the first element of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::begin() noexcept -> iterator
{
  return std::begin(values);
}

/**
 * @brief Returns an iterator to the first element following the last element of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::end() noexcept -> iterator
{
  return std::end(values);
}

/**
 * @brief Returns a const_iterator to the first element of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::begin() const noexcept -> const_iterator
{
  return std::begin(values);
}

/**
 * @brief Returns a const_iterator to the first element following the last element of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::end() const noexcept -> const_iterator
{
  return std::end(values);
}

/**
 * @brief Returns a const_iterator to the first element of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::cbegin() const noexcept -> const_iterator
{
  return std::cbegin(values);
}

/**
 * @brief Returns a const_iterator to the first element following the last element of the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::cend() const noexcept -> const_iterator
{
  return std::cend(values);
}

/**
 * @brief Index operator. Returns a reference to the element spcified by the given indices.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename... Is>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::operator()(Is... is) -> reference
{
  return arrayAccessor(is...);
}

/**
 * @brief Index operator. Returns a const reference to the element spcified by the given indices.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename... Is>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::operator()(Is... is) const -> const_reference
{
  return arrayAccessor(is...);
}

/**
 * @brief Returns a raw pointer to the underlying array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::data() -> pointer
{
  return values;
}

/**
 * @brief Returns a const raw pointer to the underlying array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::data() const -> const_pointer
{
  return values;
}


template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename>
constexpr auto MultiIndexArray<T, Ordering, Dimensions...>::equals(MultiIndexArray const& src, int, int) const noexcept -> bool
{
  return cpptools::compareArray(values, src.values);
}

/**
 * @brief performs a bitwise comparison of the arrays.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename>
inline auto MultiIndexArray<T, Ordering, Dimensions...>::equals(MultiIndexArray const& src) const -> bool
{
  return std::memcmp(values, src.values, totalSize * sizeof(T)) == 0;
}

/**
 * @brief performs an element wise comparison of the arrays.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename>
inline auto MultiIndexArray<T, Ordering, Dimensions...>::equals(MultiIndexArray const& src, int) const -> bool
{
  return std::equal(cbegin(), cend(), src.cbegin());
}

/**
 * @brief Assignment operator. Replaces the contents of the array with the content of a given array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename>
inline void MultiIndexArray<T, Ordering, Dimensions...>::copyFrom(MultiIndexArray const& src)
{
  std::memcpy(values, src.values, totalSize * sizeof(T));
}

/**
 * @brief Assignment operator. Replaces the contents of the array with the content of a given array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename>
inline void MultiIndexArray<T, Ordering, Dimensions...>::copyFrom(MultiIndexArray const& src, int)
{
  std::copy(/*std::execution::par, */src.begin(), src.end(), begin());
}

/**
 * @brief Applies compound multiplication to each element in the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline void MultiIndexArray<T, Ordering, Dimensions...>::operator*=(value_type const& factor)
{
  multiply(values, factor); 
}

/**
 * @brief Applies compound division by a given divisor to each element in the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline void MultiIndexArray<T, Ordering, Dimensions...>::operator/=(value_type const& divisor)
{
  divide(values, divisor); 
}

/**
 * @brief Applies compound addition to each element in the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline void MultiIndexArray<T, Ordering, Dimensions...>::operator+=(MultiIndexArray const& src)
{
  addToArray(values, src.values);
}

/**
 * @brief Applies compound addition to each element in the array.
 * @tparam S the value_type of the source array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename S, typename>
inline void MultiIndexArray<T, Ordering, Dimensions...>::operator+=(MultiIndexArray<S, Ordering, Dimensions...> const& src)
{
  addToArray(values, src.data(), totalSize);
}

/**
 * @brief Applies compound substraction to each element in the array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline void MultiIndexArray<T, Ordering, Dimensions...>::operator-=(MultiIndexArray const& src)
{
  subtractFromArray(values, src.values);
}

/**
 * @brief Applies compound subtraction to each element in the array.
 * @tparam S the value_type of the source array.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename S, typename>
inline void MultiIndexArray<T, Ordering, Dimensions...>::operator-=(MultiIndexArray<S, Ordering, Dimensions...> const& src)
{
  subtractFromArray(values, src.data(), totalSize);
}

/**
 * @brief streams the Array to an ostream object.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
template <typename CharT, typename CharTraits>
auto MultiIndexArray<T, Ordering, Dimensions...>::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
  const auto space    = ostr.widen(' ');
  const auto oldFlags = ostr.flags();
  ostr.setf(std::ios::fixed);
  ostr.fill(space);
  const auto curPrec = ostr.precision(2);
  for(const auto& v : *this)
  {
    ostr << std::setw(6) << v << space;
  }

  ostr.flags(oldFlags);
  ostr.precision(curPrec);
  return ostr;
}

/**
 * @brief comparison operator for MultiIndexArrays.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
constexpr auto operator==(MultiIndexArray<T, Ordering, Dimensions...> const& x, MultiIndexArray<T, Ordering, Dimensions...> const& y)
{
  return x.equals(y);
}

template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator!=(MultiIndexArray<T, Ordering, Dimensions...> const& x, MultiIndexArray<T, Ordering, Dimensions...> const& y)
{
  return !(x == y);
}

/**
 * @brief multiplication of MultiIndexArrays with scalars.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator*(MultiIndexArray<T, Ordering, Dimensions...> const& x, T factor)
{
  MultiIndexArray<T, Ordering, Dimensions...> result(x);
  result *= factor;
  return result;
}

template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator*(T factor, MultiIndexArray<T, Ordering, Dimensions...> const& x)
{
  return x * factor;
}

/**
 * @brief division of MultiIndexArrays by a given divisor.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator/(MultiIndexArray<T, Ordering, Dimensions...> const& x, T divisor)
{
  MultiIndexArray<T, Ordering, Dimensions...> result(x);
  result /= divisor;
  return result;
}

/**
 * @brief addition of MultiIndexArrays.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator+(MultiIndexArray<T, Ordering, Dimensions...> const& x, MultiIndexArray<T, Ordering, Dimensions...> const& y)
{
  MultiIndexArray<T, Ordering, Dimensions...> result(x);
  result += y;
  return result;
}

/**
 * @brief addition of MultiIndexArrays with different types.
 */
template <typename T, typename S, storageOrdering Ordering, std::size_t... Dimensions, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
inline auto operator+(MultiIndexArray<T, Ordering, Dimensions...> const& x, MultiIndexArray<S, Ordering, Dimensions...> const& y)
{
  MultiIndexArray<std::common_type_t<T, S>, Ordering, Dimensions...> result(x);
  result += y;
  return result;
}

/**
 * @brief subtraction of MultiIndexArrays.
 */
template <typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator-(MultiIndexArray<T, Ordering, Dimensions...> const& x, MultiIndexArray<T, Ordering, Dimensions...> const& y)
{
  MultiIndexArray<T, Ordering, Dimensions...> result(x);
  result -= y;
  return result;
}

/**
 * @brief subtraction of MultiIndexArrays with different types.
 */
template <typename T, typename S, storageOrdering Ordering, std::size_t... Dimensions, typename = std::enable_if_t<std::is_convertible_v<S, T>>>
inline auto operator-(MultiIndexArray<T, Ordering, Dimensions...> const& x, MultiIndexArray<S, Ordering, Dimensions...> const& y)
{
  MultiIndexArray<std::common_type_t<T, S>, Ordering, Dimensions...> result(x);
  result -= y;
  return result;
}

/**
 * @brief stream operator.
 */
template <typename CharT, typename CharTraits, typename T, storageOrdering Ordering, std::size_t... Dimensions>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, MultiIndexArray<T, Ordering, Dimensions...> const& arr) 
 -> std::basic_ostream<CharT, CharTraits>&
{
  return arr.put(ostr);
}


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTIINDEXARRAY_H_1774211191304802872220982451316113153173835 
