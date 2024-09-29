#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"
#include "rigid_geometric_algebra/detail/decays_to.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
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
public:
  using base_type = std::array<T, Capacity>;
  base_type base_{};
  std::size_t size_{};

  template <std::ranges::random_access_range R>
    requires (not detail::decays_to<R, array_subset>) and
             std::constructible_from<T, std::ranges::range_value_t<R>>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr array_subset(R&& rng) : size_{rng.size()}
  {
    detail::precondition(rng.size() <= Capacity);
    std::ranges::move(rng, base_.begin());
  }

  template <std::random_access_iterator I, std::sentinel_for<I> S>
  constexpr array_subset(I first, S last)
      : array_subset{std::ranges::subrange{first, last}}
  {}

  constexpr array_subset(std::initializer_list<std::size_t> il)
      : array_subset{std::ranges::subrange{il.begin(), il.end()}}
  {}

  constexpr auto begin() const noexcept { return base_.begin(); }
  constexpr auto end() const noexcept { return begin() + size_; }
};

}  // namespace rigid_geometric_algebra::detail
