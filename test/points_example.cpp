#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"

#include <glaze/glaze.hpp>
#include <iostream>
#include <vector>

using G3 = ::rigid_geometric_algebra::algebra<double, 3>;

auto main() -> int
{
  const auto points = std::vector<G3::point>{{1, 1, 0, 0}, {1, 0, 1, 0}};

  std::cout << *glz::write_json(points) << "\n";
}
