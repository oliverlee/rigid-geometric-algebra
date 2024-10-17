#pragma once

#include "rigid_geometric_algebra/is_algebra.hpp"
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

}  // namespace rigid_geometric_algebra
