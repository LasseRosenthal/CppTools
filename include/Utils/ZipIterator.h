/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ZipIterator.h 
 * @brief   Definition of the ZipIterator and the Zipper classes.
 *          ZipIterator is a forward iterator that aggregates an arbitrary number of iterators for parallel iteration.
 *          Zipper is a range class that provides begin and end methods that may be used in loops.
 * 
 * @author  Lasse Rosenthal 
 * @date    12.08.2020 
 */ 
 
#ifndef ZIPITERATOR_H_2761028059265252111017216268961088015170186 
#define ZIPITERATOR_H_2761028059265252111017216268961088015170186 
 
 
// includes
#include <Meta/Utility.h>

#include <tuple>
#include <type_traits>


namespace cpptools {

 
/** 
 * @class ZipIterator.
 * @brief ZipIterator is a forward iterator that aggregates an arbitrary number
 *        of iterators for parallel iteration.
 * @see   https://en.cppreference.com/w/cpp/named_req/ForwardIterator
 */
template <typename... Iters>
class ZipIterator {

public:

  // --------------------------------------------------- 
  // public constants
  static constexpr std::size_t extent = sizeof...(Iters);

  // --------------------------------------------------- 
  // public types
  using IteratorTypes = std::tuple<Iters...>;

  template <std::size_t index>
  using iterator_type = typename std::tuple_element_t<index, IteratorTypes>;

  template <std::size_t index>
  using type = typename iterator_type<index>::value_type;

  template <std::size_t index>
  using referenceT = typename iterator_type<index>::reference;

private:

  // ---------------------------------------------------
  // ReferenceSequenceGen is used to construct a tuple type
  // with references to the value_types of the stored iterators as elements.
  // for a given length N, ReferenceSequenceGen recursively constructs an
  // index sequence which can be used in pack expansion.
  // ReferenceSequenceGen<N>::type = ReferenceSequenceGen<N-1, N-1>::type = ReferenceSequenceGen <N-2, N-2, N-1>::type
  // = ... = ReferenceSequenceGen<0, 0, 1, ..., N-1>::type
  template <std::size_t...>
  struct ReferenceSequenceGenT;

  template <std::size_t... Is>
  using ReferenceSequenceGen = typename ReferenceSequenceGenT<Is...>::type;

  template <std::size_t I, std::size_t... Is>
  struct ReferenceSequenceGenT<I, Is...>
    : meta::IdentityT<ReferenceSequenceGen<I - 1, I - 1, Is...>> {};

  template <std::size_t... Is>
  struct ReferenceSequenceGenT<0, Is...> : meta::IdentityT<std::tuple<referenceT<Is>...>> {};

  using extentIndexSequence = std::make_index_sequence<extent>;

public:

  // ---------------------------------------------------
  // iterator tags
  using iterator_category = std::forward_iterator_tag;
  using value_type        = ReferenceSequenceGen<extent>;
  using difference_type   = std::ptrdiff_t;
  using pointer           = value_type*;
  using reference         = value_type&;

  // ---------------------------------------------------
  // construction, destruction & assignment
  ZipIterator    () = default;
  ZipIterator    (Iters...);
  ZipIterator    (ZipIterator const& src) = default;
  ZipIterator    (ZipIterator&& src) = default;
  ~ZipIterator   () = default;
  auto operator= (ZipIterator const& src) -> ZipIterator&;
  auto operator= (ZipIterator&& src) -> ZipIterator&;

  // ---------------------------------------------------
  // public api
  void swap       (ZipIterator& other) noexcept;
  auto operator++ () -> ZipIterator&;
  auto operator++ (int) -> ZipIterator&;
  auto operator*  () -> value_type;

  // ---------------------------------------------------
  // friends
  template <typename... Iters2>
  friend bool operator!= (ZipIterator<Iters2...> const& lhs, ZipIterator<Iters2...> const& rhs);

private:

  // ---------------------------------------------------
  // private methods
  template <std::size_t... Is>
  void increment (std::index_sequence<Is...>);

  template <std::size_t... Is>
  auto getValue (std::index_sequence<Is...>) -> value_type;

  template <std::size_t... Is1, std::size_t... Is2>
  auto notEqual (ZipIterator const& other, std::index_sequence<Is1...>,
                 std::index_sequence<Is2...>) const -> bool;

  // ---------------------------------------------------
  // data
  IteratorTypes iterators;
};


/**
 * @brief Constructor. Creates a tuple that stores the provided iterators
 */
template <typename... Iters>
inline ZipIterator<Iters...>::ZipIterator(Iters... iters)
  : iterators{iters...}
{}

/**
 * @brief Copy assignment.
 */
template <typename... Iters>
inline auto ZipIterator<Iters...>::operator=(ZipIterator const& src) -> ZipIterator&
{
  ZipIterator copy{src};
  swap(copy);
  return *this;
}

/**
 * @brief Move assignment.
 */
template <typename... Iters>
inline auto ZipIterator<Iters...>::operator=(ZipIterator&& src) -> ZipIterator&
{
  iterators = std::move(src.iterators);
  return *this;
}

/**
 * @brief Swap method. Exchanges the iterator tuple of the ZipIterator with the iterator
 *        tuple of a given ZipIterator.             
 */
template <typename... Iters>
inline void ZipIterator<Iters...>::swap(ZipIterator& other) noexcept
{
  iterators.swap(other.iterators);
}

/**
 * @brief increment operator (prefix version).
 */
template <typename... Iters>
inline auto ZipIterator<Iters...>::operator++() -> ZipIterator&
{
  increment(extentIndexSequence());
  return *this;
}

/**
 * @brief increment operator (postfix version).
 */
template <typename... Iters>
inline auto ZipIterator<Iters...>::operator++(int) -> ZipIterator&
{
  ZipIterator tmp{*this};
  increment(extentIndexSequence());
  return tmp;
}

/**
 * @brief Auxiliary method that increments all stored iterators.
 */
template <typename... Iters>
template <std::size_t... Is>
inline void ZipIterator<Iters...>::increment(std::index_sequence<Is...>)
{
  (..., ++std::get<Is>(iterators));
}

/**
 * @brief  returns the current value which is a tuple of lvalue-references
 *         to the values the stored iterators are pointing to.
 * @remark since the return type is a tuple, it can be decomposed via structered bindings.
 */
template <typename... Iters>
inline auto ZipIterator<Iters...>::operator*() -> value_type
{
  return getValue(extentIndexSequence());
}

/**
 * @brief Auxiliary method that creates a tuple of lvalue-references
 *        to the values the stored iterators are pointing to.  
 */
template <typename... Iters>
template <std::size_t... Is>
inline auto ZipIterator<Iters...>::getValue(std::index_sequence<Is...>) -> value_type
{
  return value_type((*std::get<Is>(iterators))...);
}

/**
 * @brief   Auxiliary method that checks the inequality of two given ZipIterators.
 * @remarks since we want to loop over ranges that may have different lengths,
 *          it is generally not possible to match both end iterators at the same
 *          time. Thus iterating has to be aborted when the first end iterator
 *          of either range is reached.
 */
template <typename... Iters>
template <std::size_t... Is1, std::size_t... Is2>
inline auto ZipIterator<Iters...>::notEqual(ZipIterator const& other, 
                                            std::index_sequence<Is1...>,
                                            std::index_sequence<Is2...>) const -> bool
{
  return (... && (std::get<Is1>(iterators) != std::get<Is2>(other.iterators)));
}

/**
 * @brief Test for inequality
 */
template <typename... Iters>
bool operator!= (ZipIterator<Iters...> const& lhs, ZipIterator<Iters...> const& rhs)
{
  return lhs.notEqual(rhs, std::index_sequence_for<Iters...>{}, std::index_sequence_for<Iters...>{});
}

/**
 * @brief Test for equality
 */
template <typename... Iters>
inline auto operator== (ZipIterator<Iters...> const& lhs, ZipIterator<Iters...> const& rhs) -> bool
{
  return !(lhs != rhs);
}


/** 
 * @class Zipper.
 * @brief Zipper is a range class that returns ZipIterators from its
 *        begin and end methods
 */
template <typename... Ts>
class Zipper {

public:

  // --------------------------------------------------- 
  // public types

  // Conts is tuple holding references to the containers.
  using Conts = std::tuple<Ts&...>;

  template <std::size_t index>
  using iteratorType = typename std::remove_reference_t<std::tuple_element_t<index, Conts>>::iterator;

  template <std::size_t index>
  using constIteratorType = typename std::remove_reference_t<std::tuple_element_t<index, Conts>>::const_iterator;

private:

  // ---------------------------------------------------
  // ZipIteratorGenerator is used to construct a tuple type that holds iterators.
  template <bool IsConst, std::size_t...>
  struct ZipIteratorGeneratorT;

  template <bool IsConst, std::size_t... Is>
  using ZipIteratorGenerator = typename ZipIteratorGeneratorT<IsConst, Is...>::type;

  template <bool IsConst, std::size_t I, std::size_t... Is>
  struct ZipIteratorGeneratorT<IsConst, I, Is...>
    : meta::IdentityT<ZipIteratorGenerator<IsConst, I - 1, I - 1, Is...>> {};

  template <bool IsConst, std::size_t... Is>
  struct ZipIteratorGeneratorT<IsConst, 0, Is...> 
    : meta::IdentityT <ZipIterator<std::conditional_t<IsConst, constIteratorType<Is>, iteratorType<Is>>...>> {};

public:

  // ---------------------------------------------------
  // the iterator type
  using iterator       = ZipIteratorGenerator<false, sizeof...(Ts)>;
  using const_iterator = ZipIteratorGenerator<true, sizeof...(Ts)>;
  
  // ---------------------------------------------------
  // construction, destruction & assignment
  Zipper  () = delete;
  Zipper  (Ts&... conts);
  Zipper  (Zipper const&) = default;
  Zipper  (Zipper&&) = delete;
  ~Zipper () = default;

  // ---------------------------------------------------
  // public api
  auto begin  () -> iterator;
  auto end    () -> iterator;
  auto begin  () const -> const_iterator;
  auto end    () const -> const_iterator;
  auto cbegin () const -> const_iterator;
  auto cend   () const -> const_iterator;

private:

  // ---------------------------------------------------
  // private methods
  template <std::size_t... Is>
  auto begin  (std::index_sequence<Is...>) -> iterator;
  template <std::size_t... Is>
  auto cbegin (std::index_sequence<Is...>) const -> const_iterator;
  template <std::size_t... Is>
  auto end    (std::index_sequence<Is...>) -> iterator;
  template <std::size_t... Is>
  auto cend   (std::index_sequence<Is...>) const -> const_iterator;

  // ---------------------------------------------------
  // data
  Conts m_conts;
};


/**
 * @brief Constructor. Creates a tuple holding references to the given containers.
 */
template <typename... Ts>
inline Zipper<Ts...>::Zipper(Ts&... conts)
  : m_conts{conts...}
{}

/**
 * @brief Begin method. returns a ZipIterator aggregating non-const begin iterators.
 */
template <typename... Ts>
inline auto Zipper<Ts...>::begin() -> iterator
{
  return begin(std::index_sequence_for<Ts...>{});
}

/**
 * @brief  Begin method. returns a ZipIterator aggregating const begin iterators.
 * @see    https://en.cppreference.com/w/cpp/named_req/Container
 */
template <typename... Ts>
inline auto Zipper<Ts...>::begin() const -> const_iterator
{
  return cbegin(std::index_sequence_for<Ts...>{});
}

/**
 * @brief Begin method. returns a ZipIterator aggregating const begin iterators.
 */
template <typename... Ts>
inline auto Zipper<Ts...>::cbegin() const -> const_iterator
{
  return cbegin(std::index_sequence_for<Ts...>{});
}

/**
 * @brief Auxiliary method that creates a ZipIterator aggregating non-const begin iterators.
 */
template <typename... Ts>
template <std::size_t... Is>
inline auto Zipper<Ts...>::begin(std::index_sequence<Is...>) -> iterator
{
  return iterator(std::get<Is>(m_conts).begin()...);
}

/**
 * @brief Auxiliary method that creates a ZipIterator aggregating const begin iterators.
 */
template <typename... Ts>
template <std::size_t... Is>
inline auto Zipper<Ts...>::cbegin(std::index_sequence<Is...>) const -> const_iterator
{
  return const_iterator(std::get<Is>(m_conts).cbegin()...);
}

/**
 * @brief Returns a a ZipIterator aggregating non-const end iterators.
 */
template <typename... Ts>
inline auto Zipper<Ts...>::end() -> iterator
{
  return end(std::index_sequence_for<Ts...>{});
}

/**
 * @brief Returns a a ZipIterator aggregating const end iterators.
 * @see   https://en.cppreference.com/w/cpp/named_req/Container
 */
template <typename... Ts>
inline auto Zipper<Ts...>::end() const -> const_iterator
{
  return cend(std::index_sequence_for<Ts...>{});
}

/**
 * @brief Returns a a ZipIterator aggregating const end iterators.
 */
template <typename... Ts>
inline auto Zipper<Ts...>::cend() const -> const_iterator
{
  return cend(std::index_sequence_for<Ts...>{});
}

/**
 * @brief Auxiliary end method that creates a ZipIterator aggregating non-const end iterators.
 */
template <typename... Ts>
template <std::size_t... Is>
inline auto Zipper<Ts...>::end(std::index_sequence<Is...>) -> iterator
{
  return iterator(std::get<Is>(m_conts).end()...);
}

/**
 * @brief Auxiliary end method that creates a ZipIterator aggregating const end iterators.
 */
template <typename... Ts>
template <std::size_t... Is>
inline auto Zipper<Ts...>::cend(std::index_sequence<Is...>) const -> const_iterator
{
  return const_iterator(std::get<Is>(m_conts).cend()...);
}


// deduction guide
template <typename... Ts>
Zipper(Ts&...) -> Zipper<Ts...>;


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // ZIPITERATOR_H_2761028059265252111017216268961088015170186 
