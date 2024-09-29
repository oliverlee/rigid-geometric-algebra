#pragma once

#include "rigid_geometric_algebra/detail/array_subset.hpp"
#include "rigid_geometric_algebra/detail/has_type.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <ranges>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// returns the disjoint subset of values
/// @param is variadic sequence of values
///
/// Returns the disjoint values (values that only appear once) in an
/// unspecified random access container. Return values are sorted.
///
inline constexpr class
{
public:
  static constexpr auto operator()() -> std::ranges::empty_view<std::size_t>
  {
    return {};
  }

  template <class... Ts>
    requires detail::has_type_v<std::common_type<Ts...>>
  static constexpr auto
  operator()(Ts... is) -> array_subset<std::common_type_t<Ts...>, sizeof...(Ts)>
  {
    auto dims = std::array<std::common_type_t<Ts...>, sizeof...(is)>{is...};
    std::ranges::sort(dims);

    auto pending = std::ranges::subrange(dims);
    while (pending) {
      using U = decltype(pending.size());
      if (pending.size() == U{1} or pending[0] != pending[1]) {
        pending.advance(1);
        continue;
      }

      const auto next = std::ranges::find_if(
          pending,
          std::bind_front(std::ranges::not_equal_to{}, std::cref(pending[0])));
      const auto [last, _] = std::ranges::rotate(pending, next);
      pending = {pending.begin(), last};
    }

    return {dims.begin(), pending.begin()};
  }

} disjoint_subset{};

}  // namespace rigid_geometric_algebra::detail
