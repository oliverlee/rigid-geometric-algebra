#pragma once

#include <concepts>
#include <cstdlib>
#include <format>
#include <iostream>
#include <iterator>
#include <source_location>
#include <string_view>
#include <tuple>

namespace rigid_geometric_algebra::detail {

static constexpr struct
{
  std::string_view none = "\033[0m";
  std::string_view dim = "\033[2m";
  std::string_view red = "\033[31m";
} color{};

struct precondition_contract
{
  static constexpr auto name = "PRECONDITION";
  explicit precondition_contract() = default;
};

struct invariant_contract
{
  static constexpr auto name = "INVARIANT";
  explicit invariant_contract() = default;
};

struct postcondition_contract
{
  static constexpr auto name = "POSTCONDITION";
  explicit postcondition_contract() = default;
};

/// contract violation handler that only logs
///
template <class... Args>
struct logging_violation_handler
{
  // values of this class are short-lived and are not assigned
  // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
  std::ostream& os;
  const std::format_string<const Args&...> fmt;  // NOLINT(misc-include-cleaner)
  const std::tuple<const Args&...> args;
  // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

  constexpr logging_violation_handler(
      std::ostream& os,
      // NOLINTNEXTLINE(misc-include-cleaner)
      std::format_string<const Args&...> fmt,
      const Args&... args)
      : os{os}, fmt{fmt}, args{args...}
  {}

  template <class ContractKind>
  constexpr auto operator()(ContractKind, const std::source_location& sl) const
  {
    auto out = std::ostream_iterator<char>(os);

    std::format_to(
        out,
        "{}{} FAILURE{} {}{}:{}{} {}\n",
        color.red,
        ContractKind::name,
        color.none,
        color.dim,
        sl.file_name(),
        sl.line(),
        color.none,
        sl.function_name());

    std::apply(
        [out, fmt = this->fmt](const Args&... args) {
          std::format_to(out, fmt, args...);
        },
        args);

    std::format_to(out, "\n");
  }
};

/// default contract violation handler
///
/// Unary invocable that is invoked on contract failure with the source
/// location passed as an argument.
///
template <class... Args>
struct contract_violation_handler : logging_violation_handler<Args...>
{
  constexpr contract_violation_handler(
      // NOLINTNEXTLINE(misc-include-cleaner)
      std::format_string<const Args&...> fmt,
      const Args&... args)
      : logging_violation_handler<Args...>{std::cerr, fmt, args...}
  {}

  template <class ContractKind>
  constexpr auto operator()(ContractKind, const std::source_location& sl) const
  {
    logging_violation_handler<Args...>::operator()(ContractKind{}, sl);

    std::abort();
  }
};

/// check a precondition, optionally printing a message on failure
/// @param cond boolean-convertible value
/// @param violation_handler handler to invoke on contract violation.
/// @param sl source location
///
/// Check precondition `cond`. If `false`, invoke `handler(sl)`.
///
/// @{

template <class Handler>
  requires std::is_invocable_v<
      const Handler&,
      precondition_contract,
      const std::source_location&>
constexpr auto precondition(
    const std::convertible_to<bool> auto& cond,
    const Handler& violation_handler,
    const std::source_location& sl = std::source_location::current()) -> void
{
  if (cond) {
    return;
  }

  violation_handler(precondition_contract{}, sl);
}

constexpr auto precondition(
    const std::convertible_to<bool> auto& cond,
    std::format_string<> message = "",  // NOLINT(misc-include-cleaner)
    const std::source_location& sl = std::source_location::current()) -> void
{
  return precondition(cond, contract_violation_handler{message}, sl);
}

/// @}

/// check an invariant, optionally printing a message on failure
/// @param cond boolean-convertible value
/// @param violation_handler handler to invoke on contract violation.
/// @param sl source location
///
/// Check invariant `cond`. If `false`, invoke `handler(sl)`.
///
/// @{

template <class Handler>
  requires std::is_invocable_v<
      const Handler&,
      invariant_contract,
      const std::source_location&>
constexpr auto invariant(
    const std::convertible_to<bool> auto& cond,
    const Handler& violation_handler,
    const std::source_location& sl = std::source_location::current()) -> void
{
  if (cond) {
    return;
  }

  violation_handler(invariant_contract{}, sl);
}

constexpr auto invariant(
    const std::convertible_to<bool> auto& cond,
    std::format_string<> message = "",  // NOLINT(misc-include-cleaner)
    const std::source_location& sl = std::source_location::current()) -> void
{
  return invariant(cond, contract_violation_handler{message}, sl);
}

/// check a postcondition, optionally printing a message on failure
/// @param cond boolean-convertible value
/// @param violation_handler handler to invoke on contract violation.
/// @param sl source location
///
/// Check postcondition `cond`. If `false`, invoke `handler(sl)`.
///
/// @{

template <class Handler>
  requires std::is_invocable_v<
      const Handler&,
      postcondition_contract,
      const std::source_location&>
constexpr auto postcondition(
    const std::convertible_to<bool> auto& cond,
    const Handler& violation_handler,
    const std::source_location& sl = std::source_location::current()) -> void
{
  if (cond) {
    return;
  }

  violation_handler(postcondition_contract{}, sl);
}

constexpr auto postcondition(
    const std::convertible_to<bool> auto& cond,
    std::format_string<> message = "",  // NOLINT(misc-include-cleaner)
    const std::source_location& sl = std::source_location::current()) -> void
{
  return postcondition(cond, contract_violation_handler{message}, sl);
}

/// @}

}  // namespace rigid_geometric_algebra::detail
