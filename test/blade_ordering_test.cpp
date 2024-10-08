#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>

using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
using ::rigid_geometric_algebra::blade_ordering;

template <std::size_t... Is>
constexpr auto ord = blade_ordering<G2>{G2::blade<Is...>::dimension_mask};

template <std::size_t... Is>
constexpr auto ord3 = blade_ordering<G3>{G3::blade<Is...>::dimension_mask};

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

  "lt (G2)"_test = [] {
    return expect(
        lt(ord<>, ord<0>) and         //
        lt(ord<0>, ord<1>) and        //
        lt(ord<1>, ord<2>) and        //
        lt(ord<>, ord<2>) and         //
        lt(ord<0, 1>, ord<0, 2>) and  //
        lt(ord<0, 2>, ord<1, 2>) and  //
        lt(ord<1, 2>, ord<0, 1, 2>));
  };

  "lt (G3)"_test = [] {
    return expect(
        lt(ord3<0, 3>, ord3<1, 2>) and  //
        lt(ord3<2, 3>, ord3<3, 1>) and  //
        lt(ord3<3, 1>, ord3<1, 2>));
  };
}
