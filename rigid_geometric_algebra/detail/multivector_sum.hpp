#pragma once

#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/type_concat.hpp"
#include "rigid_geometric_algebra/get_or.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/multivector_fwd.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
#include "rigid_geometric_algebra/zero_constant_fwd.hpp"
#include "type_list.hpp"

#include <type_traits>

namespace rigid_geometric_algebra::detail {

class multivector_sum_fn
{
public:
  template <
      detail::multivector V1,
      detail::multivector V2,
      class A = common_algebra_type_t<V1, V2>>
  static constexpr auto
  operator()(V1&& v1, V2&& v2) -> typename detail::type_concat_t<
      typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
      typename std::remove_cvref_t<to_multivector_t<V2>>::blade_list_type>::
      template insert_into_t<sorted_canonical_blades<>>::template insert_into_t<
          ::rigid_geometric_algebra::multivector<A>>
  {
    using result_blade_list_type = typename detail::type_concat_t<
        typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
        typename std::remove_cvref_t<to_multivector_t<V2>>::blade_list_type>::
        template insert_into_t<sorted_canonical_blades<>>;

    return []<class... Ts, class V3, class V4>(
               detail::type_list<Ts...>, V3&& v3, V4&& v4) {
      return ::rigid_geometric_algebra::multivector<A, Ts...>{(
          get_or<Ts>(std::forward<V3>(v3), zero_constant<A>{}) +
          get_or<Ts>(std::forward<V4>(v4), zero_constant<A>{}))...};
    }(result_blade_list_type{},
           std::forward<V1>(v1),
           to_multivector(std::forward<V2>(v2)));
  }
};

inline constexpr auto multivector_sum = multivector_sum_fn{};

}  // namespace rigid_geometric_algebra::detail
