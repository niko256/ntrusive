#pragma once

#include <cstddef>
#include <iterator>

template <typename T, typename Node>
class ListIterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    constexpr ListIterator() noexcept = default;

    /* @param hook Pointer to the node (may be sentinel) */
    constexpr explicit ListIterator(Node* hook) noexcept;

    [[nodiscard]]
    constexpr reference operator*() const noexcept;

    [[nodiscard]]
    constexpr pointer operator->() const noexcept;

    constexpr ListIterator& operator++() noexcept;

    constexpr ListIterator operator++(int) noexcept;

    constexpr ListIterator& operator--() noexcept;

    constexpr ListIterator operator--(int) noexcept;

    constexpr bool operator==(const ListIterator* other) const noexcept;

    constexpr bool operator!=(const ListIterator* other) const noexcept;

    [[nodiscard]]
    constexpr Node* node() const noexcept;

  private:
    Node* current_{nullptr};
};

template <typename T, typename Node>
using ConstListIterator = ListIterator<const T, const Node>;

/* ------------------------------------------------------------------- */

template <typename T, typename Node>
constexpr ListIterator<T, Node>::ListIterator(Node* hook) noexcept : current_(hook) {}

template <typename T, typename Node>
constexpr typename ListIterator<T, Node>::reference
ListIterator<T, Node>::operator*() const noexcept {
    assert(current_ != nullptr);
    return *static_cast<pointer>(current_);
}

template <typename T, typename Node>
constexpr typename ListIterator<T, Node>::pointer
ListIterator<T, Node>::operator->() const noexcept {
    assert(current_ != nullptr);
    return static_cast<pointer>(current_);
}

template <typename T, typename Node>
constexpr ListIterator<T, Node>& ListIterator<T, Node>::operator++() noexcept {
    current_ = current_->next_node();
    return *this;
}

template <typename T, typename Node>
constexpr ListIterator<T, Node>& ListIterator<T, Node>::operator--() noexcept {
    current_ = current_->prev_node();
    return *this;
}

template <typename T, typename Node>
constexpr ListIterator<T, Node> ListIterator<T, Node>::operator++(int) noexcept {
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T, typename Node>
constexpr ListIterator<T, Node> ListIterator<T, Node>::operator--(int) noexcept {
    auto tmp = *this;
    --(*this);
    return tmp;
}

template <typename T, typename Node>
constexpr bool ListIterator<T, Node>::operator==(const ListIterator* other) const noexcept {
    return current_ == other;
}

template <typename T, typename Node>
constexpr bool ListIterator<T, Node>::operator!=(const ListIterator* other) const noexcept {
    return current_ != other;
}

template <typename T, typename Node>
constexpr Node* ListIterator<T, Node>::node() const noexcept {
    return current_;
}

/* ------------------------------------------------------------------- */

/* ...Deduction guide... */
template <typename Node>
ListIterator(Node*) -> ListIterator<typename Node::value_type, Node>;

/* ------------------------------------------------------------------- */
