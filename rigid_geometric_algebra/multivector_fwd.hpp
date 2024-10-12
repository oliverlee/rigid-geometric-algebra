#pragma once

#include "rigid_geometric_algebra/blade_dimensions.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"

namespace rigid_geometric_algebra {

template <class A, blade_dimensions... D>
  requires is_algebra_v<A>
class multivector;

}
