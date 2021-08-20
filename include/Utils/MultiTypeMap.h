/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    MultiTypeMap.h 
 * @brief   Definition of the MultiTypeMapT template.
 * 
 * @author  Lasse Rosenthal 
 * @date    01.07.2020 
 */ 
 
#ifndef MULTITYPEMAP_H_2453950093182223331177781544060872479324625 
#define MULTITYPEMAP_H_2453950093182223331177781544060872479324625 
 
#pragma warning(disable : 4348)
#pragma warning(disable : 4800)


// includes
#include <Meta/Utility.h>

#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>


namespace cpptools {


/** 
 * @class MultiTypeMapT
 * @brief MultiTypeMapT is an associative container that can store different types. 
 */
template <template<typename...> class MapType, typename T, typename... Ts>
class MultiTypeMapT {

  /** 
   * @brief  a simple template alias to determine the return type of the
   *         type-based const get member function.
   * @tparam V the type of the value to look up.
   * @remark if the template Parameter V is not an arithmetic type, the internally defined
   *         type is a const l-value reference of type V. For arithmetic types type is
   *         defined as V, i.e. a copy is returned.
   */
  template <typename V>
  using ConstReturnType = meta::ConstRefOrValue<std::decay_t<V>>;

  /** 
   * @brief  a simple template alias to determine the return type of the
   *         type-based non-const get member function.
   * @tparam V the type of the value to look up.
   * @remark if the template Parameter V is not an arithmetic type, the internally defined
   *         type is an l-value reference of type V. For arithmetic types type is
   *         defined as V, i.e. a copy is returned.
   */
  template <typename V>
  using NonConstReturnType = meta::RefOrValue<std::decay_t<V>>;

public:

  // ---------------------------------------------------
  // public types
  using variantType    = std::variant<Ts...>;
  using mapType        = MapType<T, variantType>;
  using key_type       = typename mapType::key_type;
  using mapped_type    = typename mapType::mapped_type;
  using value_type     = typename mapType::value_type;
  using iterator       = typename mapType::iterator;
  using const_iterator = typename mapType::const_iterator;
  using size_type      = typename mapType::size_type;

  // ---------------------------------------------------
  // public methods
  decltype(auto) operator[] (key_type const&);
  decltype(auto) operator[] (key_type&&);
  decltype(auto) at         (key_type const&);
  decltype(auto) at         (key_type const&) const;

  template <typename V, typename = std::enable_if_t<meta::isOneOf<std::decay_t<V>, Ts...>>>
  auto get (const T& key) const -> ConstReturnType<V>;

  template <typename V, typename = std::enable_if_t<meta::isOneOf<std::decay_t<V>, Ts...>>>
  auto get (const T& key) -> NonConstReturnType<V>;

  auto empty    () const noexcept -> bool;
  auto size     () const noexcept -> size_type;
  auto max_size () const noexcept -> size_type;
  void clear    ();
  auto begin    () noexcept -> iterator;
  auto end      () noexcept -> iterator;
  auto cbegin   () const noexcept -> const_iterator;
  auto cend     () const noexcept -> const_iterator;
  auto insert   (value_type const&) -> std::pair<iterator, bool>;
  auto find     (key_type const&) -> iterator;
  auto find     (key_type const&) const -> const_iterator;
  auto contains (key_type const&) const ->  bool;

private:

  // ---------------------------------------------------
  // private data
  mapType dataMap;
};

/** 
 * @brief Returns a reference to the std::variant that is mapped to a key equivalent to key,
 *        performing an insertion if such key does not already exist.
 * @param key a const l-value reference to the key of the element to find.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline decltype(auto) MultiTypeMapT<MapType, T, Ts...>::operator[] (key_type const& key)
{
  return dataMap[key];
}

/** 
 * @brief Returns a reference to the std::variant that is mapped to a key equivalent to key,
 *        performing an insertion if such key does not already exist.
 * @param key an r-value reference to the key of the element to find.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline decltype(auto) MultiTypeMapT<MapType, T, Ts...>::operator[] (key_type&& key)
{
  return dataMap[std::forward<key_type>(key)];
}

/** 
 * @brief Returns a reference to the std::variant that is mapped to a key equivalent to key.
 *        If no such element exists, an exception of type std::out_of_range is thrown.
 * @param key an r-value reference to the key of the element to find.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline decltype(auto) MultiTypeMapT<MapType, T, Ts...>::at(key_type const& key)
{
  return dataMap.at(key);
}

/** 
 * @brief Returns a reference to the std::variant that is mapped to a key equivalent to key.
 *        If no such element exists, an exception of type std::out_of_range is thrown.
 * @param key an r-value reference to the key of the element to find.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline decltype(auto) MultiTypeMapT<MapType, T, Ts...>::at(key_type const& key) const
{
  return dataMap.at(key);
}

/** 
 * @brief  Provides type-based access to the value stored in the variant.
 * @param  key an r-value reference to the key of the element to find.
 * @remark If no such element exists, an exception of type std::out_of_range is thrown.
 * @return An instantiation of \link #ConstReturnType \endlink
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
template <typename V, typename>
inline auto MultiTypeMapT<MapType, T, Ts...>::get(const T& key) const -> ConstReturnType<V>
{
  return std::get<std::decay_t<V>>(dataMap.at(key));
}

/** 
 * @brief  Provides type-based access to the value stored in the variant.
 * @param  key an r-value reference to the key of the element to find.
 * @remark If no such element exists, an exception of type std::out_of_range is thrown.
 * @return An instantiation of \link #NonConstReturnType \endlink
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
template <typename V, typename>
inline auto MultiTypeMapT<MapType, T, Ts...>::get(const T& key) -> NonConstReturnType<V>
{
  return std::get<std::decay_t<V>>(dataMap.at(key));
}

/** 
 * @brief Checks if the container has no elements, i.e. whether begin() == end().
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::empty() const noexcept -> bool
{
  return dataMap.empty();
}

/** 
 * @brief Returns the number of elements in the container
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::size() const noexcept -> size_type
{
  return dataMap.size();
}

/** 
 * @brief Returns the maximum number of elements the container is able to hold due to system or
 *        library implementation limitations, i.e. std::distance(begin(), end()) for the largest container
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::max_size() const noexcept -> size_type
{
  return dataMap.max_size();
}

/** 
 * @brief Erases all elements from the container. After this call, size() returns zero.
 *        Invalidates any references, pointers, or iterators referring to contained elements.
 *        Any past-the-end iterator remains valid.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline void MultiTypeMapT<MapType, T, Ts...>::clear()
{
  return dataMap.clear();
}

/** 
 * @brief Returns an iterator to the first element of the map.
 *        If the map is empty, the returned iterator will be equal to end().
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::begin() noexcept -> iterator
{
  return dataMap.begin();
}

/** 
 * @brief Returns an iterator to the element following the last element of the map.
 *        This element acts as a placeholder; attempting to access it results in undefined behavior.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::end() noexcept -> iterator
{
  return dataMap.end();
}

/** 
 * @brief Returns a const iterator to the first element of the map.
 *        If the map is empty, the returned iterator will be equal to end().
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::cbegin() const noexcept -> const_iterator
{
  return dataMap.cbegin();
}

/** 
 * @brief Returns an iterator to the element following the last element of the map.
 *        This element acts as a placeholder; attempting to access it results in undefined behavior.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::cend() const noexcept -> const_iterator
{
  return dataMap.cend();
}

/** 
 * @brief Inserts element(s) into the container, if the container doesn't already
 *        contain an element with an equivalent key.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::insert(value_type const& val) -> std::pair<iterator, bool>
{
  return dataMap.insert(val);
}

/** 
 * @brief  Finds an element with key equivalent to key
 * @return An Iterator to an element with key equivalent to key.
 *         If no such element is found, past-the-end iterator is returned.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::find(key_type const& key) -> iterator
{
  return dataMap.find(key);
}

/** 
 * @brief  Finds an element with key equivalent to key
 * @return An Iterator to an element with key equivalent to key.
 *         If no such element is found, past-the-end iterator is returned.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::find(key_type const& key) const -> const_iterator
{
  return dataMap.find(key);
}

/** 
 * @brief  Checks if the container contains with an element whose key is identical
 *         to the given key.
 * @return true if there is such an element, otherwise false.
 */
template <template<typename...> class MapType, typename T, typename ...Ts>
inline auto MultiTypeMapT<MapType, T, Ts...>::contains(key_type const& key) const -> bool
{
  return dataMap.find(key) != cend();
}

/** 
 * convenient typedef for multitype maps based on std::unordered_map
 */
template <typename T, typename... Ts>
using MultiTypeHashMap = MultiTypeMapT<std::unordered_map, T, Ts...>;

/** 
 * convenient typedef for multitype maps based on std::map
 */
template <typename T, typename... Ts>
using MultiTypeMap = MultiTypeMapT<std::map, T, Ts...>;


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // MULTITYPEMAP_H_2453950093182223331177781544060872479324625 
