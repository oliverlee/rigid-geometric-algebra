#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::get_or;
  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using B0 = G2::blade<0>;
  using B1 = G2::blade<1>;

  struct dummy
  {};

  "get_or invocable on tuples"_test = [] {
    return expect(
        std::is_invocable_v<
            decltype(get_or<B0>),
            G2::multivector<B0::dimensions>,
            dummy> and
        std::is_invocable_v<
            decltype(get_or<B0>),
            G2::multivector<B1::dimensions>,
            dummy> and
        std::is_invocable_v<
            decltype(get_or<B0>),
            G2::multivector<B0::dimensions, B1::dimensions>,
            dummy>);
  };

  "get_or not invocable on blades"_test = [] {
    return expect(not std::is_invocable_v<decltype(get_or<B0>), B0, dummy>);
  };

  "get_or return type"_test = [] {
    using V = G2::multivector<B0::dimensions>;
    return expect(
        std::is_same_v<
            B0&,
            std::invoke_result_t<decltype(get_or<B0>), V&, dummy>> and
        std::is_same_v<
            const B0&,
            std::invoke_result_t<decltype(get_or<B0>), const V&, dummy>> and
        std::is_same_v<
            B0&&,
            std::invoke_result_t<decltype(get_or<B0>), V&&, dummy>> and
        std::is_same_v<
            const B0&&,
            std::invoke_result_t<decltype(get_or<B0>), const V&&, dummy>> and
        std::is_same_v<
            dummy&,
            std::invoke_result_t<decltype(get_or<B1>), V&, dummy&>> and
        std::is_same_v<
            const dummy&,
            std::invoke_result_t<
                decltype(get_or<B1>),
                const V&,
                const dummy&>> and
        std::is_same_v<
            dummy&&,
            std::invoke_result_t<decltype(get_or<B1>), V&&, dummy&&>> and
        std::is_same_v<
            const dummy&&,
            std::invoke_result_t<
                decltype(get_or<B1>),
                const V&&,
                const dummy&&>>);
  };
}
