/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    EnumRange.h 
 * @brief   Definition of the EnumRange class.
 * 
 * @author  Lasse Rosenthal 
 * @date    08.07.2020 
 */

#ifndef ENUMRANGE_H_3324320342892823589193127310308841248313102 
#define ENUMRANGE_H_3324320342892823589193127310308841248313102 
 
 
// includes
#include <Meta/Typelist.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <tuple>
 

namespace cpptools {

 
template <typename E, E... enumValues>
class EnumRange;

/** 
 * @class EnumIterator
 * @brief EnumIterator is a stl-conform input iterator
 *        that allows to iterate over the defined values of an enumeration
 * @see   https://en.cppreference.com/w/cpp/named_req/InputIterator
 */
template <typename E, typename Enable, E... enumValues>
class EnumIterator;

/** 
 * @brief Partial template specialization of class EnumIterator
 *        that is enabled for enumeration types.
 */
template <typename E, E... enumValues>
class EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...> {

  friend class EnumRange<E, enumValues...>;

public:

  // --------------------------------------------------- 
  // iterator properties
  using iterator_category = std::input_iterator_tag;
  using value_type        = E;
  using difference_type   = std::ptrdiff_t;
  using pointer           = value_type const*;
  using reference         = value_type const&;

  // --------------------------------------------------- 
  // constructors & assignment
  constexpr EnumIterator          () = default;
  constexpr explicit EnumIterator (E e);
  constexpr EnumIterator          (EnumIterator const& other) = default;
  constexpr auto operator=        (EnumIterator const& other) -> EnumIterator& = default;

  // --------------------------------------------------- 
  // iterator operations
  constexpr auto operator*  () const -> const reference;
  constexpr auto operator++ () const -> EnumIterator const&;
  constexpr auto operator++ (int) const -> EnumIterator;

  // --------------------------------------------------- 
  // static begin and end methods for convenience
  constexpr static auto begin () -> EnumIterator;
  constexpr static auto end   () -> EnumIterator;

  // --------------------------------------------------- 
  // comparison operations
  friend constexpr bool operator== (EnumIterator const& lhs, EnumIterator const& rhs)
  {
    return lhs.pos == rhs.pos;
  }

  friend constexpr bool operator!= (EnumIterator const& lhs, EnumIterator const& rhs)
  {
    return !(lhs == rhs);
  }

  template <typename F>
  static constexpr auto forEach(F&& f) -> F&&;

private:

  // --------------------------------------------------- 
  // static members
  static constexpr std::size_t size = sizeof...(enumValues);
  static constexpr std::array<E, size> values = {enumValues...};

  // --------------------------------------------------- 
  // non static members
  mutable difference_type pos{size};

  template <typename F, std::size_t... Is>
  static constexpr auto forEach (F&& f, std::index_sequence<Is...>) -> F&&;
};

/**
 * @brief constructs the iterator with an enumeration value.
 */
template <typename E, E... enumValues>
constexpr EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::EnumIterator(E e)
  : pos {std::distance(values.begin(), std::find(values.begin(), values.end(), e))}
{}

/**
 * @brief returns the current value.
 */
template <typename E, E... enumValues>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::operator*() const -> const reference
{
  return values[pos];
}

/**
 * @brief increment operator (prefix version).
 */
template <typename E, E... enumValues>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::operator++() const -> EnumIterator const&
{
  ++pos;
  return *this;
}

/**
 * @brief increment operator (postfix version).
 */
template <typename E, E... enumValues>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::operator++(int) const -> EnumIterator
{
  EnumIterator cpy{*this};
  this->operator++();
  return cpy;
}

/**
 * @brief begin returns an iterator pointing to the first entry in the static array.
 */
template <typename E, E... enumValues>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::begin() -> EnumIterator
{
  EnumIterator e;
  e.pos = 0;
  return e;
}

/**
 * @brief end Returns an iterator marking the end of the enumeration range.
 *       This element acts as a placeholder; attempting to access it results in undefined behavior.
 */
template <typename E, E... enumValues>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::end() -> EnumIterator
{
  return {};
}

template <typename E, E... enumValues>
template <typename F, std::size_t... Is>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::forEach(F&& f, std::index_sequence<Is...>) -> F&& {
  (..., f.operator()<values[Is]>());
  return std::forward<F>(f);
}

template <typename E, E... enumValues>
template <typename F>
constexpr auto EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>::forEach(F&& f) -> F&& {
  return forEach(std::forward<F>(f), std::make_index_sequence<sizeof...(enumValues)>{});
}

/** 
 * @class EnumRange
 * @brief EnumRange offers a begin and end method to be used in loops.
 */
template <typename E, E... enumValues>
class EnumRange {

public:

  // --------------------------------------------------- 
  // public types
  using value_type     = E;
  using iterator       = EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>;
  using const_iterator = EnumIterator<E, std::enable_if_t<std::is_enum_v<E>>, enumValues...>;

  // --------------------------------------------------- 
  // construction
  constexpr EnumRange () = default;
  constexpr EnumRange (value_type first, value_type last);

  // --------------------------------------------------- 
  // begin /end methods
  constexpr auto begin () -> iterator;
  constexpr auto end   () -> iterator;
  constexpr auto begin () const -> const_iterator;
  constexpr auto end   () const -> const_iterator;

  template <typename F>
  static auto constexpr forEach(F&& f) -> F&&;

private:

  // --------------------------------------------------- 
  // private data
  const iterator first {iterator::begin()};
  const iterator last  {iterator::end()};
};


template <typename E, E... enumValues>
constexpr EnumRange<E, enumValues...>::EnumRange(value_type first, value_type last)
  : first {first}
  , last  {++iterator{last}}
{}


/**
 * @brief begin returns an iterator pointing to the first enum value in array.
 */
template <typename E, E... enumValues>
constexpr auto EnumRange<E, enumValues...>::begin() -> iterator
{
  return first;
}

/**
 * @brief end returns an iterator pointing to the end of the enumeration range.
 */
template <typename E, E... enumValues>
constexpr auto EnumRange<E, enumValues...>::end() -> iterator
{
  return last;
}

/**
 * @brief begin returns an iterator pointing to the first enum value.
 */
template <typename E, E... enumValues>
constexpr auto EnumRange<E, enumValues...>::begin() const -> const_iterator
{
  return first;
}

/**
 * @brief end returns an iterator pointing to the end of the enumeration range.
 */
template <typename E, E... enumValues>
constexpr auto EnumRange<E, enumValues...>::end() const -> const_iterator
{
  return last;
}

/**
 *  @brief  generic for each method.
 */
template <typename E, E... enumValues>
template <typename F>
auto constexpr EnumRange<E, enumValues...>::forEach(F&& f) -> F&&
{
  return iterator::forEach(std::forward<F>(f));
}


/** 
 * @struct EnumUtils
 * @brief  EnumUtils provides several useful typedefs.
 */
template <typename E, E... enumValues>
struct EnumUtils {

  using EnumRange = EnumRange<E, enumValues...>;

  template <template <E> class T>
  using TupleExpander = std::tuple<T<enumValues>...>;

  template <template <E> class T>
  using templateTupleTypeList = meta::Typelist<T<enumValues>...>;
};


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ENUMRANGE_H_3324320342892823589193127310308841248313102 
