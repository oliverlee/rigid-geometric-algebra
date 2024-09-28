#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"

#include <glaze/glaze.hpp>
#include <iostream>
#include <vector>

using G2 = ::rigid_geometric_algebra::algebra<double, 2>;

auto main() -> int
{
  const auto points = std::vector<G2::point>{
      {1, 2, 3}, {2, 3, 4}, {3, 4, 5}, {4, 5, 6}, {5, 6, 7}};

  std::cout << *glz::write_json(points) << "\n";
}
