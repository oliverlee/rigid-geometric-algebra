#pragma once

#include "rigid_geometric_algebra/detail/decays_to.hpp"
#include "rigid_geometric_algebra/detail/define_prioritized_overload.hpp"
#include "rigid_geometric_algebra/detail/invoke_prioritized_overload.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/priority_list.hpp"

#include <functional>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra::detail {

/// defines vector space operations for a derived type
/// @tparam D derived type
/// @tparam F component accessor function object
///
/// Defines vector space operations by performing the operation on each
/// component of the derived type `D` and then uses all the results to construct
/// a new value of the derived type.
///
/// This type defines the following operations:
/// -: D     -> D (negation)
/// -: D x D -> D (subtraction)
/// +: D x D -> D (addition)
/// *: S x D -> D (scalar multiplication)
///
/// for derived type D and scalar type S.
///
template <class D, class... F>
class derive_vector_space_operations
{
  struct minus_impl
  {
    template <class T1, class T2>
    static constexpr auto operator()(T1&& t1, T2&& t2)
    {
      return D{(F{}(std::forward<T1>(t1)) - F{}(std::forward<T2>(t2)))...};
    }
  };

public:
  /// negation
  ///
  template <detail::decays_to<D> T1>
  friend constexpr auto operator-(T1&& t1) -> D
  {
    return D{-(F{}(std::forward<T1>(t1)))...};
  }

  /// subtraction
  ///
  template <detail::decays_to<D> T1, detail::decays_to<D> T2>
    requires define_prioritized_overload_v<
        priority_for<std::minus<>, derive_vector_space_operations<>>,
        overload<std::minus<>, T1, T2>,
        minus_impl>
  friend constexpr auto operator-(T1&& t1, T2&& t2) -> D
  {
    return invoke_prioritized_overload<std::minus<>>(
        std::forward<T1>(t1), std::forward<T2>(t2));
  }

  /// addition
  ///
  template <detail::decays_to<D> T1, detail::decays_to<D> T2>
  friend constexpr auto operator+(T1&& t1, T2&& t2) -> D
  {
    return D{(F{}(std::forward<T1>(t1)) + F{}(std::forward<T2>(t2)))...};
  }

  /// scalar multiplication
  ///
  template <class S, detail::decays_to<D> T2>
    requires ((
        std::is_invocable_r_v<
            std::remove_cvref_t<std::invoke_result_t<F, T2>>,
            std::multiplies<>,
            S,
            std::invoke_result_t<F, T2>> and
        ...))
  friend constexpr auto operator*(const S& s, T2&& t2) -> D
  {
    return D{(s * F{}(std::forward<T2>(t2)))...};
  }

  /// equality comparison
  ///
  /// defined to allow default definition for derived types
  ///
  friend auto operator==(
      const derive_vector_space_operations&,
      const derive_vector_space_operations&) -> bool = default;
};

}  // namespace rigid_geometric_algebra::detail
