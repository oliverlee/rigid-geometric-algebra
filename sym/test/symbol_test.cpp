#include "skytest/skytest.hpp"
#include "sym/sym.hpp"

#include <format>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;

  using namespace sym::literals;

  "symbol comparable"_test = [] {
    return expect(eq("x"_s, "x"_s) and lt("x"_s, "y"_s) and lt("a"_s, "aa"_s));
  };

  "symbol formattable"_test = [] {
    return expect(eq("x", std::format("{}", "x"_s)));
  };
}
