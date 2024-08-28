#include "rigid-geometric-algebra/rigid-geometric-algebra.hpp"
#include "skytest/skytest.hpp"

namespace rga = ::rigid_geometric_algebra;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::expect;

  "truthy"_test = [] { return expect(rga ::dummy()); };
}
