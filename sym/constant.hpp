#pragma once

#include "sym/eval_type.hpp"

#include <compare>
#include <concepts>
#include <format>
#include <initializer_list>
#include <limits>
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

  static consteval auto ordered(auto value) -> bool
  {
    return (value <=> value_type{}) != std::partial_ordering::unordered;
  }

  // https://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions
  // https://stackoverflow.com/questions/759201/representing-integers-in-doubles
  template <class T>
  static consteval auto non_narrowing(T) -> bool
  {
    if constexpr (
        std::numeric_limits<T>::digits <=
        std::numeric_limits<eval_type_t<>>::digits) {
      return true;
    } else {
      static_assert(false, "unimplemented");
    }
  }

public:
  using value_type = eval_type_t<>;

  consteval constant() = default;

  template <class T>
    requires std::constructible_from<value_type, T>
  consteval explicit constant(T value) noexcept(
      std::is_nothrow_constructible_v<value_type, T>)
      : value_{(
            ensure(non_narrowing(value) and ordered(value)), value_type(value))}
  {}

  consteval explicit constant(std::initializer_list<value_type> il) noexcept(
      std::is_nothrow_copy_constructible_v<value_type>)
      : value_{(ensure((il.size() == 1) and ordered(*il.begin())), *il.begin())}
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
