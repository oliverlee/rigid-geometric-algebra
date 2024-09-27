#pragma once

#include <utility>

namespace rigid_geometric_algebra {
namespace detail {

auto to_multivector() -> void;

class to_multivector_fn
{
public:
  template <class T>
  static constexpr auto
  operator()(T&& t) -> decltype(to_multivector(std::forward<T>(t)))
  {
    return to_multivector(std::forward<T>(t));
  }
};

}  // namespace detail

inline namespace cpo {
inline constexpr auto to_multivector = detail::to_multivector_fn{};
}

/// obtain the type after from invoking `to_multivector` on
/// `std::declval<T>()`
///
template <class T>
using to_multivector_t = decltype(to_multivector(std::declval<T>()));

}  // namespace rigid_geometric_algebra
