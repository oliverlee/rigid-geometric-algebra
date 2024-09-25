#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/detail/type_insert.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <tuple>
#include <type_traits>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::has_type_v;
  using ::rigid_geometric_algebra::detail::type_insert;
  using ::rigid_geometric_algebra::detail::type_insert_t;
  using ::rigid_geometric_algebra::detail::type_list;

  static_assert(std::is_same_v<std::tuple<>, type_insert_t<std::tuple<>>>);

  static_assert(
      std::is_same_v<std::tuple<int>, type_insert_t<std::tuple<>, int>>);
  static_assert(
      std::is_same_v<std::tuple<int>, type_insert_t<std::tuple<int>>>);

  static_assert(
      std::
          is_same_v<std::tuple<int, int>, type_insert_t<std::tuple<int>, int>>);

  static_assert(
      std::is_same_v<
          std::tuple<int, const int>,
          type_insert_t<std::tuple<int>, const int>>);

  static_assert(
      std::is_same_v<
          type_list<int, char, int>,
          type_insert_t<type_list<int>, char, int>>);

  static_assert(not has_type_v<type_insert<int, std::tuple<>>>);
}
