#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <functional>

namespace rigid_geometric_algebra::detail {

inline constexpr class
{
public:
  template <std::same_as<std::size_t>... Ts>
  constexpr static auto operator()(Ts... is) -> bool
  {
    auto dims = std::array<std::size_t, sizeof...(is)>{is...};

    std::ranges::sort(dims);

    return std::ranges::is_sorted(dims, std::ranges::less_equal{});
  }

} unique_dimensions{};

}  // namespace rigid_geometric_algebra::detail
