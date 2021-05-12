/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    IntegerRange.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    11.05.2021 
 */ 
 
#ifndef INTEGERRANGE_H_1219517403308018514110283203352292889721616 
#define INTEGERRANGE_H_1219517403308018514110283203352292889721616 
 
 
// includes
#include <iterator>
#include <functional>
 
 
namespace cpptools {


/** 
 * @class IntegerRange
 * @brief 
 */
template <typename T>
class IntegerRange {

public:

  class RangeSentinel {};
  class Iterator;

  // ---------------------------------------------------
  // public types
  using value_type  = T;
  using filter_type = std::function<bool(value_type)>;

  using iterator = Iterator;

  // ---------------------------------------------------
  // constructors
  IntegerRange(value_type startValue, value_type endValue, value_type step = 1,
               filter_type = [](value_type) noexcept { return true; });

  // ---------------------------------------------------
  // public methods
  auto begin () -> iterator;
  auto end   () const noexcept -> RangeSentinel;

  friend auto operator|(IntegerRange range, filter_type newFilter) -> IntegerRange
  {
    return IntegerRange{range.startValue, range.endValue, range.step,
                        [f1 = range.filter, f2 = newFilter](value_type v) {
                          return f1(v) && f2(v);
                        }};
  }

private:

  // ---------------------------------------------------
  // private data
  value_type  startValue;
  value_type  endValue;
  value_type  step;
  filter_type filter;
};

template <typename T>
class IntegerRange<T>::Iterator {

public:

  // ---------------------------------------------------
  // iterator traits
  using value_type        = typename IntegerRange<T>::value_type;
  using difference_type   = std::ptrdiff_t;
  using reference         = value_type const&;
  using pointer           = value_type const*;
  using iterator_category = std::input_iterator_tag;

  // ---------------------------------------------------
  // public types
  using filter_type     = typename IntegerRange<T>::filter_type;
  using RangeSentinel   = typename IntegerRange<T>::RangeSentinel;

  // ---------------------------------------------------
  // constructors & destructor
  Iterator       (value_type startValue, value_type evd, value_type step, filter_type filter);
  Iterator       (Iterator const&) = default;
  auto operator= (Iterator const&) -> Iterator& = default;
  ~Iterator      () = default;

  // ---------------------------------------------------
  // iterator api
  auto operator*  () const noexcept -> value_type;

  auto operator++ () -> Iterator&;
  auto operator++ (int) -> Iterator;

  auto operator==(Iterator const& it) -> bool { return value == *it; }
  auto operator!=(Iterator const& it) -> bool { return !(*this == it); }
  auto operator==(RangeSentinel sen) -> bool { return value >= endValue; }
  auto operator!=(RangeSentinel sen) -> bool { return !(*this == sen); }

private:

  // ---------------------------------------------------
  // private data
  value_type    value;
  value_type    endValue;
  value_type    step;
  filter_type   filter;
  RangeSentinel sentinel;
};

template <typename T>
inline IntegerRange<T>::Iterator::Iterator(value_type startValue, value_type endValue,
                                           value_type step, filter_type filter)
  : value    {startValue}
  , endValue {endValue}
  , step     {step}
  , filter   {filter}
{
  if (!filter(value))
  {
    ++(*this);
  }
}

template <typename T>
inline auto IntegerRange<T>::Iterator::operator*() const noexcept -> value_type
{
  return value;
}

template <typename T>
auto IntegerRange<T>::Iterator::operator++() -> Iterator&
{
  do
  {
    value += step;
  } while(!filter(value) && *this != sentinel);
  return *this;
}

template <typename T>
inline auto IntegerRange<T>::Iterator::operator++(int) -> Iterator
{
  Iterator copy{*this};
  ++copy;
  return copy;
}

template <typename T>
inline IntegerRange<T>::IntegerRange(value_type startValue, value_type endValue,
                                     value_type step, filter_type filter)
  : startValue {startValue}
  , endValue   {endValue}
  , step       {step}
  , filter     {filter}
{}

template <typename T>
inline auto IntegerRange<T>::begin() -> iterator
{
  return iterator{startValue, endValue, step, filter};
}

template <typename T>
inline auto IntegerRange<T>::end() const noexcept -> RangeSentinel
{
  return {};
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // INTEGERRANGE_H_1219517403308018514110283203352292889721616 
