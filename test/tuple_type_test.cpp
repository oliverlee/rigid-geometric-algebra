#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <tuple>
#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::expect;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using ::rigid_geometric_algebra::tuple_type;
  using ::rigid_geometric_algebra::tuple_type_t;
  using ::rigid_geometric_algebra::detail::has_type_v;

  "tuple_type"_test = [] {
    static_assert(
        std::is_same_v<
            std::tuple<G2::blade<>, G2::blade<1>>,
            tuple_type_t<G2::multivector<G2::blade<>, G2::blade<1>>>>);
    return expect(
        std::is_same_v<
            std::tuple<G2::blade<>, G2::blade<1>>,
            tuple_type_t<G2::multivector<G2::blade<>, G2::blade<1>>>> and
        (not has_type_v<tuple_type<G2::point>>));
  };
}
