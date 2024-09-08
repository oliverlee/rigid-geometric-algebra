#include "rigid_geometric_algebra/detail/is_defined.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"

#include <functional>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::is_defined_v;
  using ::rigid_geometric_algebra::detail::overload;

  static_assert(not is_defined_v<overload<std::plus<>, int>>);
  static_assert(is_defined_v<overload<std::plus<>, int>>);
}
