#pragma once

#include "base_node.hpp"
#include "ntrusive/node.hpp"
#include <cstddef>
#include <iterator>

/**
 * @brief Bidirectional Iterator for IntrusiveList<...>
 */
template <typename T, typename Tag>
class ListIterator {
  public:
    /*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    using node_type = IntrusiveListNode<Tag,
                                        typename value_type::policy_type,
                                        typename value_type::destruction_handler>;

    /*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

    constexpr ListIterator() noexcept = default;

    /* @param hook Pointer to the node (may be sentinel) */
    constexpr explicit ListIterator(NodeBase* hook) noexcept;

    [[nodiscard]]
    constexpr reference operator*() const noexcept;

    [[nodiscard]]
    constexpr pointer operator->() const noexcept;

    constexpr ListIterator& operator++() noexcept;

    constexpr ListIterator operator++(int) noexcept;

    constexpr ListIterator& operator--() noexcept;

    constexpr ListIterator operator--(int) noexcept;

    constexpr bool operator==(const ListIterator& other) const noexcept;

    constexpr bool operator!=(const ListIterator& other) const noexcept;

    operator ListIterator<const value_type, Tag>() const noexcept;

    [[nodiscard]] constexpr NodeBase* base() const noexcept;

  private:
    NodeBase* current_{nullptr};
};

template <typename T, typename Tag>
using ConstListIterator = ListIterator<const T, const Tag>;

/*---*---*---*---*---*---*---*---IMPL---*---*---*---*---*---*---*---*---*/

template <typename T, typename Tag>
constexpr ListIterator<T, Tag>::ListIterator(NodeBase* hook) noexcept
    : current_(hook) {}

template <typename T, typename Tag>
constexpr typename ListIterator<T, Tag>::reference
ListIterator<T, Tag>::operator*() const noexcept {

    /**
     *
     * NodeBase* --> node_type* --> T*
     *
     */

    return *static_cast<pointer>(static_cast<node_type*>(current_));
}

template <typename T, typename Tag>
constexpr typename ListIterator<T, Tag>::pointer
ListIterator<T, Tag>::operator->() const noexcept {
    return static_cast<pointer>(static_cast<node_type*>(current_));
}

template <typename T, typename Tag>
constexpr ListIterator<T, Tag>& ListIterator<T, Tag>::operator++() noexcept {
    current_ = current_->next_node();
    return *this;
}

template <typename T, typename Tag>
constexpr ListIterator<T, Tag>& ListIterator<T, Tag>::operator--() noexcept {
    current_ = current_->prev_node();
    return *this;
}

template <typename T, typename Tag>
constexpr ListIterator<T, Tag> ListIterator<T, Tag>::operator++(int) noexcept {
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T, typename Tag>
constexpr ListIterator<T, Tag> ListIterator<T, Tag>::operator--(int) noexcept {
    auto tmp = *this;
    --(*this);
    return tmp;
}

template <typename T, typename Tag>
constexpr bool ListIterator<T, Tag>::operator==(const ListIterator& other) const noexcept {
    return current_ == other.current_;
}

template <typename T, typename Tag>
constexpr bool ListIterator<T, Tag>::operator!=(const ListIterator& other) const noexcept {
    return current_ != other.current_;
}

template <typename T, typename Tag>
ListIterator<T, Tag>::operator ListIterator<const value_type, Tag>() const noexcept {
    return ListIterator<const value_type, Tag>{current_};
}

template <typename T, typename Tag>
constexpr NodeBase* ListIterator<T, Tag>::base() const noexcept {
    return current_;
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
