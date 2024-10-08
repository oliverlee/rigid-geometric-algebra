#pragma once

#include "rigid_geometric_algebra/blade_sum.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/copy_ref_qual.hpp"
#include "rigid_geometric_algebra/detail/is_complete.hpp"
#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"
#include "rigid_geometric_algebra/detail/multivector_promotable.hpp"
#include "rigid_geometric_algebra/detail/type_filter.hpp"
#include "rigid_geometric_algebra/detail/type_product.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/multivector_fwd.hpp"
#include "rigid_geometric_algebra/multivector_type_from_blade_list.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
#include "rigid_geometric_algebra/zero_constant_fwd.hpp"

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// helper type that synthesizes overloads for `multivector` arguments
/// @tparam F function object defining `blade` overloads
///
/// Provides overloads if `multivector` is a function argument. The derived
/// function object must implement a linear transformation - these overloads
/// always return a linear combination of the derived function object to
/// individual `blade` arguments.
///
template <class F>
class derive_multivector_overload
{
  static_assert(
      detail::is_complete_v<F>,
      "`F` must be complete - this is not intended as a CRTP base class.");

  template <template <class...> class list, class... Bs, class V>
  static constexpr auto impl(list<Bs...>, V&& v)
      -> multivector_type_from_blade_list_t<sorted_canonical_blades_t<
          std::invoke_result_t<F, detail::copy_ref_qual_t<V&&, Bs>>...>>
  {
    return blade_sum(F{}(get<Bs>(std::forward<V>(v)))...);
  }

  static constexpr auto is_non_zero_constant_result = []<class T> {
    using B1 = typename T::first_type;
    using B2 = typename T::second_type;
    using R = std::invoke_result_t<F, B1, B2>;
    return not detail::is_specialization_of<R, zero_constant>{};
  };

  template <class V1, class V2>
  using blade_list_product_t = detail::type_filter_t<
      detail::type_product_t<
          typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
          typename std::remove_cvref_t<to_multivector_t<V2>>::blade_list_type>,
      is_non_zero_constant_result>;

  template <template <class...> class list, class V1, class V2>
  static constexpr auto impl2(list<>, const V1&, const V2&)
      -> zero_constant<common_algebra_type_t<V1, V2>>
  {
    return {};
  }

  template <template <class...> class list, class... Pairs, class V1, class V2>
    requires (sizeof...(Pairs) != 0)
  static constexpr auto
  impl2(list<Pairs...>, V1&& v1, V2&& v2) -> typename sorted_canonical_blades_t<
      std::invoke_result_t<
          F,
          detail::copy_ref_qual_t<V1&&, typename Pairs::first_type>,
          detail::copy_ref_qual_t<V2&&, typename Pairs::second_type>>...>::
      template insert_into_t<
          ::rigid_geometric_algebra::multivector<common_algebra_type_t<V1, V2>>>
  {
    return blade_sum(F{}(
        get<typename Pairs::first_type>(std::forward<V1>(v1)),
        get<typename Pairs::second_type>(std::forward<V2>(v2)))...);
  }

  template <class V1, class V2>
  static constexpr auto impl(V1&& v1, V2&& v2) -> decltype(impl2(
      blade_list_product_t<V1, V2>{},
      std::forward<V1>(v1),
      std::forward<V2>(v2)))
  {
    return impl2(
        blade_list_product_t<V1, V2>{},
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
    requires has_common_algebra_type_v<V1, V2> and
             (detail::multivector<V1> or detail::multivector<V2>)
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
