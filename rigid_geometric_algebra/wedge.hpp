#pragma once

#include "rigid_geometric_algebra/blade_type_from.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/concat_ranges.hpp"
#include "rigid_geometric_algebra/detail/counted_sort.hpp"
#include "rigid_geometric_algebra/detail/geometric_operator.hpp"
#include "rigid_geometric_algebra/detail/linear_operator.hpp"
#include "rigid_geometric_algebra/detail/negate_if_odd.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/zero_constant_fwd.hpp"

#include <cstddef>
#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

class wedge_blade_fn
{
  template <detail::blade B1, detail::blade B2>
    requires has_common_algebra_type_v<B1, B2>
  static constexpr auto unique_factors_v = [] {
    return (std::remove_cvref_t<B1>::dimension_mask &
            std::remove_cvref_t<B2>::dimension_mask)
               .count() == 0U;
  }();

  template <detail::blade B1, detail::blade B2>
    requires unique_factors_v<B1, B2>
  using blade_result_t = decltype([] {
    using A = common_algebra_type_t<B1, B2>;

    static constexpr auto mask =
        (std::remove_cvref_t<B1>::dimension_mask |
         std::remove_cvref_t<B2>::dimension_mask);

    return blade_type_from_mask_t<A, mask>{};
  }());

public:
  template <detail::blade B1, detail::blade B2>
    requires has_common_algebra_type_v<B1, B2> and unique_factors_v<B1, B2>
  static constexpr auto operator()(B1&& b1, B2&& b2) -> blade_result_t<B1, B2>
  {
    static constexpr auto num_swaps =
        detail::counted_sort(detail::concat_ranges(
            std::remove_cvref_t<B1>::dimensions,
            std::remove_cvref_t<B2>::dimensions));

    return blade_result_t<B1, B2>{detail::negate_if_odd<num_swaps>{}(
        std::forward<B1>(b1).coefficient * std::forward<B2>(b2).coefficient)};
  }

  template <detail::blade B1, detail::blade B2>
    requires has_common_algebra_type_v<B1, B2> and
             (not unique_factors_v<B1, B2>)
  static constexpr auto operator()(const B1&, const B2&)
      -> zero_constant<common_algebra_type_t<B1, B2>>
  {
    return {};
  }
};

class wedge_fn
    : public detail::linear_operator<detail::wedge_blade_fn>,
      public detail::geometric_operator
{
public:
  using detail::linear_operator<detail::wedge_blade_fn>::operator();
  using detail::geometric_operator::operator();
};

}  // namespace detail

/// wedge product
///
/// @see sec. 2.1.1
///
inline constexpr auto wedge = detail::wedge_fn{};

}  // namespace rigid_geometric_algebra
