#pragma once

#include "sym/eval_type.hpp"

#include <compare>
#include <concepts>
#include <format>
#include <initializer_list>
#include <type_traits>

namespace sym {

class [[nodiscard]] constant
{
  eval_type_t<> value_{};

  static auto precondition_violation() -> void {}

  static consteval auto ensure(bool b)
  {
    if (not b) {
      precondition_violation();
    }
  }

  static consteval auto ensure_ordered_value(auto value)
  {
    ensure((value <=> value_type{}) != std::partial_ordering::unordered);
    return value;
  }

public:
  using value_type = eval_type_t<>;

  consteval constant() = default;

  consteval explicit constant(
      std::convertible_to<value_type> auto
          value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
      : value_{ensure_ordered_value(value)}
  {}

  consteval explicit constant(std::initializer_list<value_type> il) noexcept(
      std::is_nothrow_copy_constructible_v<value_type>)
      : value_{ensure_ordered_value([il] {
          ensure(il.size() == 1);
          return *il.begin();
        }())}
  {}

  [[nodiscard]]
  constexpr auto value() const
      noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> value_type
  {
    return value_;
  }

  [[nodiscard]]
  friend auto operator==(const constant&, const constant&) -> bool = default;

  [[nodiscard]]
  friend constexpr auto
  operator<=>(const constant& lhs, const constant& rhs) noexcept
      -> std::strong_ordering
  {
    const auto order = lhs.value() <=> rhs.value();

    if (order == 0) {
      return std::strong_ordering::equal;
    }

    return order < 0 ? std::strong_ordering::less
                     : std::strong_ordering::greater;
  }
};

}  // namespace sym

template <class Char>
struct ::std::formatter<::sym::constant, Char>
    : std::formatter<::sym::constant::value_type, Char>
{
  template <class O>
  constexpr auto format(
      const ::sym::constant& c, std::basic_format_context<O, Char>& ctx) const
  {
    return std::formatter<::sym::constant::value_type, Char>::format(
        c.value(), ctx);
  }
};
