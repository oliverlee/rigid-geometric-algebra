#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::detail::has_type_v;

  "has type"_test = [] {
    return expect(
        has_type_v<std::add_const<int>> and
        has_type_v<std::common_type<int, double>> and not has_type_v<int> and
        not has_type_v<std::common_type<int*, double>>);
  };
}
