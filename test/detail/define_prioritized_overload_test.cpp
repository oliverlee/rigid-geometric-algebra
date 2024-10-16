#include "rigid_geometric_algebra/detail/define_prioritized_overload.hpp"
#include "rigid_geometric_algebra/detail/derive_subtraction.hpp"
#include "rigid_geometric_algebra/detail/derive_vector_space_operations.hpp"
#include "rigid_geometric_algebra/detail/invoke_prioritized_overload.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/priority_list.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
#include <cstdlib>
#include <type_traits>

struct test_op
{};

struct get_value
{
  static constexpr auto
  operator()(auto&& arg) -> decltype(std::forward<decltype(arg)>(arg).value)
  {
    return std::forward<decltype(arg)>(arg).value;
  }
};

namespace rigid_geometric_algebra::detail {

// this is normally determined from the specialized priority list
template <>
inline constexpr auto max_priority<::test_op> = 2;

}  // namespace rigid_geometric_algebra::detail

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::define_prioritized_overload_v;
  using ::rigid_geometric_algebra::detail::invoke_prioritized_overload;
  using ::rigid_geometric_algebra::detail::overload;

  static_assert(
      define_prioritized_overload_v<
          0,
          overload<test_op>,
          std::integral_constant<std::size_t, 0>>);
  static_assert(0 == invoke_prioritized_overload<test_op>());

  static_assert(
      // an overload associated with `test_op` is already "defined"
      not define_prioritized_overload_v<
          1,
          overload<test_op>,
          std::integral_constant<std::size_t, 1>>);
  static_assert(1 == invoke_prioritized_overload<test_op>());

  static_assert(
      // an overload associated with `test_op` is already "defined"
      not define_prioritized_overload_v<
          2,
          overload<test_op>,
          std::integral_constant<std::size_t, 2>>);
  static_assert(2 == invoke_prioritized_overload<test_op>());

  // Another call to `define_priotized_overload_v` results in a hard-error
  //
  // static_assert(
  //     not define_prioritized_overload_v<
  //         1,
  //         overload<test_op>,
  //         std::integral_constant<std::size_t, 3>>);

  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;

  struct A
  {
    int value{};

    auto operator-() const -> A { std::abort(); }

    constexpr auto operator+(A rhs) const -> A { return {value + rhs.value}; }
    constexpr auto operator-(A rhs) const -> A { return {value - rhs.value}; }
    constexpr auto operator*(A rhs) const -> A { return {value * rhs.value}; }

    auto operator==(const A&) const -> bool = default;
  };

  struct B
      : ::rigid_geometric_algebra::detail::
            derive_vector_space_operations<B, get_value>,
        ::rigid_geometric_algebra::detail::derive_subtraction<B>
  {
    A value{};

    constexpr B(int v) : value{v} {}
    constexpr B(A v) : value{v} {}

    auto operator==(const B&) const -> bool = default;
  };

  "negation of A aborts"_test = [] {
    return expect(aborts([] { return -A{}; }));
  };

  "prioritizes subtraction from vector space operations"_test = [] {
    // i.e. avoids using negation from the underlying type A
    return expect(eq(B{-1}, B{1} - B{2}));
  };
}
