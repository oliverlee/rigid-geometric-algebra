#include "rigid_geometric_algebra/detail/copy_ref_qual.hpp"

#include <type_traits>

struct T
{};

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::copy_ref_qual_t;

  static_assert(std::is_same_v<T&, copy_ref_qual_t<T&, T>>);
  static_assert(std::is_same_v<const T&, copy_ref_qual_t<const T&, T>>);
  static_assert(std::is_same_v<T&&, copy_ref_qual_t<T&&, T>>);
  static_assert(std::is_same_v<const T&&, copy_ref_qual_t<const T&&, T>>);

  static_assert(std::is_same_v<T&, copy_ref_qual_t<T&, const T>>);
  static_assert(std::is_same_v<const T&, copy_ref_qual_t<const T&, const T>>);
  static_assert(std::is_same_v<T&&, copy_ref_qual_t<T&&, const T>>);
  static_assert(std::is_same_v<const T&&, copy_ref_qual_t<const T&&, const T>>);

  static_assert(std::is_same_v<T&, copy_ref_qual_t<T&, T&>>);
  static_assert(std::is_same_v<const T&, copy_ref_qual_t<const T&, T&>>);
  static_assert(std::is_same_v<T&&, copy_ref_qual_t<T&&, T&>>);
  static_assert(std::is_same_v<const T&&, copy_ref_qual_t<const T&&, T&>>);

  static_assert(std::is_same_v<T&, copy_ref_qual_t<T&, const T&>>);
  static_assert(std::is_same_v<const T&, copy_ref_qual_t<const T&, const T&>>);
  static_assert(std::is_same_v<T&&, copy_ref_qual_t<T&&, const T&>>);
  static_assert(
      std::is_same_v<const T&&, copy_ref_qual_t<const T&&, const T&>>);

  static_assert(std::is_same_v<T&, copy_ref_qual_t<T&, T&&>>);
  static_assert(std::is_same_v<const T&, copy_ref_qual_t<const T&, T&&>>);
  static_assert(std::is_same_v<T&&, copy_ref_qual_t<T&&, T&&>>);
  static_assert(std::is_same_v<const T&&, copy_ref_qual_t<const T&&, T&&>>);

  static_assert(std::is_same_v<T&, copy_ref_qual_t<T&, const T&&>>);
  static_assert(std::is_same_v<const T&, copy_ref_qual_t<const T&, const T&&>>);
  static_assert(std::is_same_v<T&&, copy_ref_qual_t<T&&, const T&&>>);
  static_assert(
      std::is_same_v<const T&&, copy_ref_qual_t<const T&&, const T&&>>);
}
