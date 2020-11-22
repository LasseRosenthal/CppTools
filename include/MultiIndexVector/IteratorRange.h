/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    IteratorRange.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.09.2020 
 */ 
 
#ifndef ITERATORRANGE_H_7154247648573474617255280313138217266446 
#define ITERATORRANGE_H_7154247648573474617255280313138217266446 
 
 
namespace multiindex {

 
/** 
 * @class IteratorRange
 * @brief IteratorRange is a simple wrapper for a pair of two iterator providing begin/end methods
 */
template <typename Iter>
struct IteratorRange {

  // ---------------------------------------------------
  // api
  constexpr auto begin () const noexcept;
  constexpr auto end   () const noexcept;

  // ---------------------------------------------------
  // data
  Iter first;
  Iter last;
};


/**
 * @brief Returns the first iterator.
 */
template <typename Iter>
constexpr auto IteratorRange<Iter>::begin() const noexcept {
  return first;
}

/**
 * @brief Returns the last iterator.
 */
template <typename Iter>
constexpr auto IteratorRange<Iter>::end() const noexcept {
  return last;
}


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ITERATORRANGE_H_7154247648573474617255280313138217266446 
