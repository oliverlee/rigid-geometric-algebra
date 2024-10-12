#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <utility>

namespace rigid_geometric_algebra {

/// dimensions of a blade
/// @tparam N blade grade
///
/// Wrapper around `std::array` providing better deduction when used as a
/// non-type template parameter.
///
template <std::size_t N>
struct blade_dimensions : std::array<std::size_t, N>
{
  constexpr blade_dimensions(const std::array<std::size_t, N>& dims)
      : std::array<std::size_t, N>{dims}
  {}

  template <std::integral... T>
    requires (sizeof...(T) == N)
  constexpr blade_dimensions(T... dims)
      : std::array<std::size_t, N>{(
            detail::precondition(std::in_range<std::size_t>(dims)),
            static_cast<std::size_t>(dims))...}
  {}
};

template <std::size_t N>
// C-array necessary for deduction
// NOLINTNEXTLINE(modernize-avoid-c-arrays)
blade_dimensions(const int (&)[N]) -> blade_dimensions<N>;

template <std::integral... T>
blade_dimensions(T...) -> blade_dimensions<sizeof...(T)>;

}  // namespace rigid_geometric_algebra
