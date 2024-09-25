#pragma once

#include <cstddef>
#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class T, class = void>
struct canonical_type_
{
  // using type = void;
};

template <class T>
struct canonical_type_<T, std::void_t<typename T::canonical_type>>
{
  using type = typename T::canonical_type;
};

}  // namespace detail

/// obtains the canonical type of a blade
/// @tparam T type
///
/// Defines member typedef `type` as the canonical blade type if
/// `std::remove_cvref_t<T>` is a specialization of `blade`. Otherwise; does
/// not define member typedef `type`.
///
/// @{

template <class T>
struct canonical_type : detail::canonical_type_<std::remove_cvref_t<T>>
{};

template <class T>
using canonical_type_t = typename canonical_type<T>::type;

/// @}

}  // namespace rigid_geometric_algebra
