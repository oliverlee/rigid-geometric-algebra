#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include "test/skytest_ext.hpp"

#include <symengine/compat.hpp>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::equal_elements;
  using ::skytest::expect;

  using GS3 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 3>;
  using ::rigid_geometric_algebra::meet;

  static const auto l = GS3::line{
      "lvx",
      "lvy",
      "lvz",
      "lmx",
      "lmy",
      "lmz",
  };

  static const auto g = GS3::plane{"gx", "gy", "gz", "gw"};
  static const auto h = GS3::plane{"hx", "hy", "hz", "hw"};

  // Table 2.7

  "meet of two planes is a line"_test = [] {
    return expect(equal_elements(
        GS3::line{
            "gz*hy - gy*hz",
            "gx*hz - gz*hx",
            "gy*hx - gx*hy",
            "gx*hw - gw*hx",
            "gy*hw - gw*hy",
            "gz*hw - gw*hz",
        },
        meet(g, h)));
  };

  "meet of a plane and a line is a point"_test = [] {
    return expect(equal_elements(
        GS3::point{
            "-gx*lvx - gy*lvy - gz*lvz",
            "gz*lmy - gy*lmz + gw*lvx",
            "gx*lmz - gz*lmx + gw*lvy",
            "gy*lmx - gx*lmy + gw*lvz",
        },
        meet(g, l)));
  };
}
