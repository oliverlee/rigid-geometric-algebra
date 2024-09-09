#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
using ::rigid_geometric_algebra::sorted_canonical_blades_t;
using ::rigid_geometric_algebra::detail::type_list;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "not in order"_test = [] {
    return expect(
        std::is_same_v<
            type_list<
                G2::blade<>,
                G2::blade<0>,
                G2::blade<1>,
                G2::blade<2>,
                G2::blade<0, 1>,
                G2::blade<0, 2>,
                G2::blade<1, 2>,
                G2::blade<0, 1, 2>>,
            sorted_canonical_blades_t<
                G2::blade<0>,
                G2::blade<>,
                G2::blade<1>,
                G2::blade<2>,
                G2::blade<0, 1>,
                G2::blade<0, 2>,
                G2::blade<1, 2>,
                G2::blade<0, 1, 2>>>);
  };

  "has duplicates"_test = [] {
    return expect(
        std::is_same_v<
            type_list<G2::blade<>, G2::blade<0>, G2::blade<1>, G2::blade<2>>,
            sorted_canonical_blades_t<
                G2::blade<0>,
                G2::blade<>,
                G2::blade<>,
                G2::blade<1>,
                G2::blade<2>>>);
  };
}
