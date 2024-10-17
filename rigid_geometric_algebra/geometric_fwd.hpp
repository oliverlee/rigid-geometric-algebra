#pragma once

#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {

template <class A>
  requires is_algebra_v<A>
class point;

template <class A>
  requires is_algebra_v<A>
class line;

template <class A>
  requires is_algebra_v<A>
class plane;

namespace detail {

template <detail::multivector V>
class geometric_interface;

template <class T>
concept geometric =
    requires { typename std::remove_cvref_t<T>::geometric_interface_type; } and
    detail::is_specialization_of_v<
        typename std::remove_cvref_t<T>::geometric_interface_type,
        detail::geometric_interface>;

}  // namespace detail
}  // namespace rigid_geometric_algebra
