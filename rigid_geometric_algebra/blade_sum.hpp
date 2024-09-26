#pragma once

#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/multivector.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {
namespace detail {

class blade_sum_fn
{
  template <class Canonical>
  struct sum_if
  {
    template <detail::blade... Bs>
    static constexpr auto operator()(const std::tuple<Bs...>& bs) -> Canonical
    {
      static constexpr auto mask =
          std::array{std::is_same_v<canonical_type_t<Bs>, Canonical>...};
      static constexpr auto count = std::ranges::count(mask, true);
      static_assert(count >= 1);

      static constexpr auto indices = []() consteval {
        auto indices = std::array<std::size_t, count>{};
        auto it = indices.begin();
        for (const auto [i, convertible] :
             std::views::zip(std::views::iota(0UZ), mask)) {
          if (convertible) {
            *it++ = i;
          }
        }
        return indices;
      }();

      return [&bs]<std::size_t... Is>(std::index_sequence<Is...>) {
        return (std::get<indices[Is]>(bs).canonical() + ...);
      }(std::make_index_sequence<count>{});
    }
  };

  template <class V, class... Cs>
  static constexpr auto impl(detail::type_list<Cs...>, const auto& bs) -> V
  {
    return {sum_if<Cs>{}(bs)...};
  }

public:
  template <
      detail::blade... Bs,
      class A = common_algebra_type_t<Bs...>,
      class V = typename sorted_canonical_blades_t<canonical_type_t<Bs>...>::
          template insert_into_t<::rigid_geometric_algebra::multivector<A>>>
    requires (sizeof...(Bs) != 0)
  static constexpr auto operator()(Bs&&... bs) -> V
  {
    return impl<V>(
        typename V::blade_list_type{},
        std::forward_as_tuple(std::forward<Bs>(bs)...));
  }
};

}  // namespace detail

/// variadic summation blades
/// @tparam Bs const and reference qualified `blade` types
/// @param bs blade values
///
/// Add multiple `blade` types to create a `multivector`. This differs from a
/// fold expression in that:
/// * it avoids creation temporary, intermediate multivector values
/// * it is usable in unevaluated context
///
/// Returns the linear combination of `bs...` as a  `multivector`, with `blade`
/// values converted to canonical form.
///
/// This cannot be used with no arguments to return an "empty" multivector.
/// Without any `blade` values, the `algebra_type` cannot be determined.
///
/// @note Requires:
/// * sizeof...(Bs) != 0
///
inline constexpr auto blade_sum = detail::blade_sum_fn{};

}  // namespace rigid_geometric_algebra
