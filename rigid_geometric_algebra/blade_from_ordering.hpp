#pragma once

#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"

#include <array>
#include <cstddef>
#include <utility>

namespace rigid_geometric_algebra {

/// determine the blade type from a `blade_ordering` value
///
/// @{

template <auto ordering>
  requires detail::is_specialization_of_v<decltype(ordering), blade_ordering>
struct blade_from_ordering
{
  template <std::size_t... Is>
  static constexpr auto impl(std::index_sequence<Is...>)
  {
    using A = typename decltype(ordering)::algebra_type;

    static constexpr auto dims = [] {
      const auto& mask = ordering.mask;
      auto dims = std::array<std::size_t, sizeof...(Is)>{};
      auto it = dims.begin();

      for (auto index = 0UZ; index != mask.size(); ++index) {
        if (mask.test(index)) {
          assert(it != dims.end());
          *it++ = index;
        }
      }
      assert(it == dims.end());

      return dims;
    }();

    return blade<A, std::get<Is>(dims)...>{};
  }

  using type =
      decltype(impl(std::make_index_sequence<ordering.mask.count()>{}));
};

template <auto ordering>
using blade_from_ordering_t = typename blade_from_ordering<ordering>::type;

/// @}

}  // namespace rigid_geometric_algebra
