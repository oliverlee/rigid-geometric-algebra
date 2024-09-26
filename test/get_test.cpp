#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <tuple>
#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::get;
  using ::rigid_geometric_algebra::multivector;
  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using B0 = G2::blade<0>;
  using B1 = G2::blade<1>;

  "get invocable on tuples"_test = [] {
    return expect(
        std::is_invocable_v<decltype(get<B0>), multivector<G2, B0>> and
        std::is_invocable_v<decltype(get<B0>), multivector<G2, B0, B1>>);
  };

  "get not invocable on tuples missing element"_test = [] {
    return expect(
        (not std::is_invocable_v<decltype(get<B1>), multivector<G2, B0>>));
  };

  "get not invocable on tuples"_test = [] {
    return expect(not std::is_invocable_v<decltype(get<B0>), std::tuple<B0>>);
  };

  "get not invocable on blades"_test = [] {
    return expect(not std::is_invocable_v<decltype(get<B0>), B0>);
  };

  "get return type"_test = [] {
    return expect(
        std::is_same_v<
            B0&,
            std::invoke_result_t<decltype(get<B0>), multivector<G2, B0>&>> and
        std::is_same_v<
            const B0&,
            std::invoke_result_t<
                decltype(get<B0>),
                const multivector<G2, B0>&>> and
        std::is_same_v<
            B0&&,
            std::invoke_result_t<decltype(get<B0>), multivector<G2, B0>&&>> and
        std::is_same_v<
            const B0&&,
            std::invoke_result_t<
                decltype(get<B0>),
                const multivector<G2, B0>&&>>);
  };
}
