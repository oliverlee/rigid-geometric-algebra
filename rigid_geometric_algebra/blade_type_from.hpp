#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/detail/contract.hpp"

#include <array>
#include <cstddef>
#include <utility>

namespace rigid_geometric_algebra {

/// determine the blade type from a `blade_ordering` value
///
/// @{

template <blade_ordering ord>
struct blade_type_from
{
  template <std::size_t... Is>
  static constexpr auto impl(std::index_sequence<Is...>)
  {
    using A = algebra_type_t<decltype(ord)>;

    static constexpr auto dims = [] {
      const auto& mask = ord.mask;
      auto dims = std::array<std::size_t, sizeof...(Is)>{};
      auto it = dims.begin();

      for (auto index = 0UZ; index != mask.size(); ++index) {
        if (mask.test(index)) {
          detail::invariant(it != dims.end());
          *it++ = index;
        }
      }

      detail::postcondition(it == dims.end());
      return dims;
    }();

    return blade<A, std::get<Is>(dims)...>{};
  }

  using type = decltype(impl(std::make_index_sequence<ord.mask.count()>{}));
};

template <blade_ordering ord>
using blade_type_from_t = typename blade_type_from<ord>::type;

/// @}

}  // namespace rigid_geometric_algebra
