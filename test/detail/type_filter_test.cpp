#include "rigid_geometric_algebra/detail/type_filter.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <concepts>
#include <tuple>
#include <type_traits>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::type_filter_t;
  using ::rigid_geometric_algebra::detail::type_list;

  static_assert(
      std::is_same_v<
          std::tuple<>,
          type_filter_t<std::tuple<>, []<class> { return true; }>>);

  static_assert(
      std::is_same_v<
          std::tuple<>,
          type_filter_t<std::tuple<>, []<class> { return false; }>>);

  static_assert(
      std::is_same_v<
          type_list<>,
          type_filter_t<type_list<>, []<class> { return true; }>>);

  static_assert(
      std::is_same_v<
          type_list<>,
          type_filter_t<type_list<>, []<class> { return false; }>>);

  static_assert(
      std::is_same_v<type_list<int>, type_filter_t<type_list<int>, []<class T> {
                       return std::is_same_v<T, int>;
                     }>>);

  static_assert(
      std::is_same_v<type_list<>, type_filter_t<type_list<double>, []<class T> {
                       return std::is_same_v<T, int>;
                     }>>);

  static_assert(
      std::is_same_v<
          type_list<float, double>,
          type_filter_t<type_list<int, float, double>, []<class T> {
            return std::floating_point<T>;
          }>>);
}
