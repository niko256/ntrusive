#pragma once

#include "iterator.hpp"
#include <cstdint>
#include <type_traits>

enum class LinkMode : uint8_t {
    NoLink,
    Link,
    AutoUnlink,
};

template <typename tag = void, LinkMode Mode = LinkMode::Link>
class IntrusiveListNode {
  public:
    static constexpr LinkMode link_mode = Mode;

    constexpr IntrusiveListNode() noexcept = default;

    ~IntrusiveListNode();

    /* non-copyable */
    IntrusiveListNode(const IntrusiveListNode&) = delete;
    IntrusiveListNode& operator=(const IntrusiveListNode&) = delete;

    /* non-moveble */
    IntrusiveListNode(IntrusiveListNode&&) noexcept = delete;
    IntrusiveListNode& operator=(IntrusiveListNode&&) noexcept = delete;

    [[nodiscard]]
    constexpr bool is_linked() const noexcept;

    void unlink() noexcept;

  private:
    constexpr void reset_links() noexcept;

    constexpr void set_linked() noexcept;

    constexpr void link_between(IntrusiveListNode* prev, IntrusiveListNode* next) noexcept;


    [[nodiscard]]
    constexpr IntrusiveListNode* next_node() const noexcept;

    [[nodiscard]]
    constexpr IntrusiveListNode* prev_node() const noexcept;

    constexpr void set_next() noexcept;

    constexpr void set_prev() noexcept;

  private:
    IntrusiveListNode* prev_{nullptr};
    IntrusiveListNode* next_{nullptr};

    [[no_unique_address]]
    std::conditional_t<Mode == LinkMode::NoLink, std::false_type, bool> is_linked_{};

    template <typename, typename>
    friend class IntrusiveList;

    template <typename, typename>
    friend class ListIterator;
};
