#pragma once

#include "rigid_geometric_algebra/is_algebra.hpp"
namespace rigid_geometric_algebra {

template <class A>
  requires is_algebra_v<A>
class point;

template <class A>
  requires is_algebra_v<A>
class line;

}  // namespace rigid_geometric_algebra
