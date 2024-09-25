#pragma once

#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class T, class = void>
struct algebra_type_
{};

template <class T>
struct algebra_type_<T, std::void_t<typename T::algebra_type>>
{
  using type = typename T::algebra_type;
};

}  // namespace detail

/// obtains the algebra type
/// @tparam T type
///
/// Defines member typedef `type` as the algebra type if
/// `std::remove_cvref_t<T>` has member typedef `algebra_type`. Otherwise;
/// does not define member typedef `type`.
///
/// @{

template <class T>
struct algebra_type : detail::algebra_type_<std::remove_cvref_t<T>>
{};

template <class T>
using algebra_type_t = typename algebra_type<T>::type;

/// @}

}  // namespace rigid_geometric_algebra
