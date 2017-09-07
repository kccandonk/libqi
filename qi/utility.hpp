#pragma once
#ifndef _QI_UTILITY_HPP_
#define _QI_UTILITY_HPP_
#include <type_traits>
#include <memory>
#include <qi/type/traits.hpp>
#include <qi/macro.hpp>

namespace qi
{
  /// Less noisy equivalent to `std::forward`.
  ///
  /// The only purpose is to reduce the noise in libqi's generic code:
  /// occurrences of `std::forward<T>(t)` can be replaced by `fwd<T>(t)`.
  ///
  /// Note: This code is from the libstdc++ shipped with g++-7.
  template<typename T>
  BOOST_CONSTEXPR T&& fwd(typename std::remove_reference<T>::type& t) QI_NOEXCEPT(true)
  {
    return static_cast<T&&>(t);
  }

  template<typename T>
  BOOST_CONSTEXPR T&& fwd(typename std::remove_reference<T>::type&& t) QI_NOEXCEPT(true)
  {
    static_assert(!std::is_lvalue_reference<T>::value,
      "template argument substituting T is an lvalue reference type");
    return static_cast<T&&>(t);
  }

  /// Produce a L-value reference in a non-evaluated context.
  ///
  /// Note: Because of the non-evaluated context, the function need not be defined.
  ///
  /// Note: This follows the same idea as `std::declval()`.
  ///
  /// Example: Statically selecting an overload.
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// // Different overloads of `f` will return different types.
  /// template<typename T>
  /// T f(T& t) {
  ///   // ...
  /// }
  ///
  /// template<typename T>
  /// T* f(T (&a)[N]) {
  ///   // ...
  /// }
  ///
  /// template<ytpename T>
  /// struct X {
  ///   // Produce a "fake" L-value reference in a `decltype` context.
  ///   using U = decltype(f(declref<T>()));
  /// };
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  template<typename T>
  T& declref();

  /// Constructs a std::shared_ptr<T> with T deduced from the parameter.
  template <typename T>
  std::shared_ptr<traits::Decay<T>> sharedPtr(T&& t)
  {
    return std::make_shared<traits::Decay<T>>(fwd<T>(t));
  }
} // namespace qi

#endif // _QI_UTILITY_HPP_
