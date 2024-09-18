#include "rigid_geometric_algebra/detail/contract.hpp"
#include "skytest/skytest.hpp"

#include <sstream>
#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::rigid_geometric_algebra::detail::contract_violation_handler;
  using ::rigid_geometric_algebra::detail::logging_handler;
  using ::rigid_geometric_algebra::detail::precondition;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  "with handler"_test = [] {
    return expect(aborts([] {
      precondition(false, contract_violation_handler{"{}", 42});
    }));
  };

  "with logging"_test = [] {
    auto ss = std::stringstream{};

    precondition(false, logging_handler{ss, "value is {}", 42});

    const auto contains = pred([&ss](auto substr) {
      return ss.view().contains(substr);
    });

    return expect(
        contains("PRECONDITION FAILURE") and
        contains("test/detail/contract_test.cpp:27") and contains("main") and
        contains("value is 42"));
  };

  "with message"_test = [] {
    return expect(aborts([] { precondition(false, "failure"); }));
  };

  "without message"_test = [] {
    return expect(aborts([] { precondition(false); }));
  };

  "invocable in constant expressions"_ctest = [] {
    return expect([] {
      precondition(true);
      return true;
    }());
  };

  "cond is convertible to bool"_ctest = [] {
    return expect([] {
      precondition(std::bool_constant<true>{});
      return true;
    }());
  };

  "cond is convertible to bool"_ctest = [] {
    return expect([] {
      precondition(std::bool_constant<true>{});
      return true;
    }());
  };
}
