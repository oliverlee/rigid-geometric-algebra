#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/complement.hpp"
#include "rigid_geometric_algebra/detail/concat_ranges.hpp"
#include "rigid_geometric_algebra/detail/counted_sort.hpp"
#include "rigid_geometric_algebra/detail/linear_operator.hpp"
#include "rigid_geometric_algebra/detail/negate_if_odd.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/zero_constant_fwd.hpp"

#include <cstddef>
#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

class antiwedge_blade_fn
{
public:
  template <detail::blade B1, detail::blade B2>
  static constexpr auto operator()(const B1& b1, const B2& b2)
      -> decltype(left_complement(right_complement(b1) ^ right_complement(b2)))
  {
    using result_type =
        decltype(left_complement(right_complement(b1) ^ right_complement(b2)));

    if constexpr (
        std::is_same_v<
            result_type,
            zero_constant<algebra_type_t<result_type>>>) {
      return {};
    } else {
      static constexpr auto negate_count =
          std::size_t(detail::blade_complement_negates<right_t, B1>) +
          std::size_t(detail::blade_complement_negates<right_t, B2>) +
          detail::counted_sort(detail::concat_ranges(
              decltype(right_complement(b1))::dimensions,
              decltype(right_complement(b2))::dimensions)) +
          std::size_t(detail::blade_complement_negates<
                      left_t,
                      decltype(right_complement(b1) ^ right_complement(b2))>);

      return result_type{detail::negate_if_odd<negate_count>{}(
          b1.coefficient * b2.coefficient)};
    }
  }
};

}  // namespace detail

/// antiwedge product
///
/// @see eq. 2.25
///
inline constexpr auto antiwedge =
    detail::linear_operator<detail::antiwedge_blade_fn>{};

}  // namespace rigid_geometric_algebra
