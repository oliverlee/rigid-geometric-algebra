#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <array>
#include <glaze/glaze.hpp>

using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
using G3 = ::rigid_geometric_algebra::algebra<double, 3>;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "serialize point"_test = [] {
    const auto p = G2::point{1, 2, 3};

    return expect(eq(R"({"point":[1,2,3]})", glz::write_json(p)));
  };

  "deserialize point"_test = [] {
    const auto p = G2::point{1, 2, 3};

    return expect(eq(p, glz::read_json<G2::point>(R"({"point":[1,2,3]})")));
  };

  "serialize point array"_test = [] {
    constexpr auto points =
        std::array{G2::point{1, 2, 3}, G2::point{2, 3, 4}, G2::point{3, 4, 5}};

    constexpr auto msg =
        R"([{"point":[1,2,3]},{"point":[2,3,4]},{"point":[3,4,5]}])";

    return expect(eq(msg, glz::write_json(points)));
  };
}
