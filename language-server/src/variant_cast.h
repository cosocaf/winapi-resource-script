#ifndef WINRSLS_VARIANT_CAST_H_
#define WINRSLS_VARIANT_CAST_H_

#include <variant>

namespace winrsls {
  // template <typename... From>
  // struct VariantCastProxy {
  //   std::variant<From...> v;
  //   VariantCastProxy(const decltype(v)& v) : v(v) {}
  //   VariantCastProxy(decltype(v)&& v) : v(v) {}

  //   template <typename... To>
  //   operator std::variant<To...>() const {
  //     return std::visit([](auto&& arg) -> std::variant<To...> { return arg;
  //     },
  //                       v);
  //   }
  // };
  template <typename... To, typename... From>
  std::variant<To...> variant_cast(const std::variant<From...>& v) {
    return std::visit([](auto&& v) -> std::variant<To...> { return v; }, v);
  }
  template <typename... To, typename... From>
  std::variant<To...> variant_cast(std::variant<From...>&& v) {
    return std::visit([](auto&& v) -> std::variant<To...> { return v; },
                      std::move(v));
  }
} // namespace winrsls

#endif // WINRSLS_VARIANT_CAST_H_