#include "rigid_geometric_algebra/detail/rebind_args_into.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <tuple>
#include <type_traits>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::rebind_args_into;
  using ::rigid_geometric_algebra::detail::rebind_args_into_t;
  using ::rigid_geometric_algebra::detail::type_list;

  static_assert(
      std::is_same_v<
          std::tuple<>,
          rebind_args_into_t<std::tuple<>, std::tuple>>);

  static_assert(
      std::is_same_v<
          std::tuple<int>,
          rebind_args_into_t<std::tuple<int>, std::tuple>>);

  static_assert(
      std::is_same_v<
          type_list<int>,
          rebind_args_into_t<std::tuple<int>, type_list>>);

  static_assert(
      std::is_same_v<
          std::tuple<int, const int>,
          rebind_args_into_t<type_list<int, const int>, std::tuple>>);
}
