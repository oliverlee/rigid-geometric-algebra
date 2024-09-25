#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/complement.hpp"
#include "rigid_geometric_algebra/detail/even.hpp"
#include "rigid_geometric_algebra/wedge.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <cstddef>
#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra {

/// antiwedge product
///
/// @see eq. 2.25
///
inline constexpr class
// TODO derive nary multivector operation
{
public:
  template <class B1, class B2>
    requires is_blade_v<B1> and is_blade_v<B2>
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
      using C1 = std::invoke_result_t<decltype(right_complement), B1>;
      using C2 = std::invoke_result_t<decltype(right_complement), B2>;

      static constexpr auto negate_count =
          std::size_t(detail::blade_complement_negates<right_t, B1>) +
          std::size_t(detail::blade_complement_negates<right_t, B2>) +
          std::size_t(detail::blade_complement_negates<
                      left_t,
                      std::invoke_result_t<decltype(wedge), C1, C2>>);

      using maybe_negate = std::conditional_t<
          detail::even(negate_count),
          std::identity,
          std::negate<>>;

      return result_type{maybe_negate{}(b1.coefficient * b2.coefficient)};
    }
  }
} antiwedge{};

}  // namespace rigid_geometric_algebra
