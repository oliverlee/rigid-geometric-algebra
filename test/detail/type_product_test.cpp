#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/detail/type_product.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::has_type_v;
  using ::rigid_geometric_algebra::detail::type_list;
  using ::rigid_geometric_algebra::detail::type_product;
  using ::rigid_geometric_algebra::detail::type_product_t;

  static_assert(
      std::is_same_v<type_list<>, type_product_t<std::tuple<>, std::tuple<>>>);

  static_assert(
      std::is_same_v<
          type_list<>,
          type_product_t<std::tuple<>, std::tuple<int>>>);

  static_assert(
      std::is_same_v<
          type_list<>,
          type_product_t<std::tuple<int>, std::tuple<>>>);

  static_assert(
      std::is_same_v<
          type_list<std::pair<int, int>>,
          type_product_t<std::tuple<int>, std::tuple<int>>>);

  static_assert(
      std::is_same_v<
          type_list<
              std::pair<std::integral_constant<std::size_t, 0>,
                        std::integral_constant<std::size_t, 2>>,
              std::pair<std::integral_constant<std::size_t, 0>,
                        std::integral_constant<std::size_t, 3>>,
              std::pair<std::integral_constant<std::size_t, 0>,
                        std::integral_constant<std::size_t, 4>>,
              std::pair<std::integral_constant<std::size_t, 1>,
                        std::integral_constant<std::size_t, 2>>,
              std::pair<std::integral_constant<std::size_t, 1>,
                        std::integral_constant<std::size_t, 3>>,
              std::pair<std::integral_constant<std::size_t, 1>,
                        std::integral_constant<std::size_t, 4>>>,
          type_product_t<
              type_list<std::integral_constant<std::size_t, 0>,
                        std::integral_constant<std::size_t, 1>>,
              type_list<std::integral_constant<std::size_t, 2>,
                        std::integral_constant<std::size_t, 3>,
                        std::integral_constant<std::size_t, 4>>>>);

  static_assert(not has_type_v<type_product<int, type_list<const int>>>);
  static_assert(not has_type_v<type_product<int, int>>);
}
