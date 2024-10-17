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
  using ::rigid_geometric_algebra::join;

  static const auto p = GS3::point{"pw", "px", "py", "pz"};
  static const auto q = GS3::point{"qw", "qx", "qy", "qz"};

  static const auto l = GS3::line{
      "lvx",
      "lvy",
      "lvz",
      "lmx",
      "lmy",
      "lmz",
  };

  // Table 2.7
  "join of two points is a line"_test = [] {
    return expect(equal_elements(
        GS3::line{
            "pw*qx - px*qw",
            "pw*qy - py*qw",
            "pw*qz - pz*qw",
            "py*qz - pz*qy",
            "pz*qx - px*qz",
            "px*qy - py*qx",
        },
        join(p, q)));
  };

  "join of a point and a line is a plane"_test = [] {
    return expect(equal_elements(
        GS3::plane{
            "lvy*pz - lvz*py + lmx*pw",
            "lvz*px - lvx*pz + lmy*pw",
            "lvx*py - lvy*px + lmz*pw",
            "-lmx*px - lmy*py - lmz*pz",
        },
        join(l, p)));
  };
}
