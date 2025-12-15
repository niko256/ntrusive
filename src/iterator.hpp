#pragma once

#include <cstddef>
#include <iterator>

template <typename T, typename Hook>
class ListIterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    constexpr ListIterator() noexcept = default;

    /* @param hook Pointer to the node (may be sentinel) */
    constexpr explicit ListIterator(Hook* hook) noexcept;

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

    constexpr Hook* hook() const noexcept;

  private:
    Hook* current_{nullptr};
};

template <typename T, typename Hook>
using ConstListIterator = ListIterator<const T, const Hook>;
