#pragma once

#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"

#include <format>
#include <functional>
#include <sstream>
#include <string_view>

// IWYU pragma: begin_exports
#include <symengine/assumptions.h>
#include <symengine/expression.h>
#include <symengine/functions.h>
#include <symengine/logic.h>
#include <symengine/real_double.h>
#include <symengine/refine.h>
#include <symengine/sets.h>
#include <symengine/simplify.h>
#include <symengine/symbol.h>
// IWYU pragma: end_exports

// standard library extensions
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

template <>
struct ::rigid_geometric_algebra::
    field_identity<::SymEngine::Expression, std::multiplies<>>
{
  inline static const auto value = ::SymEngine::Expression{1};
};

template <>
inline constexpr auto ::rigid_geometric_algebra::disable_line_invariant<
    ::SymEngine::Expression> = true;
