#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::expect;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;

  "scalar type"_test = [] {
    return expect(
        std::is_same_v<G2::blade<>, G2::scalar> and
        std::is_same_v<G3::blade<>, G3::scalar>);
  };

  "antiscalar type"_test = [] {
    return expect(
        std::is_same_v<G2::blade<0, 1, 2>, G2::antiscalar> and
        std::is_same_v<G3::blade<0, 1, 2, 3>, G3::antiscalar>);
  };
}
