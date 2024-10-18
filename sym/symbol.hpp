#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <ranges>
#include <string_view>

namespace sym {
namespace detail {

template <std::size_t N>
struct [[nodiscard]]
string_literal
{
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  const std::array<char, N - 1> chars{};

  // NOLINTNEXTLINE(modernize-avoid-c-arrays)
  consteval string_literal(const char (&name)[N])
      : chars{[&] {
          auto r = std::array<char, N - 1>{};
          std::ranges::copy(name | std::views::take(N - 1), r.begin());
          return r;
        }()}
  {}

  [[nodiscard]]
  constexpr operator std::string_view() const noexcept
  {
    return std::string_view{chars.begin(), chars.size()};
  }
};

}  // namespace detail

class [[nodiscard]] symbol
{
  std::string_view name_;

public:
  consteval explicit symbol(std::string_view name) noexcept : name_{name} {}

  [[nodiscard]]
  constexpr auto name() const noexcept -> std::string_view
  {
    return name_;
  }

  [[nodiscard]]
  friend auto operator<=>(const symbol&, const symbol&) = default;
};

inline namespace literals {

template <detail::string_literal s>
[[nodiscard]]
consteval auto operator""_s() noexcept
{
  return symbol{s};
}

}  // namespace literals
}  // namespace sym

template <class Char>
struct ::std::formatter<::sym::symbol, Char>
    : std::formatter<std::string_view, Char>
{
  template <class O>
  constexpr auto
  format(const ::sym::symbol& s, std::basic_format_context<O, Char>& ctx) const
  {
    return std::formatter<std::string_view, Char>::format(s.name(), ctx);
  }
};
