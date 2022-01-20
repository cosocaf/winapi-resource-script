#ifndef WINRSLS_VARIANT_CAST_H_
#define WINRSLS_VARIANT_CAST_H_

#include <variant>

namespace winrsls {
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