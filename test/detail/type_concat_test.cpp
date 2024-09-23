#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/detail/type_concat.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <tuple>
#include <type_traits>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::has_type_v;
  using ::rigid_geometric_algebra::detail::type_concat;
  using ::rigid_geometric_algebra::detail::type_concat_t;
  using ::rigid_geometric_algebra::detail::type_list;

  static_assert(
      std::is_same_v<std::tuple<>, type_concat_t<std::tuple<>, std::tuple<>>>);

  static_assert(
      std::is_same_v<
          std::tuple<int>,
          type_concat_t<std::tuple<>, std::tuple<int>>>);
  static_assert(
      std::is_same_v<
          std::tuple<int>,
          type_concat_t<std::tuple<int>, std::tuple<>>>);

  static_assert(
      std::is_same_v<
          std::tuple<int, int>,
          type_concat_t<std::tuple<int>, std::tuple<int>>>);

  static_assert(
      std::is_same_v<
          std::tuple<int, const int>,
          type_concat_t<std::tuple<int>, std::tuple<const int>>>);

  static_assert(
      std::is_same_v<
          type_list<int, const int>,
          type_concat_t<type_list<int>, type_list<const int>>>);

  static_assert(
      not has_type_v<type_concat<std::tuple<int>, type_list<const int>>>);
}
