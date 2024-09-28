#pragma once

#include <algorithm>
#include <ranges>
#include <type_traits>
#include <vector>

namespace rigid_geometric_algebra::detail {

inline constexpr class
{
public:
  template <
      std::ranges::random_access_range R1,
      std::ranges::random_access_range R2>
    requires std::is_same_v<
        std::ranges::range_value_t<R1>,
        std::ranges::range_value_t<R2>>
  static constexpr auto operator()(const R1& r1, const R2& r2)
      -> std::vector<std::ranges::range_value_t<R1>>
  {
    auto out = std::vector<std::ranges::range_value_t<R1>>{};
    out.resize(r1.size() + r2.size());

    const auto [_, it] = std::ranges::copy(r1, out.begin());
    std::ranges::copy(r2, it);

    return out;
  }
} concat_ranges{};

}  // namespace rigid_geometric_algebra::detail
