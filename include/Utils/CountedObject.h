/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    CountedObject.h 
 * @brief   Definition of the CountedObject class.
 *          CountedObject is a convenience class to can be used as a CRTP class
 *          to define classes that provide atomic reference counting.
 * 
 * @author  Lasse Rosenthal 
 * @date    06.08.2021 
 */ 
 
#ifndef COUNTEDOBJECT_H_37212169599302063324431459525440893611682 
#define COUNTEDOBJECT_H_37212169599302063324431459525440893611682 
 
 
// includes
#include <atomic>
#include <cstddef>


namespace cpptools {


/**
 * @class CountedObject
 * @brief CountedObject is a convenience class to can be used as a CRTP class
 *        to define classes that provide atomic reference counting.
 */
template <typename Host, typename T = std::size_t>
class CountedObject {

  // ---------------------------------------------------
  // private aliases
  using value_type = T;

public:

  // ---------------------------------------------------
  // api
  static auto count() noexcept -> value_type;

protected:

  // ---------------------------------------------------
  // special member functions
  CountedObject  () noexcept;
  CountedObject  (CountedObject const&) noexcept;
  CountedObject  (CountedObject&&) noexcept;
  auto operator= (CountedObject const&) noexcept -> CountedObject& = default;
  auto operator= (CountedObject&&) noexcept -> CountedObject& = default;
  ~CountedObject () noexcept;
  
private:

  // ---------------------------------------------------
  // data
  inline static std::atomic<value_type> ctr{}; ///< the counter that holds the number of currently living objects
};


/**
 * @brief Default constructor. Increases the counter by one.
 */
template <typename Host, typename T>
inline CountedObject<Host, T>::CountedObject() noexcept {
  ++ctr;
}

/**
 * @brief Copy constructor. Increases the counter by one.
 */
template <typename Host, typename T>
inline CountedObject<Host, T>::CountedObject(CountedObject const&) noexcept
  : CountedObject()
{}

/**
 * @brief Move constructor. Increases the counter by one.
 */
template <typename Host, typename T>
inline CountedObject<Host, T>::CountedObject(CountedObject&&) noexcept
  : CountedObject()
{}

/**
 * @brief Destructor. Decreases the object counter by one.
 */
template <typename Host, typename T>
inline CountedObject<Host, T>::~CountedObject() noexcept {
  --ctr;
}

/**
 * @brief Returns the current number of instances.
 */
template <typename Host, typename T>
inline auto CountedObject<Host, T>::count() noexcept -> value_type
{
  return ctr.load();
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // COUNTEDOBJECT_H_37212169599302063324431459525440893611682 
