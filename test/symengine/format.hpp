#pragma once

#include <format>
#include <sstream>
#include <string_view>
#include <symengine/expression.h>

namespace test::stx {

// https://stackoverflow.com/questions/75738118/using-stdformat-with-types-that-have-operator
template <typename Char>
struct basic_ostream_formatter
    : std::formatter<std::basic_string_view<Char>, Char>
{
  template <typename T, typename OutputIt>
  constexpr auto
  format(const T& value, std::basic_format_context<OutputIt, Char>& ctx) const
      -> OutputIt
  {
    std::basic_stringstream<Char> ss;
    ss << value;
    return std::formatter<std::basic_string_view<Char>, Char>::format(
        ss.view(), ctx);
  }
};

}  // namespace test::stx

template <class CharT>
struct std::formatter<::SymEngine::Expression, CharT>
    : ::test::stx::basic_ostream_formatter<CharT>
{};
