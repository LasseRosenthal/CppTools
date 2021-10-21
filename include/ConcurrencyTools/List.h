/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    List.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    30.06.2021 
 */ 
 
#ifndef LIST_H_28449361014277151491309631315139371239319961 
#define LIST_H_28449361014277151491309631315139371239319961 
 
 
// includes
#include "ConcurrencyTools/ThreadingModel.h"

#include <cstddef>
#include <memory>


namespace cctools {


/**
 * @class ListT
 * @brief
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
class ListT {

public:

  // ---------------------------------------------------
  // public types
  using value_type = T;
  using size_type  = std::size_t;

  // ---------------------------------------------------
  // construction
  ListT ();

  // ---------------------------------------------------
  // public api
  void push_front    (value_type const& value);
  void push_front    (value_type&& value);
  template <typename... ValueArgs>
  void emplace_front (ValueArgs&&... valueArgs);

  void push_back     (value_type const& value);
  void push_back     (value_type&& value);   

  template <typename Predicate>
  void remove_if     (Predicate&& p);

  template <typename F>
  void for_each      (F&& f);
  


private:

  struct Node;

  // ---------------------------------------------------
  // private data
  Node head;
  std::shared_ptr<Node> tail;

  // ---------------------------------------------------
  // auxiliary methods
  void insertNodeAtFront (std::shared_ptr<Node>&&);
  void insertNodeAtBack  (std::shared_ptr<Node>&&);

};


/**
 * @class ListT::Node
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
struct ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::Node
  : public ThreadingPolicy<Node, AdditionalPolicyArgs...> {

  friend class ListT;

  // ---------------------------------------------------
  // public types
  using value_type = T;
  using threadingPolicy = ThreadingPolicy<Node, AdditionalPolicyArgs...>;

  // ---------------------------------------------------
  // construction
  Node () = default;
  Node (value_type const& value);
  Node (value_type&& value);
  //template <typename... ValueArgs>
  //Node (ValueArgs&&...);

  // ---------------------------------------------------
  // data
  std::unique_ptr<value_type> data;
  std::shared_ptr<Node>       next;
  Node*                       prev {nullptr};
};


/**
 * @brief copy constructs a node from a given value.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::Node::Node(value_type const& value)
  : data {std::make_unique<value_type>(value)}
{}

/**
 * @brief move constructs a node from a given value.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::Node::Node(value_type&& value)
  : data {std::make_unique<value_type>(std::move(value))}
{}

/**
 * @brief Constructs a node element from a given set of constructor arguments
 *        for the value.
 */
//template <typename T, template <typename...> class ThreadingPolicy,
//          typename... AdditionalPolicyArgs>
//template <typename... ValueArgs>
//inline ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::Node::Node(ValueArgs&&... valueArgs)
//  : data {std::make_unique<value_type>(std::forward<ValueArgs>(valueArgs)...)}
//{}

/**
 * @brief Default constructor. Constructs the tail object
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::ListT()
  : tail {std::make_shared<Node>()}
{}

/**
 * @brief Inserts a copy of a given value at the front of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::push_front(value_type const& value)
{
  insertNodeAtFront(std::make_shared<Node>(value));
}

/**
 * @brief move-copies of a given value to the front of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::push_front(value_type&& value)
{
  insertNodeAtFront(std::make_shared<Node>(std::move(value)));
}

/**
 * @brief Constructs a new list element from a list of constructor arguments and
 *        and inserts it at the front of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
template <typename... ValueArgs>
inline void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::emplace_front(ValueArgs&&... valueArgs)
{
  insertNodeAtFront(std::make_shared<Node>(std::forward(valueArgs)...));
}



/**
 * @brief Inserts a copy of a given value at the end of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::push_back(value_type const& value)
{
  insertNodeAtBack(std::make_shared<Node>(value));
}

/**
 * @brief move-copies of a given value to the end of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
inline void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::push_back(value_type&& value)
{
  insertNodeAtBack(std::make_shared<Node>(std::move(value)));
}


/**
 * @brief Inserts a node at the front of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::insertNodeAtFront(std::shared_ptr<Node>&& newNode)
{
  auto lockHead = head.lock();
  newNode->prev = &head;
  if(head.next)
  {
    newNode->next = std::move(head.next);
    auto lockNext = newNode->next->lock();
    newNode->next->prev = newNode.get();
  }
  else
  {
    tail = newNode;
  }

  head.next = std::move(newNode);
}

/**
 * @brief Inserts a node at the end of the list.
 */
template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::insertNodeAtBack(std::shared_ptr<Node>&& newNode)
{
  auto lockTail = tail->lock();
  newNode->prev = tail.get();
  tail->next = newNode;

  if(!tail->prev)
  {
    auto lockHead = head.lock();
    head.next = newNode;
  }
  
  // the new node is always the tail
  tail = std::move(newNode);
}

template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
template <typename F>
void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::for_each(F&& f)
{
  Node* current = &head;
  auto lock = head.lockUnique();

  while(Node* next = current->next.get())
  {
    auto lockNext = next->lockUnique();
    lock.unlock();
    std::forward<F>(f)(*next->data);
    current = next;
    lock = std::move(lockNext);
  }
}

template <typename T, template <typename...> class ThreadingPolicy,
          typename... AdditionalPolicyArgs>
template <typename Predicate>
void ListT<T, ThreadingPolicy, AdditionalPolicyArgs...>::remove_if(Predicate&& pred)
{
  Node* current = &head;
  auto lock = head.lockUnique();

  Node* next = current->next.get();
  //  ... <-> current <-> next <-> ...
  while(next != nullptr)
  {
    auto lockNext = next->lockUnique();
    if(pred(*next->data))
    {
      std::shared_ptr<Node> oldNext = std::move(current->next);

      // remove next by updating current->next
      if(next->next)
      {
        current->next = std::move(next->next);

        auto lockNextNext = current->next->lockUnique();
        current->next->prev = current;
      }
      // we can unlock lockNext since next will be deleted when oldNext goes out of scope
      lockNext.unlock();
      // don't update current because we have to check the new next node.
    }
    else
    {
      lock.unlock();
      // update current to point the next node
      current = next;
      lock = std::move(lockNext);
    }

    next = current->next.get();
  }
}





/**
 * @brief convenience template alias for a list without locking.
 */
template <typename T>
using List = ListT<T, SingleThreaded>;

/**
 * @brief convenience template alias for a list threadsafe access to its elements.
 */
template <typename T, typename Mutex = std::shared_mutex>
using ThreadsafeList = ListT<T, ObjectLevelLockable, Mutex>;


}   // namespace cctools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // LIST_H_28449361014277151491309631315139371239319961 
