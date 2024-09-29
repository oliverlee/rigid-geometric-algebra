#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/detail/indices_array.hpp"

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
    return blade<
        algebra_type_t<decltype(ord)>,
        std::get<Is>(detail::indices_array<ord.mask>)...>{};
  }

  using type = decltype(impl(std::make_index_sequence<ord.mask.count()>{}));
};

template <blade_ordering ord>
using blade_type_from_t = typename blade_type_from<ord>::type;

/// @}

}  // namespace rigid_geometric_algebra
