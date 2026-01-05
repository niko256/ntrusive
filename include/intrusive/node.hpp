#pragma once

#include "policies.hpp"
#include <cassert>

/**
 * @section DANGER_AND_SAFETY
 * The biggest risk in intrusive containers is the 'dangling pointer'
 * If a Node is destroyed while still being linked in a list
 * => the list becomes corrupted => undefined behavior
 *
 * To avoid this problem, this class provides different safety policies.
 */


/**
 * @brief Intrusive List Node.
 * Inherit from this class to make your object linkable.
 *
 */
template <typename Tag = void, LinkPolicy Policy = TrackingPolicy, typename Handler = AutoUnlinkHandler>
class IntrusiveListNode {
  public:
    using tag_type = Tag;
    using policy_type = Policy;
    using destruction_handler = Handler;

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
    constexpr void set_linked() noexcept;

    void link_between(IntrusiveListNode* prev, IntrusiveListNode* next) noexcept;

    /* ------------------------------------------------------------------- */

    [[nodiscard]]
    constexpr auto next_node() const noexcept -> IntrusiveListNode*;

    [[nodiscard]]
    constexpr auto prev_node() const noexcept -> IntrusiveListNode*;

    constexpr void set_next(IntrusiveListNode* n) noexcept;

    constexpr void set_prev(IntrusiveListNode* p) noexcept;

    /* ------------------------------------------------------------------- */

  private:
    IntrusiveListNode* prev_{nullptr};
    IntrusiveListNode* next_{nullptr};

    [[no_unique_address]]
    typename Policy::State state_;

    template <typename, typename>
    friend class IntrusiveList;

    template <typename, typename>
    friend class ListIterator;
};

/* ------------------------------------------------------------------- */

template <typename Tag, LinkPolicy Policy, typename Handler>
IntrusiveListNode<Tag, Policy, Handler>::~IntrusiveListNode() {
    Handler::on_destroy(*this);
}

template <typename Tag, LinkPolicy Policy, typename Handler>
constexpr bool IntrusiveListNode<Tag, Policy, Handler>::is_linked() const noexcept {
    return Policy::is_linked(state_);
}

template <typename Tag, LinkPolicy Policy, typename Handler>
void IntrusiveListNode<Tag, Policy, Handler>::unlink() noexcept {
    assert(is_linked() && "Attempting to unlink element not in list!!");

    prev_->next_ = next_;
    next_->prev_ = prev_;

    prev_ = nullptr;
    next_ = nullptr;

    /* ................... */
    Policy::on_unlink(state_);
    /* ................... */
}

template <typename Tag, LinkPolicy Policy, typename Handler>
void IntrusiveListNode<Tag, Policy, Handler>::link_between(
    IntrusiveListNode* prev,
    IntrusiveListNode* next) noexcept {
    prev->next_ = this;
    next->prev_ = this;

    this->next_ = next;
    this->prev_ = prev;

    /* ................... */
    Policy::on_link(state_);
    /* ................... */
}

template <typename Tag, LinkPolicy Policy, typename Handler>
constexpr auto IntrusiveListNode<Tag, Policy, Handler>::next_node() const noexcept -> IntrusiveListNode<Tag, Policy, Handler>* {
    return next_;
}

template <typename Tag, LinkPolicy Policy, typename Handler>
constexpr auto IntrusiveListNode<Tag, Policy, Handler>::prev_node() const noexcept -> IntrusiveListNode<Tag, Policy, Handler>* {
    return prev_;
}

template <typename Tag, LinkPolicy Policy, typename Handler>
constexpr void IntrusiveListNode<Tag, Policy, Handler>::set_next(IntrusiveListNode* n) noexcept {
    next_ = n;
}

template <typename Tag, LinkPolicy Policy, typename Handler>
constexpr void IntrusiveListNode<Tag, Policy, Handler>::set_prev(IntrusiveListNode* p) noexcept {
    prev_ = p;
}
