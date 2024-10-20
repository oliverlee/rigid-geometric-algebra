#include "skytest/skytest.hpp"
#include "sym/sym.hpp"

#include <format>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;

  using namespace ::sym::literals;
  using ::sym::constant;

  "default constructible"_ctest = [] {
    return expect(eq(0, constant{}.value()));
  };

  "value constructible"_ctest = [] {
    return expect(eq(0.123, constant{0.123}.value()));
  };

  "integer constructible"_ctest = [] {
    return expect(
        eq(constant{}.value(), constant{0}.value()) and
        eq(constant{1.}.value(), constant{1}.value()));
  };

  // these need to be tested with something like:
  // https://github.com/yuyawk/rules_build_error
  //
  //"unordered value"_ctest = [] {
  //    std::ignore = constant{std::numeric_limits<double>::quiet_NaN()};
  //    return expect(true);
  //};
  //
  //"single initializer value"_ctest = [] {
  //  std::ignore = constant{1, 2};
  //  return expect(true);
  //};

  "comparable"_ctest = [] {
    return expect(
        eq(constant{42}, constant{42}) and eq(constant{-0.}, constant{+0.}) and
        lt(constant{0}, constant{1}) and lt(constant{-1}, constant{0}));
  };

  "formattable"_test = [] {
    return expect(
        eq("0", std::format("{}", constant{})) and
        eq("1", std::format("{}", constant{1.})) and
        eq("-1", std::format("{}", constant{-1.})));
  };
}
