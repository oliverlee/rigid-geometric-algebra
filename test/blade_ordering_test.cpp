#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
#include <type_traits>

using rga = ::rigid_geometric_algebra::algebra<double, 2>;
using ::rigid_geometric_algebra::blade_ordering;

template <std::size_t... Is>
constexpr auto ord =
    blade_ordering<rga>{std::type_identity<rga::blade<Is...>>{}};

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;

  "eq"_test = [] {
    return expect(
        eq(ord<>, ord<>) and eq(ord<1>, ord<1>) and eq(ord<1, 2>, ord<2, 1>) and
        eq(ord<0, 1, 2>, ord<0, 2, 1>));
  };

  "lt"_test = [] {
    return expect(
        lt(ord<>, ord<0>) and lt(ord<0>, ord<1>) and lt(ord<1>, ord<2>) and
        lt(ord<>, ord<2>) and lt(ord<0, 1>, ord<0, 2>) and
        lt(ord<0, 2>, ord<1, 2>) and lt(ord<1, 2>, ord<0, 1, 2>));
  };
}
