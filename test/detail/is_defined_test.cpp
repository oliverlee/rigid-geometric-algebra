#include "rigid_geometric_algebra/detail/is_defined.hpp"

#include <functional>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::is_defined_v;
  using ::rigid_geometric_algebra::detail::op;

  static_assert(not is_defined_v<op<std::plus<>, int>>);
  static_assert(is_defined_v<op<std::plus<>, int>>);
}
