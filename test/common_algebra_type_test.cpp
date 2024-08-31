#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::common_algebra_type;
  using ::rigid_geometric_algebra::common_algebra_type_t;
  using ::rigid_geometric_algebra::detail::has_type_v;

  using rga2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using rga3 = ::rigid_geometric_algebra::algebra<double, 3>;

  "no common algebra type"_test = [] {
    return expect(
        not has_type_v<common_algebra_type<>> and
        not has_type_v<common_algebra_type<int>> and
        not has_type_v<common_algebra_type<rga2::blade<>, rga3::blade<>>>);
  };

  "common algebra type"_test = [] {
    return expect(
        std::is_same_v<rga2, common_algebra_type_t<rga2::blade<>>> and
        std::is_same_v<
            rga2,
            common_algebra_type_t<rga2::blade<>, rga2::blade<>>> and
        std::is_same_v<
            rga2,
            common_algebra_type_t<rga2::blade<1>, rga2::blade<2>>>);
  };
}
