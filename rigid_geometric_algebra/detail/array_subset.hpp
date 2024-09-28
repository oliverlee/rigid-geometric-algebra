#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"

#include <array>
#include <cstddef>
#include <ranges>

namespace rigid_geometric_algebra::detail {

/// container that holds a subset of a std::array
/// @tparam T value type
/// @tparam Capacity array capacity
///
/// A very simplified implementation fo static vectort. This is used to filter
/// values at compile time.
///
template <class T, std::size_t Capacity>
class array_subset
    : public std::ranges::view_interface<array_subset<T, Capacity>>
{
  using base_type = std::array<T, Capacity>;
  base_type base_{};
  std::size_t size_{};

public:
  constexpr array_subset(const base_type& base, std::size_t size)
      : base_{base}, size_{size}
  {
    detail::precondition(size_ <= Capacity);
  }

  constexpr auto begin() const noexcept { return base_.begin(); }
  constexpr auto end() const noexcept { return begin() + size_; }
};

}  // namespace rigid_geometric_algebra::detail
