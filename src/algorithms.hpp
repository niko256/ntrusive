#pragma once

#include <cstddef>


template <typename Hook>
constexpr void init_sentinel(Hook* sentinel) noexcept;


template <typename Hook>
constexpr void insert_before(Hook* pos, Hook* node) noexcept;


template <typename Hook>
constexpr void insert_after(Hook* pos, Hook* node) noexcept;


template <typename Hook>
constexpr void unlink_node(Hook* node) noexcept;


template <typename Hook>
constexpr void transfer_range(Hook* pos, Hook* first, Hook* last) noexcept;


template <typename Hook>
constexpr void transfer_one(Hook* pos, Hook* node) noexcept;


template <typename Hook>
[[nodiscard]] constexpr bool is_empty() noexcept;


template <typename Hook>
[[nodiscard]] constexpr size_t count_elements(const Hook* sentinel) noexcept;
