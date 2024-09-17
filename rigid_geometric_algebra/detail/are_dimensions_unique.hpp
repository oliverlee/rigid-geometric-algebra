#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <functional>

namespace rigid_geometric_algebra::detail {

/// determines if values are unique
/// @param is variadic sequence of `std::size_t` values
///
/// Returns `true` if all values in `is` are unique. Otherwise false.
///
inline constexpr class
{
public:
  template <std::same_as<std::size_t>... Ts>
  static constexpr auto operator()(Ts... is) -> bool
  {
    auto dims = std::array<std::size_t, sizeof...(is)>{is...};

    std::ranges::sort(dims);

    return std::ranges::is_sorted(dims, std::ranges::less_equal{});
  }

} are_dimensions_unique{};

}  // namespace rigid_geometric_algebra::detail
