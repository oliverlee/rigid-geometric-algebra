#pragma once

#include <functional>
#include <type_traits>

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
struct derive_vector_space_operations
{
  template <class T>
  static constexpr auto is_derived_reference_v =
      std::is_same_v<D, std::remove_cvref_t<T>>;

  /// negation
  ///
  template <class T1>
    requires is_derived_reference_v<T1>
  friend constexpr auto operator-(T1&& t1) -> D
  {
    return {-(F{}(std::forward<T1>(t1)))...};
  }

  /// subtraction
  ///
  template <class T1, class T2>
    requires is_derived_reference_v<T1> and is_derived_reference_v<T2>
  friend constexpr auto operator-(T1&& t1, T2&& t2) -> D
  {
    return {(F{}(std::forward<T1>(t1)) - F{}(std::forward<T2>(t2)))...};
  }

  /// addition
  ///
  template <class T1, class T2>
    requires is_derived_reference_v<T1> and is_derived_reference_v<T2>
  friend constexpr auto operator+(T1&& t1, T2&& t2) -> D
  {
    return {(F{}(std::forward<T1>(t1)) + F{}(std::forward<T2>(t2)))...};
  }

  /// scalar multiplication
  ///
  template <class S, class T2>
    requires (is_derived_reference_v<T2> and
              (std::is_invocable_r_v<
                   std::remove_cvref_t<std::invoke_result_t<F, T2>>,
                   std::multiplies<>,
                   S,
                   std::invoke_result_t<F, T2>> and
               ...))
  friend constexpr auto operator*(const S& s, T2&& t2) -> D
  {
    return {(s * F{}(std::forward<T2>(t2)))...};
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
