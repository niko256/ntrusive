#pragma once

#include "iterator.hpp"
#include "node.hpp"
#include "policies.hpp"
#include <cassert>
#include <cstddef>
#include <type_traits>


/**
 *
 * @section PHILOSOPHY
 * Unlike standart containers (like std::list), an Intrusive List doesn't manage
 * memory lifetimes of the objects it stores. Instead, the linkage metadata (like 'prev' and 'next' fields)
 * are embedded directly within the stored object itself.
 *
 * This approach has significant advantages:
 *  |> Zero Allocations on push/pop operations
 *  |> Memory Locality
 *
 */

/* ------------------------------------------------------------------- */

/**
 * @brief Concept to verify T inherits from IntrusiveListNode with specific Tag.
 */
template <typename T, typename Tag>
concept HasNodeWithTag = std::is_base_of_v<
    IntrusiveListNode<Tag, typename T::policy_type, typename T::destruction_handler>,
    T>;

template <typename T, typename Tag = void>
    requires HasNodeWithTag<T, Tag>
class IntrusiveList {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using node_type = IntrusiveListNode<Tag, typename T::policy_type, typename T::destruction_handler>;
    using sentinel_type = IntrusiveListNode<Tag, NoTrackPolicy, NoOpHandler>;

    using iterator = ListIterator<T, IntrusiveListNode<Tag>>;
    using const_iterator = ConstListIterator<T, IntrusiveListNode<Tag>>;

    using IntrusiveListNode = IntrusiveListNode<Tag>;

    /* ------------------------------------------------------------------- */

  public:
    IntrusiveList() noexcept;

    ~IntrusiveList();

    IntrusiveList(const IntrusiveList&) = delete;
    IntrusiveList& operator=(const IntrusiveList&) = delete;

    IntrusiveList(IntrusiveList&&) noexcept = delete;
    IntrusiveList& operator=(IntrusiveList&&) noexcept = delete;

  private:
    void init_sentinel() noexcept;

    /* ------------------------------------------------------------------- */
  public:
    void push_back(reference element) noexcept;

    void push_front(reference element) noexcept;

    auto insert(const_iterator pos, reference element) noexcept -> iterator;

    void pop_front() noexcept;

    void pop_back() noexcept;

    auto erase(const_iterator pos) noexcept -> iterator;

    auto erase_range(const_iterator first, const_iterator last) noexcept -> iterator;

    void clear() noexcept;

    void splice(const_iterator pos, IntrusiveList& other) noexcept;

    void splice_cell(const_iterator pos, IntrusiveList& other, const_iterator element) noexcept;

    void splice_range(const_iterator pos, IntrusiveList& other, const_iterator first, const_iterator last) noexcept;

    [[nodiscard]]
    auto try_pop_front() noexcept -> pointer;

    [[nodiscard]]
    auto try_pop_back() noexcept -> pointer;

    [[nodiscard]]
    auto extract_front(IntrusiveList& out, size_type max_cnt) noexcept -> size_type;

    static void remove(reference element) noexcept;

  private:
    void insert_after(sentinel_type* after, reference value) noexcept;

    void insert_before(sentinel_type* before, reference value) noexcept;

    /* ------------------------------------------------------------------- */

    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]]
    auto size() const noexcept -> size_type;

    [[nodiscard]]
    auto front() noexcept -> reference;

    [[nodiscard]]
    auto front() const noexcept -> const_reference;

    [[nodiscard]]
    auto back() noexcept -> reference;

    [[nodiscard]]
    auto back() const noexcept -> const_reference;

    /* ------------------------------------------------------------------- */

    [[nodiscard]]
    auto begin() noexcept -> iterator;

    [[nodiscard]]
    auto begin() const noexcept -> const_iterator;

    [[nodiscard]]
    auto cbegin() const noexcept -> const_iterator;

    [[nodiscard]]
    auto end() noexcept -> iterator;

    [[nodiscard]]
    auto end() const noexcept -> const_iterator;

    [[nodiscard]]
    auto cend() const noexcept -> const_iterator;

    /* ------------------------------------------------------------------- */

  private:
    mutable sentinel_type sentinel_;
};

/* ------------------------------------------------------------------- */

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
IntrusiveList<T, Tag>::IntrusiveList() noexcept {
    init_sentinel();
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
IntrusiveList<T, Tag>::~IntrusiveList() {
    /**
     * ...
     */
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::init_sentinel() noexcept {
    sentinel_.set_next(&sentinel_);
    sentinel_.set_prev(&sentinel_);
}

/* ------------------------------------------------------------------- */

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
bool IntrusiveList<T, Tag>::empty() const noexcept {
    return sentinel_.next_node() = &sentinel_;
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::size() const noexcept -> typename IntrusiveList<T, Tag>::size_type {
    size_type cnt = 0;

    for (auto it = begin(); it != end(); ++it) {
        ++cnt;
    }

    return cnt;
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::front() noexcept -> typename IntrusiveList<T, Tag>::reference {
    assert(!empty() && "front() called on empty list...");
    return *begin();
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::front() const noexcept -> typename IntrusiveList<T, Tag>::const_reference {
    assert(!empty() && "front() called on empty list...");
    return *begin();
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::back() noexcept -> typename IntrusiveList<T, Tag>::reference {
    assert(!empty() && "back() called on empty list...");
    /**
     * end() points to -> sentinel => sentinel.prev_ is the last element
     */
    return *(--end());
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::back() const noexcept -> typename IntrusiveList<T, Tag>::const_reference {
    assert(!empty() && "back() called on empty list...");
    return *(--end());
}

/* ------------------------------------------------------------------- */


template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::begin() noexcept -> typename IntrusiveList<T, Tag>::iterator {
    /**
     * First element is sentinel.next
     * If list is empty, sentinel.next == &sentinel, so begin() == end()
     */
    return iterator(static_cast<node_type*>(sentinel_.next_node()));
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::begin() const noexcept -> typename IntrusiveList<T, Tag>::const_iterator {
    return iterator(static_cast<node_type*>(sentinel_.next_node()));
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::cbegin() const noexcept -> typename IntrusiveList<T, Tag>::const_iterator {
    return begin();
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::end() noexcept -> typename IntrusiveList<T, Tag>::iterator {
    /**
     * end() points to the sentinel
     * but since sentinel_type and node_type are not the same,
     * we use reinterpret_cast in order to have the same mem layout.
     * They differs only in policy and handler which don't affect to layout
     */
    return reinterpret_cast<node_type*>(&sentinel_);
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::end() const noexcept -> typename IntrusiveList<T, Tag>::const_iterator {
    return reinterpret_cast<const node_type*>(&sentinel_);
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::cend() const noexcept -> typename IntrusiveList<T, Tag>::const_iterator {
    return end();
}

/* ------------------------------------------------------------------- */

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::insert_after(sentinel_type* after, reference value) noexcept {
    auto& node = value;

    assert(!node.is_linked() && "Element already in a list!!");

    /**
     * Before : after <-> next
     * After : afetr <-> node <-> next
     */

    auto* next = after->next_node();
    node.link_between(after, next);
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::insert_before(sentinel_type* before, reference value) noexcept {
    auto& node = value;

    assert(!node.is_linked() && "Element already in a list!!");

    /**
     * Before : prev <-> before
     * After : prev <-> node <-> before
     */

    auto* prev = before->prev_node();
    node.link_between(prev, before);
}

/* ------------------------------------------------------------------- */

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::push_back(reference element) noexcept {
    /**
     * Before : ... <-> last <-> sentinel
     * After : ... <-> last' <-> element <-> sentinel
     */
    insert_before(&sentinel_, element);
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::push_front(reference element) noexcept {
    /**
     * Before : sentinel <-> first <-> ...
     * After : sentinel <-> element <-> first' <-> ...
     */
    insert_after(&sentinel_, element);
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::insert(
    const_iterator pos, reference element) noexcept -> typename IntrusiveList<T, Tag>::iterator {
    /**
     * insert before pos (because inserted node position will become new pos)
     *
     * But we need to convert const iterator to mutable node pointer
     */
    node_type* node_on_pos = const_cast<node_type*>(pos.node());
    insert_before(reinterpret_cast<sentinel_type*>(node_on_pos), element);

    return iterator(static_cast<node_type*>(&static_cast<node_type&>(element)));
}

/* ------------------------------------------------------------------- */

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::pop_front() noexcept {
    assert(!empty() && "pop_front() on empty list!!");

    /**
     * unlink the first element
     */
    sentinel_.next_node()->unlink();
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::pop_back() noexcept {
    assert(!empty() && "pop_front() on empty list!!");

    /**
     * unlink the last element
     */
    sentinel_.prev_node()->unlink();
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::erase(const_iterator pos) noexcept -> typename IntrusiveList<T, Tag>::iterator {
    assert(pos != end() && "Cannot erase sentinel...");

    node_type* node = static_cast<node_type*>(pos.node());
    node_type* next_node = static_cast<node_type*>(node->next_node());

    node->unlink();

    /* return the iterator to the element that followed the erased one */
    return iterator(next_node);
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
auto IntrusiveList<T, Tag>::erase_range(const_iterator first, const_iterator last) noexcept -> typename IntrusiveList<T, Tag>::iterator {
    while (first != last) {
        erase(first);
    }

    return iterator(const_cast<node_type*>(last.node()));
}

template <typename T, typename Tag>
    requires HasNodeWithTag<T, Tag>
void IntrusiveList<T, Tag>::clear() noexcept {
    while (!empty()) {
        pop_front();
    }
}
