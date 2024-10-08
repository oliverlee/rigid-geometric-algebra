#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/detail/are_dimensions_unique.hpp"

#include <cstddef>
namespace rigid_geometric_algebra {

template <class A, std::size_t... Is>
  requires ((Is < algebra_dimension_v<A>) and ...) and
           (detail::are_dimensions_unique(Is...))
class blade;

}
