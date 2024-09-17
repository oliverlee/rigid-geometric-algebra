#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <symengine/compat.hpp>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "unit hypervolume (floating point)"_ctest = [] {
    using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
    return expect(eq(G2::blade<0, 1, 2>{1}, G2::unit_hypervolume));
  };

  "unit hypervolume (symengine expression)"_test = [] {
    using GS2 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 2>;
    return expect(eq(
        GS2::blade<0, 1, 2>{::SymEngine::Expression{1}},
        GS2::unit_hypervolume));
  };
}
