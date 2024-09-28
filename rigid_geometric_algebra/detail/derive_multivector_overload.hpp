#pragma once

#include "rigid_geometric_algebra/blade_sum.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/copy_ref_qual.hpp"
#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"
#include "rigid_geometric_algebra/detail/multivector_promotable.hpp"
#include "rigid_geometric_algebra/detail/type_filter.hpp"
#include "rigid_geometric_algebra/detail/type_product.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/multivector.hpp"
#include "rigid_geometric_algebra/multivector_type_from_blade_list.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// synthesizes a multivector overload as a linear combination of blade
/// overloads
/// @tparam D derived type
///
template <class D>
class derive_multivector_overload
{
  template <
      class Self = D,
      template <class...> class list,
      class... Bs,
      class V>
  static constexpr auto impl(list<Bs...>, V&& v)
      -> multivector_type_from_blade_list_t<sorted_canonical_blades_t<
          std::invoke_result_t<Self, detail::copy_ref_qual_t<V&&, Bs>>...>>
  {
    return blade_sum(Self::operator()(get<Bs>(std::forward<V>(v)))...);
  }

  template <class Self>
  static constexpr auto is_non_zero_constant_result = []<class T> {
    using B1 = typename T::first_type;
    using B2 = typename T::second_type;
    using R = std::invoke_result_t<Self, B1, B2>;
    return not detail::is_specialization_of<R, zero_constant>{};
  };

  template <class Self, class V1, class V2>
  using blade_list_product_t = detail::type_filter_t<
      detail::type_product_t<
          typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
          typename std::remove_cvref_t<to_multivector_t<V2>>::blade_list_type>,
      is_non_zero_constant_result<Self>>;

  template <template <class...> class list, class V1, class V2>
  static constexpr auto impl2(list<>, const V1&, const V2&)
      -> zero_constant<common_algebra_type_t<V1, V2>>
  {
    return {};
  }

  template <
      class Self = D,
      template <class...> class list,
      class... Pairs,
      class V1,
      class V2>
    requires (sizeof...(Pairs) != 0)
  static constexpr auto
  impl2(list<Pairs...>, V1&& v1, V2&& v2) -> typename sorted_canonical_blades_t<
      std::invoke_result_t<
          Self,
          detail::copy_ref_qual_t<V1&&, typename Pairs::first_type>,
          detail::copy_ref_qual_t<V2&&, typename Pairs::second_type>>...>::
      template insert_into_t<
          ::rigid_geometric_algebra::multivector<common_algebra_type_t<V1, V2>>>
  {
    return blade_sum(Self::operator()(
        get<typename Pairs::first_type>(std::forward<V1>(v1)),
        get<typename Pairs::second_type>(std::forward<V2>(v2)))...);
  }

  template <class Self = D, class V1, class V2>
  static constexpr auto impl(V1&& v1, V2&& v2) -> decltype(impl2(
      blade_list_product_t<Self, V1, V2>{},
      std::forward<V1>(v1),
      std::forward<V2>(v2)))
  {
    return impl2(
        blade_list_product_t<Self, V1, V2>{},
        std::forward<V1>(v1),
        std::forward<V2>(v2));
  }

public:
  template <detail::multivector V>
  static constexpr auto operator()(V&& v) -> decltype(impl(
      typename std::remove_cvref_t<V>::blade_list_type{}, std::forward<V>(v)))
  {
    return impl(
        typename std::remove_cvref_t<V>::blade_list_type{}, std::forward<V>(v));
  }
  template <
      detail::multivector_promotable V1,
      detail::multivector_promotable V2>
    requires (not(detail::blade<V1> and detail::blade<V2>)) and
             has_common_algebra_type_v<V1, V2>
  static constexpr auto operator()(V1&& v1, V2&& v2) -> decltype(impl(
      to_multivector(std::forward<V1>(v1)),
      to_multivector(std::forward<V2>(v2))))
  {
    return impl(
        to_multivector(std::forward<V1>(v1)),
        to_multivector(std::forward<V2>(v2)));
  }
};

}  // namespace rigid_geometric_algebra::detail
