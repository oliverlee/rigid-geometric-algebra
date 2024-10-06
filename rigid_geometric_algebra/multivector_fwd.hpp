#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"

#include <type_traits>
namespace rigid_geometric_algebra {

template <class A, class... Bs>
  requires is_algebra_v<A> and (is_canonical_blade_order<Bs...>()) and
           (std::is_same_v<A, algebra_type_t<Bs>> and ...)
class multivector;

}
