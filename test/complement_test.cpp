#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "rigid_geometric_algebra/unit_hypervolume.hpp"
#include "skytest/skytest.hpp"

#include <symengine/compat.hpp>
#include <tuple>

using ::rigid_geometric_algebra::algebra_field_t;
using ::rigid_geometric_algebra::algebra_type_t;
using ::rigid_geometric_algebra::antiscalar_type_t;
using ::rigid_geometric_algebra::left_complement;
using ::rigid_geometric_algebra::one;
using ::rigid_geometric_algebra::right_complement;
using ::rigid_geometric_algebra::unit_hypervolume;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::param_ref;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
  using GS2 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 2>;

  static constexpr auto params = std::tuple{
      G2::blade<>{1},
      G2::blade<0>{2},
      G2::blade<0>{-2},
      G2::blade<1, 2>{3},
      G2::blade<0, 1, 2>{3},
      G2::blade<0, 2, 1>{3},
      G3::blade<1>{4},
      G3::blade<2>{4},
  };

  "aborts if zero coefficient"_test *
      std::tuple{
          G2::blade<>{},
          G2::blade<1>{},
          G3::blade<1, 2>{},
          GS2::blade<>{},
          GS2::blade<1>{}} = []<class B>(B b) {
    return expect(
        aborts(std::bind_front(left_complement, b)) and
        aborts(std::bind_front(right_complement, b)));
  };

  "left complement property"_ctest * param_ref<params> = []<class B>(B b) {
    using A = algebra_type_t<B>;

    return expect(eq(unit_hypervolume<A>, left_complement(b) ^ b));
  };

  "right complement property"_ctest * param_ref<params> = []<class B>(B b) {
    using A = algebra_type_t<B>;

    return expect(eq(unit_hypervolume<A>, b ^ right_complement(b)));
  };

  "complement property (symengine)"_test = [] {
    const auto b = GS2::blade<1>{"a"};

    return expect(
        eq(unit_hypervolume<GS2>, left_complement(b) ^ b) and
        eq(unit_hypervolume<GS2>, b ^ right_complement(b)) and
        eq(right_complement(left_complement(b)), b) and
        eq(left_complement(right_complement(b)), b));
  };

  "complement coefficient value"_test = [] {
    using ::SymEngine::Expression;

    const auto abs = []<class T>(T&& arg) -> Expression {
      return {::SymEngine::abs(Expression{std::forward<T>(arg)})};
    };

    const auto b = GS2::blade<1>{"a"};

    return expect(
        eq(abs("1/a"), abs(right_complement(b).coefficient)) and
        eq(abs("1/a"), abs(left_complement(b).coefficient)));
  };

  "complement of multivectors"_test = [] {
    const auto a = GS2::blade<>{"a"};
    const auto b = GS2::blade<1>{"b"};
    const auto c = GS2::blade<1, 2>{"c"};

    return expect(
        eq(left_complement(a + b + c),
           left_complement(a) + left_complement(b) + left_complement(c)) and
        eq(right_complement(a + b + c),
           right_complement(a) + right_complement(b) + right_complement(c)));
  };
}
