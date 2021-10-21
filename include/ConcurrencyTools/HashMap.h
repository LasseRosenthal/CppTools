/* -------------------------------------------------------------------------- 
 * Copyright (c) 2019-2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */

/**
 * @file   HashMap.h
 * @brief  implementation of a thread safe hash map
 *         based on Williams - C++ Concurrency in Action
 *         
 * @author Lasse Rosenthal
 * @date   05.07.2019
 */

#ifndef HASHMAP_H_3740194082434717657293912020615842268611759 
#define HASHMAP_H_3740194082434717657293912020615842268611759 
 
 
// includes
#include "ThreadingModel.h"
#include "detail/FunctionTraits.h"

#include <algorithm>
#include <initializer_list>
#include <list>
#include <memory>
#include <mutex>
#include <numeric>
#include <utility>
#include <vector>


namespace cctools {
 
 
/**
 * @class HashMapT
 * @brief HashMapT is a queue that allows access to its elements
 *        according to specified threading policy
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash = std::hash<Key>, typename... AdditionalPolicyArgs>
class HashMapT : public ThreadingPolicy<HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>,
                                        AdditionalPolicyArgs...> {

  // ---------------------------------------------------
  // SFINAE aliases
  template <typename F>
  using requiresReturnValue = std::enable_if_t<!std::is_same_v<typename detail::FunctionTraits<F>::result_type, void>>;
  template <typename F>
  using requiresVoid = std::enable_if_t<std::is_same_v<typename detail::FunctionTraits<F>::result_type, void>>;
  template <typename T>
  using requiresDefaultConstructible = std::enable_if_t<std::is_default_constructible_v<T>>;
  template <typename T>
  using requiresNonDefaultConstructible = std::enable_if_t<!std::is_default_constructible_v<T>>;

  using threadingPolicy = ThreadingPolicy<HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>,
                                         AdditionalPolicyArgs...>;

  // import the locking methods of the threading policy
  using threadingPolicy::lock;
  using threadingPolicy::lockUnique;
  using threadingPolicy::lockShared;

public:

  class Bucket;

  // ---------------------------------------------------
  // private types
  using bucketContainer = std::vector<std::unique_ptr<Bucket>>;

public:

  // ---------------------------------------------------
  // public types
  using key_type    = Key;
  using mapped_type = Value;
  using value_type  = std::pair<key_type const, mapped_type>;
  using hasher      = Hash;
  using size_type   = std::size_t;

  // ---------------------------------------------------
  // special member functions
  HashMapT       (size_type nBuckets = 19ULL, hasher const& hash = hasher());
  HashMapT       (std::initializer_list<value_type> valueList, size_type nBuckets = 19ULL,
                  hasher const& hash = hasher());
  HashMapT       (HashMapT const& src) = delete;
  HashMapT       (HashMapT&& src);
  auto operator= (HashMapT const& src) -> HashMapT& = delete;
  auto operator= (HashMapT&& src) -> HashMapT&;
  ~HashMapT      () = default;

  // ---------------------------------------------------
  // public methods
  [[nodiscard]] auto size     () const -> size_type;
  [[nodiscard]] auto empty    () const -> bool;
  [[nodiscard]] auto contains (key_type const&) const -> bool;

  template <typename = requiresDefaultConstructible<mapped_type>>
  auto operator[]             (key_type const& key) -> mapped_type&;
  template <typename = requiresNonDefaultConstructible<mapped_type>, typename = void>
  auto operator[]             (key_type const& key) -> mapped_type&;
  auto at                     (key_type const& key) -> mapped_type&;
  auto insertOrReplace        (key_type const& key, mapped_type const& value);
  auto insertOrReplace        (key_type const& key, mapped_type&& value);
  template <typename... Args>
  auto emplace                (key_type const& key, Args&&... args) -> size_type;
  auto erase                  (key_type const& key) -> size_type;
  template <typename F>
  auto forEach                (F&& f) -> F&&;
  template <typename F>
  auto forEachValue           (F&& f) -> F&&;
  template <typename F>
  auto forEachValue           (F&& f) const -> F&&;
  template <typename F, typename = requiresReturnValue<F>>
  auto invoke                 (key_type const& key, F&& f) -> typename detail::FunctionTraits<F>::result_type;
  template <typename F, typename = requiresVoid<F>>
  void invoke                 (key_type const& key, F&& f);

private:

  // ---------------------------------------------------
  // private data
  size_type       numBuckets;
  hasher          elemHash;
  bucketContainer buckets;

  // ---------------------------------------------------
  // auxiliary methods
  auto findBucket    (key_type const&) -> Bucket&;
  auto findBucket    (key_type const&) const -> Bucket const&;
  auto bucketIndex   (key_type const& key) const -> size_type;
  void setupBuckets  ();
  void moveFromOther (HashMapT&& src);
};


/**
 * @class HashMapT::Bucket
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
class HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket
  : public ThreadingPolicy<Bucket, AdditionalPolicyArgs...> {

  using hashmapType     = HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>;
  using threadingPolicy = ThreadingPolicy<Bucket, AdditionalPolicyArgs...>;

  // import the locking methods of the threading policy
  using threadingPolicy::lock;
  using threadingPolicy::lockUnique;
  using threadingPolicy::lockShared;

public:

  // ---------------------------------------------------
  // public types
  using key_type         = typename hashmapType::key_type;
  using mapped_type      = typename hashmapType::mapped_type;
  using value_type       = typename hashmapType::value_type;
  using dataContainer    = std::list<value_type>;
  using size_type        = typename dataContainer::size_type;
  using iterator         = typename dataContainer::iterator;
  using const_iterator   = typename dataContainer::const_iterator;
  using threading_policy = ThreadingPolicy<Bucket, AdditionalPolicyArgs...>;

  // ---------------------------------------------------
  // public api
  auto size            () const -> size_type;
  auto get             (key_type const& key) -> mapped_type&;
  auto getOrThrow      (key_type const& key) -> mapped_type&;
  auto contains        (key_type const& key) const -> bool;
  auto insertOrReplace (key_type const& key, mapped_type const& value) -> size_type;
  auto insertOrReplace (key_type const& key, mapped_type&& value) -> size_type;
  template <typename... Args>
  auto emplace         (key_type const& key, Args&&... args) -> size_type;
  auto removeMapping   (key_type const& key) -> size_type;
  template <typename F>
  auto forEach         (F&& f) -> F&&;
  template <typename F>
  auto forEachValue    (F&& f) -> F&&;
  template <typename F>
  auto forEachValue    (F&& f) const -> F&&;
  template <typename F, typename = hashmapType::template requiresReturnValue<F>>
  auto invoke          (key_type const& key, F&& f) -> typename detail::FunctionTraits<F>::result_type;
  template <typename F, typename = hashmapType::template requiresVoid<F>>
  void invoke          (key_type const& key, F&& f);

private:

  // ---------------------------------------------------
  // private data
  dataContainer data;

  // ---------------------------------------------------
  // auxiliary methods
  auto find           (key_type const&) -> iterator;
  auto find           (key_type const&) const -> const_iterator;
  auto getOrThrowImpl (key_type const& key) -> mapped_type&;
};


/**
 * @brief Returns the number of elements.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::size() const -> size_type
{
  // ------- begin critical section ------- //
  auto lck = lockShared();
  return data.size();
}

/**
 * @brief Returns a reference to an element with a key equal to key. If no such element exists,
 *        a new element with the specified key is inserted.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, 
          typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::get(key_type const& key) -> mapped_type&
{
  // ------- begin critical section ------- //
  auto lck = lockUnique();

  if(auto val = find(key); data.end() != val)
  {
    return val->second;
  }

  data.emplace_back(std::make_pair(key, mapped_type{}));
  return data.back().second;
}

/**
 * @brief Returns a reference to an element with a key equal to key. If no such element exists,
 *        a std::out_of_range error is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::getOrThrow(key_type const& key) -> mapped_type&
{
  // ------- begin critical section ------- //
  auto lck = lockShared();
  return getOrThrowImpl(key);
}

/**
 * @brief Checks if an element with a given key is present.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::contains(key_type const& key) const -> bool
{
  // ------- begin critical section ------- //
  auto lck = lockShared();
  return find(key) != data.end();
}

/**
 * @brief Inserts a new element into the bucket. If there is already an element with the given key,
 *        its value is replaced by the given value. Otherwise, a new element is constructed and inserted.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::insertOrReplace(key_type const& key, mapped_type const& value)
  -> size_type
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();

    auto const val = find(key);
    if(val == data.end())
    {
      data.push_back(std::make_pair(key, value));
      return 1ULL;
    }

    val->second = value;
  }
  // -------- end critical section -------- //

  return 0ULL;
}

/**
 * @brief Inserts a new element into the bucket. If there is already an element with the given key,
 *        its value is replaced by the given value. Otherwise, a new element is constructed and inserted.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::insertOrReplace(key_type const& key, mapped_type&& value)
  -> size_type
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();

    auto const val = find(key);
    if(val == data.end())
    {
      data.push_back(std::make_pair(key, std::move(value)));
      return 1ULL;
    }

    val->second = std::move(value);
  }
  // -------- end critical section -------- //

  return 0ULL;
}

/**
 * @brief  Inserts a new element in the bucket constructed in place if no such element with a key
 *         equal to key can be found. Otherwise, the existing element is replaced by a new constructed element
 * @params args... the parameters forwarded to the constructor of the new element.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename... Args>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::emplace(key_type const& key, Args&&... args) -> size_type
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();
    
    if(auto val = find(key); val != data.end())
    {
      val->second = mapped_type{std::forward<Args>(args)...};
      return 0ULL;
    }
    
    data.emplace_back(
      std::piecewise_construct,
      std::make_tuple(key),
      std::make_tuple(std::forward<Args>(args)...)
    );
  }
  // -------- end critical section -------- //

  return 1ULL;
}

/**
 * @brief Searches for an element with a key equal to key and erases it.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::removeMapping(key_type const& key) -> size_type
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();

    if(auto const val = find(key); val != data.end())
    {
      data.erase(val);
      return 1ULL;
    }
  }
  // -------- end critical section -------- //

  return 0ULL;
}

/**
 * @brief Applies a given function to every element in the bucket.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::forEach(F&& f) -> F&&
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();

    for(auto& [key, val] : data)
    {
      std::forward<F>(f)(val);
    }
  }
  // -------- end critical section -------- //

  return std::forward<F>(f);
}

/**
 * @brief Applies a given function to every key-value pair in the bucket.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::forEachValue(F&& f) -> F&&
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();

    for(auto& entry : data)
    {
      std::forward<F>(f)(entry);
    }
  }
  // -------- end critical section -------- //

  return std::forward<F>(f);
}

/**
 * @brief Applies a given function to every key-value pair in the bucket.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::forEachValue(F&& f) const -> F&&
{
  // ------- begin critical section ------- //
  {
    auto lck = lockUnique();

    for(auto& entry : data)
    {
      std::forward<F>(f)(entry);
    }
  }
  // -------- end critical section -------- //

  return std::forward<F>(f);
}

/**
 * @brief Applies a given function to the element with a specified key. If no such element exists,
 *        an exception is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F, typename>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::invoke(key_type const& key, F&& f) ->
  typename detail::FunctionTraits<F>::result_type
{
  // ------- begin critical section ------- //
  auto lck = lockUnique();
  return std::forward<F>(f)(getOrThrowImpl(key));
}

/**
 * @brief Applies a given function to the element with a specified key. If no such element exists,
 *        an exception is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, typename Hash, typename... AdditionalPolicyArgs>
template <typename F, typename>
inline void HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::invoke(key_type const& key, F&& f)
{
  // ------- begin critical section ------- //
  auto lck = lockUnique();
  std::forward<F>(f)(getOrThrowImpl(key));
}

/**
 * @brief Returns an iterator to the first element with a specified key.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::find(key_type const& key) -> iterator
{
  return std::find_if(data.begin(), data.end(),
                      [&key](value_type const& v) { return v.first == key; });
}

/**
 * @brief Returns a const iterator to the first element with a specified key.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::find(key_type const& key) const -> const_iterator
{
  return std::find_if(data.begin(), data.end(),
                      [&key](value_type const& v) { return v.first == key; });
}

/**
 * @brief Returns a reference to an element with a key equal to key. If no such element exists,
 *        a std::out_of_range error is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::Bucket::getOrThrowImpl(key_type const& key) -> mapped_type&
{
  if(auto val = find(key); data.end() != val)
  {
    return val->second;
  }

  throw std::out_of_range("entry with given key not found");
}


/**
 * @brief Constructor.
 * @param nBuckets the number of buckets used for the has map. Default is 19. It's a good
 *        choice to use a prime number.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::HashMapT(size_type nBuckets, hasher const& hash)
  : numBuckets {nBuckets}
  , elemHash   {hash}
{
  setupBuckets();
}

/**
 * @brief Constructs the map with the contents of the given initializer list.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::HashMapT(std::initializer_list<value_type> valueList,
                                                                               size_type nBuckets, hasher const& hash)
  : numBuckets {nBuckets}
  , elemHash   {hash}
{
  setupBuckets();
  for(auto& [key, value] : valueList)
  {
    insertOrReplace(key, std::move(value));
  }
}

/**
 * @brief Move Constructor.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::HashMapT(HashMapT&& src)
{
  // ------- begin critical section ------- //
  auto lckThis  = lock();
  auto lckOther = src.lock();

  moveFromOther(std::move(src));
}

/**
 * @brief Move Assignment.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::operator=(HashMapT&& src) -> HashMapT&
{
  // ------- begin critical section ------- //
  auto lckThis  = lock();
  auto lckOther = src.lock();

  moveFromOther(std::move(src));

  return *this;
}

/**
 * @brief Returns the number of elements stored in the map.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline [[nodiscard]] auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::size() const -> size_type
{
  // ------- begin critical section ------- //
  auto lck = lockShared();
  return std::accumulate(buckets.begin(), buckets.end(), 0ULL,
                         [](size_type n, auto const& b) { return n + b->size(); });
}

/**
 * @brief Checks if the hash map is empty.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline [[nodiscard]] auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::empty() const -> bool
{
  // ------- begin critical section ------- //
  auto lck = lockShared();
  return size() == 0ULL;
}

/**
 * @brief Checks if the container stores an element with a given key.
 * @param key the key value of the element to search.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline [[nodiscard]] auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::contains(key_type const& key) const -> bool
{
  // ------- begin critical section ------- //
  auto lck = lockShared();
  return findBucket(key).contains(key);
}

/**
 * @brief Returns a reference to an element with a key equal to key. If no such element exists,
 *        a new element with the specified key is inserted and a reference to it is returned.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, 
          typename Hash, typename... AdditionalPolicyArgs>
template <typename>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::operator[](key_type const& key) -> mapped_type&
{
  return findBucket(key).get(key);
}

/**
 * @brief Returns a reference to an element with a key equal to key. If no such element exists,
 *        a std::out_of_range error is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename, typename>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::operator[](key_type const& key) -> mapped_type&
{
  return findBucket(key).getOrThrow(key);
}

/**
 * @brief Returns a reference to an element with a key equal to key. If no such element exists,
 *        a std::out_of_range error is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::at(key_type const& key) -> mapped_type&
{
  return findBucket(key).getOrThrow(key);
}

/**
 * @brief Inserts a new element into the container. If there is already an element with the given key,
 *        its value is replaced by the given value. Otherwise, a new element is constructed and inserted.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::insertOrReplace(key_type const& key,
                                                                                                  mapped_type const& value)
{
  findBucket(key).insertOrReplace(key, value);
}

/**
 * @brief Inserts a new element into the container. If there is already an element with the given key,
 *        its value is replaced by the given value. Otherwise, a new element is constructed and inserted.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::insertOrReplace(key_type const& key,
                                                                                                  mapped_type&& value)
{
  findBucket(key).insertOrReplace(key, std::move(value));
}

/**
 * @brief  Inserts a new element constructed in place if no such element with a key
 *         equal to key can be found. Otherwise, the existing element is replaced by a new constructed element.
 * @params args... the parameters forwarded to the constructor of the new element.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename... Args>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::emplace(key_type const& key, Args&&... args) -> size_type
{
  return findBucket(key).emplace(key, std::forward<Args>(args)...);
}

/**
 * @brief  Removes all elements with a given key.
 * @return the number of removed elements.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::erase(key_type const& key) -> size_type
{
  return findBucket(key).removeMapping(key);
}

/**
 * @brief Applies a given function to every element in the map.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::forEach(F&& f) -> F&&
{
  for(auto& bucket : buckets)
  {
    bucket->forEach(std::forward<F>(f));
  }

  return std::forward<F>(f);
}

/**
 * @brief Applies a given function to every key-value pair in the map.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::forEachValue(F&& f) -> F&&
{
  for(auto& bucket : buckets)
  {
    bucket->forEachValue(std::forward<F>(f));
  }

  return std::forward<F>(f);
}

/**
 * @brief Applies a given function to every key-value pair in the map.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F>
auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::forEachValue(F&& f) const -> F&&
{
  for(auto& bucket : buckets)
  {
    bucket->forEachValue(std::forward<F>(f));
  }

  return std::forward<F>(f);
}

/**
 * @brief Applies a given function to the element with a specified key. If no such element exists,
 *        an exception is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F, typename>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::invoke(key_type const& key, F&& f)
  -> typename detail::FunctionTraits<F>::result_type
{
  return findBucket(key).invoke(key, std::forward<F>(f));
}

/**
 * @brief Applies a given function to the element with a specified key. If no such element exists,
 *        an exception is thrown.
 * @throw std::out_of_range
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
template <typename F, typename>
inline void HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::invoke(key_type const& key, F&& f)
{
  findBucket(key).invoke(key, std::forward<F>(f));
}

/**
 * @brief Returns a reference to the bucket for given key.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::findBucket(key_type const& key) -> Bucket&
{
  return *buckets[bucketIndex(key)];
}

/**
 * @brief Returns a const reference to the bucket for given key.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, 
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::findBucket(key_type const& key) const -> Bucket const&
{
  return *buckets[bucketIndex(key)];
}

/**
 * @brief Calculates the bucket index for a given key.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy,
          typename Hash, typename... AdditionalPolicyArgs>
inline auto HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::bucketIndex(key_type const& key) const -> size_type
{
  return elemHash(key) % numBuckets;
}

/**
 * @brief Constructs the bucket container with empty buckets.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, typename Hash, typename... AdditionalPolicyArgs>
void HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::setupBuckets()
{
  buckets.reserve(numBuckets);
  for(std::size_t i{}; i < numBuckets; ++i)
  {
    buckets.push_back(std::make_unique<Bucket>());
  }
}

/**
 * @brief Performs element-wise move copy.
 */
template <typename Key, typename Value, template <typename...> class ThreadingPolicy, typename Hash, typename... AdditionalPolicyArgs>
void HashMapT<Key, Value, ThreadingPolicy, Hash, AdditionalPolicyArgs...>::moveFromOther(HashMapT&& src)
{
  numBuckets = std::move(src.numBuckets);
  elemHash   = std::move(src.elemHash);
  buckets    = std::move(src.buckets);
}


/**
 * @brief convenience template alias for a hash map without locking.
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>>
using HashMap = HashMapT<Key, Value, SingleThreaded, Hash>;

/**
 * @brief convenience template alias for a hash map offering threadsafe access to its elements.
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Mutex = std::shared_mutex>
using ThreadsafeHashMap = HashMapT<Key, Value, ObjectLevelLockable, Hash, Mutex>;


}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // HASHMAP_H_3740194082434717657293912020615842268611759 
