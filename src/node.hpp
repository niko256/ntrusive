#pragma once

#include <cstdint>
#include <type_traits>


/**
 * @section PHILOSOPHY
 * Unlike standart containers (like std::list), an Intrusive List doesn't manage
 * memory lifetimes of the objects it stores. Instead, the linkage metadata (like 'prev' and 'next' fields)
 * are embedded directly within the stored object itself.
 *
 * This approach has significant advantages:
 *  \ Zero Allocations
 *  \ Memory Locality
 *
 * @section DANGER_AND_SAFETY
 * The biggest risk in intrusive containers is the 'dangling pointer'
 * If a Node is destroyed while still being linked in a list
 * => the list becomes corrupted => undefined behavior
 *
 * To avoid this problem, this class provides three safety modes via the 'Mode' template parameter.
 */

/**
 * [...CONTRACT...]
 */
enum class LinkMode : uint8_t {
    /**
     * @brief 'Safe' Mode.
     * Throws a panic in the dtor if object dies while linked in the list.
     * Also this mode prevents adding an already-linked item to a list.
     */
    Link, /* default */

    /**
     * @brief RAII [Auto-Disconnect]
     * If the object is destroyed while it linked in a list, it automatically unlink itself,
     * and connect it's neighbors together.
     *
     * Ideal for objects with non-deterministic lifetimes.
     */
    AutoUnlink, /* safe and convenient */

    /**
     * @brief Raw Mode.
     * No state checking, no checks in the dtor.
     *
     * @warning USE ONLY when object lifetime strictly exceeds list lifetime.
     * User guarantees safety.
     */
    Raw, /* unsafe */
};

/**
 * @brief Intrusive List Node Hook.
 * Inherit from this class to make your object linkable.
 *
 * @param Tag Types descriminator.
 * @param Mode Safety level selector.
 */
template <typename Tag = void, LinkMode Mode = LinkMode::Link>
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

    /* ------------------------------------------------------------------- */

    [[nodiscard]]
    constexpr bool is_linked() const noexcept;

    void unlink() noexcept;

  private:
    constexpr void reset_links() noexcept;

    constexpr void set_linked() noexcept;

    constexpr void link_between(IntrusiveListNode* prev, IntrusiveListNode* next) noexcept;

    /* ------------------------------------------------------------------- */

    [[nodiscard]]
    constexpr IntrusiveListNode* next_node() const noexcept;

    [[nodiscard]]
    constexpr IntrusiveListNode* prev_node() const noexcept;

    constexpr void set_next() noexcept;

    constexpr void set_prev() noexcept;

    /* ------------------------------------------------------------------- */

  private:
    IntrusiveListNode* prev_{nullptr};
    IntrusiveListNode* next_{nullptr};

    [[no_unique_address]]
    std::conditional_t<Mode == LinkMode::Raw, std::false_type, bool> is_linked_{};

    template <typename, typename>
    friend class IntrusiveList;

    template <typename, typename>
    friend class ListIterator;
};

/* ------------------------------------------------------------------- */
